#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QStyleFactory>
#include <qtextcodec.h>
#include <QSplashScreen>
#include <time.h>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                     QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);
    MainWindow::initResSystem();

    qsrand((unsigned)time(new time_t()));

    QPixmap map(MainWindow::pixFile("interface\\splash.png"));
    QSplashScreen splash(map);
    splash.show();
    a.processEvents();
    a.setStyle("windowsmodernstyle");//windowsmodernstyle NorwegianWood

    MainWindow w;
    w.connectToDataBase();
    w.setupInterface();
    w.show();
    splash.finish(&w);
    return a.exec();
}
