#-------------------------------------------------
#
# Project created by QtCreator 2016-11-06T17:33:39
#
#-------------------------------------------------

QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BluetoothLE
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    deviceinfo.cpp \
    waitingspinnerwidget.cpp \
    displayplot.cpp \
    bluetoothleuart.cpp

HEADERS  += mainwindow.h \
    deviceinfo.h \
    waitingspinnerwidget.h \
    displayplot.h \
    bluetoothleuart.h

FORMS    += mainwindow.ui


# QWTPlot Library - please change path!
LIBS += -L /home/nico/Qt5.7.0/qwt-6.1.3/lib -lqwt
INCLUDEPATH +=  /home/nico/Qt5.7.0/qwt-6.1.3/src

