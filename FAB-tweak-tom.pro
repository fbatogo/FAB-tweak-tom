#-------------------------------------------------
#
# Project created by QtCreator 2016-01-27T20:12:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FAB-tweak-tom
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    createbedlevelinggcode.cpp \
    gcodeeditor.cpp \
    logger.cpp

HEADERS  += mainwindow.h \
    createbedlevelinggcode.h \
    gcodeeditor.h \
    logger.h

FORMS    += mainwindow.ui
