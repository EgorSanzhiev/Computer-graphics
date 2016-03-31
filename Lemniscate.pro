#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T21:06:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lemniscate
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    consolemode.cpp \
    controller.cpp \
    controlspanel.cpp \
    controlswidget.cpp \
    drawwidget.cpp \
    lemniscate.cpp \
    point.cpp

HEADERS  += mainwindow.h \
    consolemode.h \
    controller.h \
    controlspanel.h \
    controlswidget.h \
    drawwidget.h \
    iserializable.h \
    lemniscate.h \
    point.h \
    intmath.h
