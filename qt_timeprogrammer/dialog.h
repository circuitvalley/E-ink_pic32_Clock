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

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "masterthread.h"

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QComboBox;

QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);

private slots:
    void transaction();
    void get_time();
    void set_time();
    void showcurrentTime();
    void showResponse(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);

private:
    void setControlsEnabled(bool enable);

private:
    int transactionCount;
    QLabel *serialPortLabel;
    QComboBox *serialPortComboBox;
    QLabel *waitResponseLabel;
    QSpinBox *waitResponseSpinBox;
    QLabel *requestLabel;
    QLabel *trafficLabel;
    QLabel *statusLabel;
    QPushButton *linkCheckButton;
    QPushButton *setTime;
    QPushButton *getTime;

    MasterThread thread;
};

#endif // DIALOG_H
