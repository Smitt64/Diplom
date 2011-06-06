#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QPluginLoader>
#include <QDir>
#include <QDebug>
#include <QToolBar>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QSignalMapper>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QStyleFactory>
#include "../plugininterface.h"

#define TO_INTERFACE(object) (qobject_cast<PluginInterface*>(object))

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    transaction(false)
{
    ui->setupUi(this);

    mdiArea = new QMdiArea;
    mdiArea->setActivationOrder(QMdiArea::CreationOrder);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setCentralWidget(mdiArea);

    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)),
                 this, SLOT(setActiveSubWindow(QWidget*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMenu()
{
    QAction *action = qobject_cast<QAction *>(sender());
    PluginInterface *plug = qobject_cast<PluginInterface*>(action->parent());
    QString text = action->text();
    //this->ui->tabWidget->addTab(plug->central(text, this), action->icon(), text);

    QWidget *central = plug->central(text, this);
    QMdiSubWindow *w = mdiArea->addSubWindow(plug->central(text, this));
    w->setGeometry(central->geometry());
    w->show();
}

void MainWindow::setActiveSubWindow(QWidget *window)
 {
     if (!window)
         return;
     mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
 }

void MainWindow::connectToDataBase()
{
    QSettings settings("Depot", "Depot", this);

    dBase = QSqlDatabase::addDatabase("QMYSQL");
    dBase.setHostName(settings.value("server", "127.0.0.1").toString());
    dBase.setPort(settings.value("port", "3306").toInt());
    dBase.setPassword("NXkNEoZHQB");
    dBase.setUserName("redactor");
    dBase.setDatabaseName("depot");

    if(!dBase.open())
    {
        QMessageBox dlg(this);
        dlg.setText("Не удалось подключиться к базе данных!");
        dlg.setDetailedText(dBase.lastError().text());
        dlg.setIcon(QMessageBox::Critical);
        dlg.exec();
        return;
    }
}

void MainWindow::addToMenu(QString menuName, QObject *plugin, QStringList texts)
{
    QMenu *menu = new QMenu(menuName);
    QToolBar *bar = new QToolBar();
    this->addToolBar(bar);
    this->ui->menubar->addMenu(menu);
    //this->ui->menubar->addMenu()
    foreach(QString text, texts)
    {
        QAction *action = new QAction(text, plugin);
        action->setIcon(TO_INTERFACE(plugin)->getIcon(text));
        connect(action, SIGNAL(triggered()), this, SLOT(onMenu()));

        menu->addAction(action);

        bar->addAction(action);
    }

    menu->addActions(TO_INTERFACE(plugin)->commands());

}

void MainWindow::setupPlugins()
{
    qDebug() << qApp->applicationDirPath();
    QDir pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("plugins");
    pluginsDir.cd("depot");

    foreach(QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();

        if (plugin)
        {
            PluginInterface *plug = qobject_cast<PluginInterface*>(plugin);
            addToMenu(plug->menu(), (QObject*)plugin, plug->windows());
        }
    }
}

void MainWindow::tabRemoved(int index)
{
    //this->ui->tabWidget->removeTab(index);
}

void MainWindow::addSubWindow(QWidget *widget)
{
    QMdiSubWindow *w = mdiArea->addSubWindow(widget);
    w->show();
}

void MainWindow::setupInterface()
{
    QSettings settings("Depot", "Depot", this);
    qApp->setStyle(settings.value("style", "Windows").toString());

    QMenu *serviceMenu = this->ui->menubar->addMenu("Сервис");
    beginTran = serviceMenu->addAction("Транзакция");
    commitTran = serviceMenu->addAction("Применить изменения");
    rollTran = serviceMenu->addAction("Отменить изменения");
    serviceMenu->addSeparator();
    styleMenu = serviceMenu->addMenu("Оформление");

    QMenu *wndMenu = this->ui->menubar->addMenu("Окна");
    this->closeWnd = wndMenu->addAction("Закрыть");
    this->closeAllWnd = wndMenu->addAction("Закрыть все окна");
    wndMenu->addSeparator();
    this->titleWnd = wndMenu->addAction("Рядом");
    this->cascadeWnd = wndMenu->addAction("Каскадом");
    wndMenu->addSeparator();

    foreach(QString styleName, QStyleFactory::keys())
    {
        QAction *action = styleMenu->addAction(styleName, this, SLOT(onChangeStyle()));
        action->setCheckable(true);
        if(qApp->style()->objectName().toLower() == styleName.toLower())
        {
            action->setChecked(true);
        }
    }

    tranLabel = new QLabel;
    tranLabel->setGeometry(0, 0, 16, 16);
    tranLabel->setFrameShape(QFrame::NoFrame);
    tranLabel->setFrameShadow(QFrame::Plain);
    this->ui->statusbar->addPermanentWidget(this->tranLabel);

    QPixmap icon = QIcon(":/transaction").pixmap(QSize(16, 16), QIcon::Disabled, QIcon::Off);
    this->tranLabel->setPixmap(icon);

    //this->ui->menubar->addMenu(wndMenu);
    connect(this->closeWnd, SIGNAL(triggered()), this->mdiArea, SLOT(closeActiveSubWindow()));
    connect(this->closeAllWnd, SIGNAL(triggered()), this->mdiArea, SLOT(closeAllSubWindows()));
    connect(this->titleWnd, SIGNAL(triggered()), this->mdiArea, SLOT(tileSubWindows()));
    connect(this->cascadeWnd, SIGNAL(triggered()), this->mdiArea, SLOT(cascadeSubWindows()));

    connect(this->beginTran, SIGNAL(triggered()), this, SLOT(beginTransaction()));
}

void MainWindow::onChangeStyle()
{
    QAction *action = qobject_cast<QAction*>(sender());

    for(int i = 0; i < styleMenu->actions().count(); i++)
        styleMenu->actions().at(i)->setChecked(false);

    action->setChecked(true);
    qApp->setStyle(action->text());

    QSettings settings("Depot", "Depot", this);
    settings.setValue("style", action->text());
}

void MainWindow::beginTransaction()
{
    if(!this->dBase.transaction())
    {
        this->transaction = false;
        QMessageBox::critical(this, "Ошибка транзакции", "Не удалось запустить транзакцию");
    }
    else
    {
        transaction = true;
        QPixmap icon = QIcon(":/transaction").pixmap(QSize(16, 16), QIcon::Normal, QIcon::On);
        this->tranLabel->setPixmap(icon);
    }
}
