#-------------------------------------------------
#
# Project created by QtCreator 2016-05-26T11:49:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Plotter
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
    intmath.cpp

HEADERS  += mainwindow.h \
    consolemode.h \
    controller.h \
    controlspanel.h \
    controlswidget.h \
    drawwidget.h \
    iserializable.h \
    lemniscate.h \
    intmath.h
