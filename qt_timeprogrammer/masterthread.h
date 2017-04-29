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

#ifndef MASTERTHREAD_H
#define MASTERTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

//! [0]
class MasterThread : public QThread
{
    Q_OBJECT

public:
    explicit MasterThread(QObject *parent = nullptr);
    ~MasterThread();

    void transaction(const QString &portName, const QString &request);
    void run() Q_DECL_OVERRIDE;

signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    QString portName;
    QString request;
    int waitTimeout;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
};
//! [0]

#endif // MASTERTHREAD_H
