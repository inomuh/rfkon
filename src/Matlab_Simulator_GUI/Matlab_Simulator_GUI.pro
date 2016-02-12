#-------------------------------------------------
#
# Project created by QtCreator 2015-09-08T10:29:35
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Matlab_Simulator_GUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpacceptor.cpp \
    tcpconnector.cpp \
    tcpstream.cpp

HEADERS  += mainwindow.h \
    tcpacceptor.h \
    tcpconnector.h \
    tcpstream.h

FORMS    += mainwindow.ui

DISTFILES += \
    Matlab_Simulator_GUI.pro.user
