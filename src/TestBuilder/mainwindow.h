#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../questpanel/questpanel.h"
#include "QStyleFactory"
#include <qdir.h>
#include <qdebug.h>
#include <qaction.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qsqlerror.h>
#include <qsqlquery.h>
#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <qsqlrecord.h>
#include <qsqlresult.h>
#include <qsettings.h>
#include <qtreewidget.h>
#include <qgraphicsview.h>
#include <qlistwidget.h>
#include <qtoolbutton.h>
#include <qinputdialog.h>
#include "../filesystem/filesystem.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //Работа с файловой системой
    static FSHANDLE *resHandle();
    static QByteArray fileData(QString name);
    static QPixmap pixFile(QString name);
    static QString textFile(QString name);
    static void initResSystem();
    static FSHANDLE *resources;

    static void addTempFile(QString fname);

    //Настройка внешнего вида
    void setupInterface();

    //Подключение к базе данных
    void connectToDataBase();
    static void setStyleHelper(QWidget *widget, QStyle *style);

protected:
    FSHANDLE testFile;
    QTreeWidgetItem *curItem;
    void setupDockPanel();
    void changeEvent(QEvent *e);

    void updateQuestTree();
    void updateQuestList();
    void updateTestToolControls(bool state);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);
    void setCurrentFile(QString fileName);

    QString getQuestName(QString value);

    static QStringList tempFiles;

    virtual void closeEvent(QCloseEvent *event);

protected slots:
    void onNew();
    void onOpen();
    void openTest(QString fileName);
    void onSave();
    void onPrint();
    void onCloseTest();
    void onTestProperty();
    void openRecentFile();

    void onAddQuestGroup();
    void onEditQuestGroup();
    void onRenameQuestion();
    void onAddQuestion();

    void onChangeQuestion();

    void onRemoveGroup();
    void onRemoveFromGroup();
    void onRemoveQuestion();

    void onShowRight(bool value);
    void onShowTab(bool value);
    void onShowEndButton(bool value);
    void onShowNextButton(bool value);

    void onChangeQuestTreeItem(QTreeWidgetItem * item, int column);
    void onQuestListItemSelect(QListWidgetItem *item);

    void onMoveUp();
    void onMoveDown();
    void onConnectionSettings();

    void onChangeStyle();
    void about();

private:
    Ui::MainWindow *ui;
    QToolButton *toolBtn;
    QToolButton *removeBtn;
    QAction *newAction, *openAction, *saveAction, *saveAsAction, *closeAction, *printAction;
    QAction *testProp;
    QAction *viewMainPage, *viewComment;

    QAction *addQuestGroup, *editQuestGroup, *addQuestion, *editQuestion, *renameQuestion;
    QAction *removeGroup, *removeQuestion, *removeFromGroup;
    QAction *showRight, *showQuestTab, *showEndButton, *showNextButton;

    QAction *moveUp, *moveDown;

    enum{MaxRecentFiles = 5};
    QAction *separatorAct;
    QAction *recentFileActs[MaxRecentFiles];

    QToolBar *questToolBar;
    QuestPanel *panel;
    QToolBar *dockBar;
    QTreeWidget *questTree;
    QListWidget *questList;

    QSqlDatabase db;
};

#endif // MAINWINDOW_H
