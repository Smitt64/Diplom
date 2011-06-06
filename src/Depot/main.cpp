#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QStyleFactory>
#include <qtextcodec.h>

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);

    a.setStyle("windowsmodernstyle");

    MainWindow w;
    w.connectToDataBase();
    w.setupPlugins();
    w.setupInterface();
    w.show();
    return a.exec();
}

