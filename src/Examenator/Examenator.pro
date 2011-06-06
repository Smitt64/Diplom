# -------------------------------------------------
# Project created by QtCreator 2010-12-04T21:46:34
# -------------------------------------------------
QT += sql \
    xml
TARGET = Examenator
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    ../filesystem/filesystem.cpp \
    ../questpanel/questpanel.cpp \
    ../questpanel/questionwidgt.cpp \
    ../Questions.cpp \
    ../questpanel/answerwidget.cpp \
    ../questpanel/textbrowser.cpp \
    ConSettings.cpp
HEADERS += mainwindow.h \
    ../filesystem/filesystem.h \
    ../questpanel/questpanel.h \
    ../questpanel/questionwidgt.h \
    ../Questions.h \
    ../questpanel/answerwidget.h \
    ../questpanel/textbrowser.h \
    ConSettings.h \
    ../showpanelinterface.h
FORMS += mainwindow.ui \
    ../questpanel/questpanel.ui \
    ../questpanel/questionwidgt.ui \
    ../questpanel/answerwidget.ui \
    consettings.ui
debug:DEFINES += FS_DEBUG \
    DEBUG_MSG
DEPLOYMENT_PLUGIN += qjpeg \
    qmng \
    qgif
RESOURCES += ../../res/res.qrc \
    ../../res/Examenator/examenator.qrc

linux-g++ {
    release: DESTDIR = ../../bin/release/unix/
    debug: DESTDIR = ../../bin/debug/unix/
}

win32 {
    release: DESTDIR = ../../bin/release/win32/
    debug: DESTDIR = ../../bin/debug/win32/
}

android-g++ {
    DEFINES += OS_ANDROID
}
