QT += xml
TEMPLATE = lib
CONFIG += plugin
SOURCES += \
    puzzlequest.cpp \
    editor.cpp \
    ../../Questions.cpp \
    pieceslist.cpp \
    puzzlewidget.cpp \
    showpanel.cpp

TARGET = $$qtLibraryTarget(PuzzleQuestion)

HEADERS += \
    puzzlequest.h \
    editor.h \
    ../../questeditorinterface.h \
    ../../Questions.h \
    pieceslist.h \
    puzzlewidget.h \
    ../../showpanelinterface.h \
    showpanel.h

linux-g++ {
    release: DESTDIR = ../../../bin/release/unix/TestTypes/
    debug: DESTDIR = ../../../bin/debug/unix/TestTypes/
}

win32 {
    release: DESTDIR = ../../../bin/release/win32/TestTypes/
    debug: DESTDIR = ../../../bin/debug/win32/TestTypes/
}

FORMS += \
    editor.ui
