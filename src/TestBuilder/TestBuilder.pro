# -------------------------------------------------
# Project created by QtCreator 2010-10-20T18:02:48
# -------------------------------------------------
QT += sql \
    xml
TARGET = TestBuilder
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    ../filesystem/filesystem.cpp \
    ../questpanel/questpanel.cpp \
    testproperties.cpp \
    texteditor.cpp \
    questeditor.cpp \
    groupeditor.cpp \
    ../Questions.cpp \
    ../questpanel/answerwidget.cpp \
    ../questpanel/questionwidgt.cpp \
    ../questpanel/textbrowser.cpp \
    texteditwidget.cpp \
    settings.cpp
HEADERS += mainwindow.h \
    ../filesystem/filesystem.h \
    ../questpanel/questpanel.h \
    testproperties.h \
    texteditor.h \
    questeditor.h \
    groupeditor.h \
    ../Questions.h \
    ../questpanel/answerwidget.h \
    ../questpanel/questionwidgt.h \
    ../questpanel/textbrowser.h \
    texteditwidget.h \
    settings.h \
    ../testbuilderplugininterface.h \
    ../questeditorinterface.h \
    ../showpanelinterface.h
FORMS += mainwindow.ui \
    ../questpanel/questpanel.ui \
    testproperties.ui \
    texteditor.ui \
    questeditor.ui \
    groupeditor.ui \
    ../questpanel/answerwidget.ui \
    ../questpanel/questionwidgt.ui \
    settings.ui
    
DEFINES += REDACTOR

debug:DEFINES += FS_DEBUG \
    DEBUG_MSG
    
RESOURCES += ../../res/res.qrc

RC_FILE = ../../res/TestBuilder/sample.rc

TRANSLATIONS = editor_ru.ts

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
