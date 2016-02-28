#-------------------------------------------------
#
# Project created by QtCreator 2016-02-24T23:16:52
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Circle
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    circle.cpp \
    drawwidget.cpp \
    controlswidget.cpp \
    controlspanel.cpp \
    controller.cpp

HEADERS  += mainwindow.h \
    circle.h \
    drawwidget.h \
    controlspanel.h \
    controlswidget.h \
    controller.h
