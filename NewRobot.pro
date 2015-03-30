#-------------------------------------------------
#
# Project created by QtCreator 2015-02-12T09:06:43
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleRobot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    robot/control.cpp \
    robot/model.cpp \
    robot/robot.cpp \
    robot/stlparser.cpp \
    robot/view.cpp

HEADERS  += mainwindow.h \
    robot/control.h \
    robot/model.h \
    robot/robot.h \
    robot/stlparser.h \
    robot/view.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
