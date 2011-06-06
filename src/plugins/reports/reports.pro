QT += sql \
    xml
TEMPLATE = lib
CONFIG += plugin

TARGET = $$qtLibraryTarget(Reports)

INCLUDEPATH += $$quote(../../../include/)

debug:LIBS += $$quote(../../../libs/libncreportd.a)
release:LIBS += $$quote(../../../libs/libncreportd.a)

linux-g++ {
    release: DESTDIR = ../../../bin/release/unix/plugins/depot
    debug: DESTDIR = ../../../bin/debug/unix/plugins/depot
}

win32 {
    release: DESTDIR = ../../../bin/release/win32/plugins/depot
    debug: DESTDIR = ../../../bin/debug/win32/plugins/depot
}

HEADERS += \
    ../../plugininterface.h \
    reports.h \
    wizardsubject.h \
    selecttest.h

SOURCES += \
    reports.cpp \
    wizardsubject.cpp \
    selecttest.cpp

RESOURCES += \
    res.qrc

FORMS += \
    wizardsubject.ui \
    selecttest.ui
