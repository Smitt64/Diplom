#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QLabel>

class QMdiArea;
class QSignalMapper;
class QSqlDatabase;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void connectToDataBase();
    void setupPlugins();
    void setupInterface();

private slots:
    void onMenu();
    void tabRemoved(int index);
    void setActiveSubWindow(QWidget *window);
    void addSubWindow(QWidget *widget);
    void onChangeStyle();

    void beginTransaction();

private:
    QSqlDatabase dBase;
    void addToMenu(QString menuName, QObject *plugin, QStringList texts);
    Ui::MainWindow *ui;
    QMdiArea *mdiArea;
    QMenu *styleMenu;
    QAction *beginTran, *commitTran, *rollTran;
    QAction *closeWnd, *closeAllWnd, *titleWnd, *cascadeWnd, *nextWnd, *prevWnd;
    QSignalMapper *windowMapper;

    QLabel *tranLabel;

    bool transaction;
};

#endif // MAINWINDOW_H
