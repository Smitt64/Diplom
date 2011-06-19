#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qsqlrecord.h>
#include <qsqlresult.h>
#include <qsettings.h>
#include <qsqlerror.h>
#include <qsqlquery.h>
#include <qsqlquerymodel.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include <qmap.h>
#include <QGraphicsProxyWidget>
#include <QGraphicsBlurEffect>
#include <QPropertyAnimation>
#include "../questpanel/questpanel.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QStringList arguments, QWidget *parent = 0);
    ~MainWindow();

    //Подключение к базе данных
    void connectToDataBase();

    bool hasCmdParam(QString name);
    QString getCmdParam(QString name);
    void onConnectionSettings();
    void setViewWidget(QWidget *widget);
    void setProxyWidget(QGraphicsProxyWidget *value);

    static QString textFile(QString fname);
private slots:
    void onStartTest();
    void onOpen();

protected:
    virtual void closeEvent(QCloseEvent *event);

signals:
    void onOpenTest();
    void onClose();

private:
    QSqlDatabase db;
    QuestPanel *panel;
    QMap<QString, QString> cmd;
    Ui::MainWindow *ui;
    QWidget *pr;
    QGraphicsProxyWidget *proxy;
    QGraphicsBlurEffect blur;
};

#endif // MAINWINDOW_H
