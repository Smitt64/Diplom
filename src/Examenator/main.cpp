#include <QtGui/QApplication>
#include "mainwindow.h"
#include <qtextcodec.h>
#include <qdebug.h>
#include <qlibrary.h>
#include <qmessagebox.h>
#include <time.h>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    qsrand((unsigned)time(new time_t()));

    //Параметры запуска
    QStringList list = a.arguments();

    QGraphicsScene scene;
    QGraphicsView *view = new QGraphicsView(&scene);
    QWidget *desc = qApp->desktop();
    scene.setStickyFocus(true);
    scene.setSceneRect(desc->geometry());

    MainWindow *w = new MainWindow(list);
    w->setViewWidget(view);
    w->setProperty("pos", QPoint((desc->width() - w->width()) / 2, (desc->height() - w->height()) / 2));

    QGraphicsProxyWidget *window = scene.addWidget(w, Qt::Window);
    w->setProxyWidget(window);
    window->setWidget(w);
    scene.addItem(window);

    view->setRenderHints(view->renderHints() | QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setBackgroundBrush(QPixmap(":/back"));
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->showFullScreen();

    return a.exec();
}
