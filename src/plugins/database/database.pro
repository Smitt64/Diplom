# -------------------------------------------------
# Project created by QtCreator 2010-10-20T18:02:48
# -------------------------------------------------
QT += sql \
    xml

OBJECTS_DIR = .build
MOC_DIR = .build
UI_DIR = .build

TEMPLATE = lib
CONFIG += plugin

TARGET = $$qtLibraryTarget(DBManager)

HEADERS += \
    databasemanager.h \
    ../../plugininterface.h \
    speciality.h \
    students.h \
    addspeciality.h \
    addgroup.h

SOURCES += \
    databasemanager.cpp \
    speciality.cpp \
    students.cpp \
    addspeciality.cpp \
    addgroup.cpp

FORMS += \
    speciality.ui \
    students.ui \
    addspeciality.ui \
    addgroup.ui

RESOURCES += \
    res.qrc

linux-g++ {
    release: DESTDIR = ../../../bin/release/unix/plugins/depot
    debug: DESTDIR = ../../../bin/debug/unix/plugins/depot
}

win32 {
    release: DESTDIR = ../../../bin/release/win32/plugins/depot
    debug: DESTDIR = ../../../bin/debug/win32/plugins/depot
}
