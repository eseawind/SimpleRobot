#-------------------------------------------------
#
# Project created by QtCreator 2015-02-12T09:06:43
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NewRobot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    stlparser.cpp \
    model.cpp \
    view.cpp \
    control.cpp \
    robot.cpp

HEADERS  += mainwindow.h \
    stlparser.h \
    model.h \
    view.h \
    control.h \
    robot.h

FORMS    += mainwindow.ui

DISTFILES += \
    assistant.stl \
    bar.stl \
    base.stl \
    master.stl \
    assistant-ascii.stl \
    bar-ascii.stl \
    base-ascii.stl \
    master-ascii.stl \
    NewRobot.pro.user \
    total-ascii.stl \
    adsgouraud.fsh \
    adsphong.fsh \
    adsgouraud.vsh \
    adsphong.vsh

RESOURCES += \
    res.qrc
