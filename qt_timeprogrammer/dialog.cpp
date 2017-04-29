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

#include "dialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QTime>
#include <QTimer>
#include <QtSerialPort/QSerialPortInfo>

QT_USE_NAMESPACE

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , transactionCount(0)
    , serialPortLabel(new QLabel(tr("Serial port:")))
    , serialPortComboBox(new QComboBox())
    , trafficLabel(new QLabel(tr("No traffic.")))
    , statusLabel(new QLabel(tr("Status: Not running.")))
    , linkCheckButton(new QPushButton(tr("Link Check")))
    , setTime(new QPushButton(tr("Set Time")))
    , getTime(new QPushButton(tr("Get Time")))
{
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        serialPortComboBox->addItem(info.portName());


    auto mainLayout = new QGridLayout;
    mainLayout->addWidget(serialPortLabel, 0, 0 ,1,1 );
    mainLayout->addWidget(serialPortComboBox, 0, 1,1,1);
    mainLayout->addWidget(linkCheckButton, 1, 0, 1, 2);
    mainLayout->addWidget(setTime,2,0,1,2);
    mainLayout->addWidget(getTime,3,0,1,2);
    mainLayout->addWidget(statusLabel, 4, 0, 1, 2);

    setLayout(mainLayout);

    setWindowTitle(tr("TimeProgrammer"));
    setMinimumSize(240,150);
    setMaximumSize(240,150);
    serialPortComboBox->setFocus();

    connect(linkCheckButton, &QPushButton::clicked, this, &Dialog::transaction);
    connect(getTime, &QPushButton::clicked,this,&Dialog::get_time);
    connect(setTime, &QPushButton::clicked,this,&Dialog::set_time);
    connect(&thread, &MasterThread::response, this, &Dialog::showResponse);
    connect(&thread, &MasterThread::error, this, &Dialog::processError);
    connect(&thread, &MasterThread::timeout, this, &Dialog::processTimeout);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showcurrentTime()));
    timer->start(1000);


}
void Dialog::showcurrentTime()
{
QTime times = (QTime::currentTime());
QString currentTime=times.toString("hh:mm:ss");
setTime->setText(tr("Set Time : %1").arg(currentTime));
}

void Dialog::transaction()
{
    setControlsEnabled(false);
    statusLabel->setText(tr("Status: Open, port %1.")
                         .arg(serialPortComboBox->currentText()));
    thread.transaction(serialPortComboBox->currentText(),
                       "whoareyou\r");
}

void Dialog::get_time()
{
    setControlsEnabled(false);
    statusLabel->setText(tr("Status: Open, port %1.")
                         .arg(serialPortComboBox->currentText()));
    thread.transaction(serialPortComboBox->currentText(),
                       "get_time\r");
}
void Dialog::set_time()
{
    QTime times = (QTime::currentTime()).addMSecs(100);
    QString currentTime=times.toString("hh:mm:ss");
    setControlsEnabled(false);

    statusLabel->setText(tr("Status: Open, port %1.")
                         .arg(serialPortComboBox->currentText()));
    thread.transaction(serialPortComboBox->currentText(),
                       tr("set_time %1\r")
                       .arg(currentTime));
}
void Dialog::showResponse(const QString &s)
{
    setControlsEnabled(true);

    if(!QString::compare(tr("%1").arg(s).remove('\n').remove('\r'), "cveink", Qt::CaseInsensitive) || !QString::compare(tr("%1").arg(s).remove('\n').remove('\r'), "Error", Qt::CaseInsensitive) )
    {
    linkCheckButton->setText(tr("Connected To: %1").arg(s).remove('\n'));
    }else
    {
    getTime->setText(tr("Get Time : %1").arg(s).remove('\n'));
    }
}

void Dialog::processError(const QString &s)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Not running, %1.").arg(s));
    trafficLabel->setText(tr("No traffic."));
}

void Dialog::processTimeout(const QString &s)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Running, %1.").arg(s));
    trafficLabel->setText(tr("No traffic."));
}

void Dialog::setControlsEnabled(bool enable)
{
    linkCheckButton->setEnabled(enable);
    getTime->setEnabled(enable);
    setTime->setEnabled(enable);

    serialPortComboBox->setEnabled(enable);

  //  requestLineEdit->setEnabled(enable);
}
