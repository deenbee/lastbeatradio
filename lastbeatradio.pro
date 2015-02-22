#-------------------------------------------------
#
# Project created by QtCreator 2013-06-22T18:24:11
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lastbeatradio
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    digitalclock.cpp \
    audioplayer.cpp

HEADERS  += mainwindow.h \
    digitalclock.h \
    audioplayer.h

FORMS    += mainwindow.ui

win32 {
INCLUDEPATH += C:\Qt\taglib-1.8
INCLUDEPATH += C:\Qt\taglib-1.8\taglib\toolkit
INCLUDEPATH += C:\Qt\taglib-1.8\taglib

LIBS += -L"C:\Qt\taglib-1.8\taglib-release" \
    libtaglib
}

linux {
LIBS += -ltag
}

RESOURCES += Resource.qrc
