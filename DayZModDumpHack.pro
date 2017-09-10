#-------------------------------------------------
#
# Project created by QtCreator 2017-08-28T13:30:16
#
#-------------------------------------------------

QT       += core gui xml widgets concurrent

TARGET = DayZModDumpHack
TEMPLATE = app

VERSION = 0.5.5.0
QMAKE_TARGET_COMPANY = EvilWind
QMAKE_TARGET_PRODUCT = DayZ Mod Dump Hack
QMAKE_TARGET_DESCRIPTION = Dump Hack
QMAKE_TARGET_COPYRIGHT = EvilWind

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    worldstate.cpp \
    interactivemap.cpp \
    memoryapi.cpp \
    sidepanel.cpp \
    entitydata.cpp \
    settingsmanager.cpp

HEADERS += \
        mainwindow.h \
    worldstate.h \
    interactivemap.h \
    memoryapi.h \
    sidepanel.h \
    entitydata.h \
    settingsmanager.h

RC_ICONS = icon.ico

RESOURCES += \
    resources.qrc
