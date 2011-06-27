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
    QCoreApplication::setOrganizationName("Smitt64Company");
    QCoreApplication::setApplicationName("TestBuilder");

    QTranslator translator;
    if(!translator.load("editor_" + QLocale::system().name()))
        translator.load("editor_en_EN");
    a.installTranslator(&translator);

    MainWindow::initResSystem();

    qsrand((unsigned)time(new time_t()));

    QPixmap map(MainWindow::pixFile("interface\\splash.png"));
    QSplashScreen splash(map);
    splash.show();
    a.processEvents();
    a.setStyle("windowsmodernstyle");

    MainWindow w;
    w.connectToDataBase();
    w.setupInterface();
    w.show();
    splash.finish(&w);
    return a.exec();
}
