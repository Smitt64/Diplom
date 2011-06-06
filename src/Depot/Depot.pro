# -------------------------------------------------
# Project created by QtCreator 2010-10-20T18:02:48
# -------------------------------------------------
QT += sql \
    xml
TEMPLATE = app
TARGET = Depot
DEPLOYMENT_PLUGIN += qjpeg \
    qmng \
    qgif

SOURCES += \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    mainwindow.h \
    ../plugininterface.h \

FORMS += \
    mainwindow.ui

linux-g++ {
    release: DESTDIR = ../../bin/release/unix/
    debug: DESTDIR = ../../bin/debug/unix/
}

win32 {
    HEADERS += ../msstyle/windowsmodernstyle.h
    SOURCES += ../msstyle/windowsmodernstyle.cpp
    release: DESTDIR = ../../bin/release/win32/
    debug: DESTDIR = ../../bin/debug/win32/
}

RESOURCES += \
    ../../res/Depot/depot.qrc
