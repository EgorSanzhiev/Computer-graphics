#-------------------------------------------------
#
# Project created by QtCreator 2016-04-12T20:40:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Font
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp\
        consolemode.cpp\
        controller.cpp\
        controlspanel.cpp\
        controlswidget.cpp\
        drawwidget.cpp \
    figure.cpp

HEADERS += mainwindow.h\
        consolemode.h\
        controller.h\
        controlspanel.h\
        controlswidget.h\
        drawwidget.h\
        iserializable.h \
    figure.h
