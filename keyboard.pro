#-------------------------------------------------
#
# Project created by QtCreator 2018-04-13T12:14:38
#
#-------------------------------------------------

QT       += core gui
QT += network widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = keyboard
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14
#QMAKE_CFLAGS = -std=gnu++14
#QMAKE_CXXFLAGS = -std=gnu++14
#QMAKE_CXXFLAGS -= -std=gnu++11

LIBS+= $$OUT_PWD\release\tobii_stream_engine.dll


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    arrows.cpp \
    udp.cpp \
    mythread.cpp \
    eyetracker.cpp \
    control_rules.cpp

HEADERS += \
        mainwindow.h \
    include/tobii/tobii.h \
    include/tobii/tobii_engine.h \
    include/tobii/tobii_streams.h \
    include/tobii/tobii_wearable.h \
    mythread.h \
    arrows.h \
    control_rules.h \
    udp.h \
    eyetracker.h

FORMS +=

DISTFILES += \
    lib/x86/tobii_stream_engine.lib
