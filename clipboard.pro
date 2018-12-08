#-------------------------------------------------
#
# Project created by QtCreator 2016-12-03T21:02:16
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = clipboard
TEMPLATE = app
#CONFIG += app_bundle
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

SOURCES += main.cpp\
    helper.cpp \
    history.cpp

HEADERS  += \
    helper.h \
    history.h

FORMS    += \
    history.ui

RESOURCES += \
    qrc.qrc

ICON = clipboard.icns

DISTFILES += \
    clipboard.icns \
    clipboard.rc
