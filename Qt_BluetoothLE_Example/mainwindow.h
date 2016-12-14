/****************************************************************************
**
** This file is part of the Bluetooth LE UART example written by Nico Lingg
**
**
** Author: Nico Lingg
** VERSION: 0.1
** Date: 09/12/2016 (DD/MM/YYYY)
**
**
** Permission is hereby granted, free of charge, to any person obtaining a copy of
** this software and associated documentation files (the "Software"), to deal in
** the Software without restriction, including without limitation the rights to
** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
** the Software, and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <QLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QString>
#include <QDebug>
#include <QtWidgets>
#include <qregularexpression.h>

#include "displayplot.h"
#include "waitingspinnerwidget.h"
#include "bluetoothleuart.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void DataHandler(const QString &s);
    void connectDevice();
    void toggleLED();
    void sendData();
    void changedState(bluetoothleUART::bluetoothleState state);

 signals:
    void connectToDevice(int i);


private:
    void createPlotGroupBox();
    void createComboGroupBox();
    void createValueDisplayGroupBox();
    void createMenuGroupBox();
    void createStatusBar();

    void showStatusBarMessage(const QString &text);

    Ui::MainWindow *ui;
    WaitingSpinnerWidget *m_spinner;
    QLabel *m_status;
    QLabel *m_lTemperature;
    QLabel *m_lHumidity;
    QComboBox m_cbDevicesFound;
    QGroupBox *m_plotGroupBox;
    QGroupBox *m_comboGroupBox;
    QGroupBox *m_valueDisplayGroupBox;
    QGroupBox *m_menuGroupBox;
    QList<QString> m_qlFoundDevices;
    QPushButton *m_pBSearch;
    QPushButton *m_pBConnect;
    QPushButton *m_pBLED;
    QPushButton *m_pBsend;
    QLineEdit *m_lineEdit;
    bluetoothleUART m_bleConnection;
    displayplot *m_tempDisplayPlot;
    displayplot *m_humDisplayPlot;

    /* Variables */
    bool m_LED_ON;
    float m_fTemperature;
    float m_fHumidity;


};

#endif // MAINWINDOW_H
