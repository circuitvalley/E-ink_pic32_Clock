/*
 * Author:  Gaurav
 * website: www.circuitvalley.com
 * Created on 20 March, 2017, 1:33 AM
 *	This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *	Email: gauravsingh@circuitvalley.com
************************************************************************/

#include "masterthread.h"

#include <QtSerialPort/QSerialPort>

#include <QTime>

QT_USE_NAMESPACE

MasterThread::MasterThread(QObject *parent)
    : QThread(parent), waitTimeout(0), quit(false)
{
}

//! [0]
MasterThread::~MasterThread()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}
//! [0]

//! [1] //! [2]
void MasterThread::transaction(const QString &portName, const QString &request)
{
    //! [1]
    QMutexLocker locker(&mutex);
    this->portName = portName;
    this->waitTimeout = 1000; //1sec
    this->request = request;
    //! [3]
    if (!isRunning())
        start();
    else
        cond.wakeOne();
}
//! [2] //! [3]

//! [4]
void MasterThread::run()
{
    bool currentPortNameChanged = false;

    mutex.lock();
    //! [4] //! [5]
    QString currentPortName;
    if (currentPortName != portName) {
        currentPortName = portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = waitTimeout;
    QString currentRequest = request;
    mutex.unlock();
    //! [5] //! [6]
    QSerialPort serial;

    serial.setBaudRate(serial.Baud115200);
    while (!quit) {
        //![6] //! [7]
        if (currentPortNameChanged) {
            serial.close();
            serial.setPortName(currentPortName);

            if (!serial.open(QIODevice::ReadWrite)) {
                emit error(tr("Can't open %1, error code %2")
                           .arg(portName).arg(serial.error()));
                return;
            }
        }
        //! [7] //! [8]
        // write request
        QByteArray requestData = currentRequest.toLocal8Bit();
        serial.write(requestData);
        if (serial.waitForBytesWritten(waitTimeout)) {
            //! [8] //! [10]
            // read response
            if (serial.waitForReadyRead(currentWaitTimeout)) {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(10))
                    responseData += serial.readAll();

                QString response(responseData);
                //! [12]
                emit this->response(response);
                //! [10] //! [11] //! [12]
            } else {
                emit timeout(tr("Wait read response timeout %1")
                             .arg(QTime::currentTime().toString()));
            }
            //! [9] //! [11]
        } else {
            emit timeout(tr("Wait write request timeout %1")
                         .arg(QTime::currentTime().toString()));
        }
        //! [9]  //! [13]
        mutex.lock();
        cond.wait(&mutex);
        if (currentPortName != portName) {
            currentPortName = portName;
            currentPortNameChanged = true;
        } else {
            currentPortNameChanged = false;
        }
        currentWaitTimeout = waitTimeout;
        currentRequest = request;
        mutex.unlock();
    }
    //! [13]
}
