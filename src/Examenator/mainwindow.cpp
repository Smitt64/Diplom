#include "mainwindow.h"
#include "ConSettings.h"
#include "QFileDialog"
#include <QFile>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QStringList arguments, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    panel(NULL)
{
    ui->setupUi(this);

    for(int i = 1; i < arguments.count(); i++)
    {
        QString arg = arguments[i];
        int pos = arg.indexOf(':');
        QString varName = arg.left(pos);
        arg = arg.remove(varName + ":");

#ifndef DEBUG_MSG
        qDebug() << varName << arg;
#endif
        this->cmd.insert(varName, arg);
    }//ebay.com

    if(this->hasCmdParam("file"))
    {
        this->connectToDataBase();
        this->panel = new QuestPanel(this->getCmdParam("file"), this);
        this->setCentralWidget(this->panel);
        this->panel->updateTopics();

        this->setWindowTitle(this->panel->getWindowCaption());
        this->panel->setUnsatisfactory(this->hasCmdParam("-uq"));

        bool mixQ = false;
        bool mixA = false;
        if(this->hasCmdParam("-mixQ"))
            mixQ = (bool)this->getCmdParam("-mixq").toInt();
        if(this->hasCmdParam("-mixA"))
            mixA = (bool)this->getCmdParam("-mixa").toInt();

        int rnd = 0;
        if(this->hasCmdParam("-rnd"))
            rnd = this->getCmdParam("-rnd").toInt();

        if(this->hasCmdParam("-t"))
            this->panel->buildQuestionList(this->getCmdParam("-t"), rnd, mixQ, mixA);
        else
            this->panel->buildQuestionList("all", rnd, mixQ, mixA);

        this->panel->showStartScreen();
        connect(this->panel, SIGNAL(startTestButton()), this, SLOT(onStartTest()));
    }
    else
    {
        connect(this->ui->pushButton, SIGNAL(clicked()), this, SLOT(onOpen()));
    }

    this->setAutoFillBackground(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpen()
{
    QString fName = QFileDialog::getOpenFileName(this, "Открыть тест...");

    if(!fName.isEmpty())
    {
        this->connectToDataBase();
        this->panel = new QuestPanel(fName, this);
        this->panel->updateTopics();
        this->setCentralWidget(this->panel);

        this->panel->buildQuestionList("all", 0, false, false);

        this->panel->setGeometry(this->geometry());

        this->panel->showStartScreen();
        this->pr->setWindowFlags(this->pr->windowFlags() | Qt::WindowStaysOnTopHint);
        this->pr->showFullScreen();
        connect(this->panel, SIGNAL(startTestButton()), this, SLOT(onStartTest()));
    }
    //this->setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);| Qt::WindowTitleHint | Qt::CustomizeWindowHint
}

void MainWindow::setViewWidget(QWidget *widget)
{
    this->pr = widget;
}

bool MainWindow::hasCmdParam(QString name)
{
    return this->cmd.contains(name);
}

QString MainWindow::getCmdParam(QString name)
{
    if(this->hasCmdParam(name))
        return this->cmd[name];
    return "";
}

void MainWindow::onConnectionSettings()
{
    ConSettings dlg(this);

    if(dlg.exec() == QDialog::Accepted)
    {
        QSettings settings("Depot", "Examenator", this);
        settings.setValue("server", dlg.getServer());
        settings.setValue("port", dlg.getPort());

        if(this->db.databaseName() == "depot")
        {
                this->db.close();
                connectToDataBase();
        }
    }

#ifndef DEBUG_MSG
        qDebug() << this->db.databaseName();
#endif
}

void MainWindow::connectToDataBase()
{
    QSettings settings("Depot", "Examenator", this);

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("server", "localhost").toString());
    db.setPort(settings.value("port", "3306").toInt());
    db.setPassword("NXkNEoZHQB");//NXkNEoZHQB
    db.setUserName("Examenator");//redactor
    db.setDatabaseName("depot");
    if(!db.open())
    {
        QMessageBox dlg(this);
        dlg.setText("Не удалось подключиться к базе данных!");
        dlg.setInformativeText("Проверьте пареметры подключения.");
        dlg.setDetailedText(db.lastError().text());
        dlg.setIcon(QMessageBox::Critical);
        if(dlg.exec() == QMessageBox::Ok)
        {
            onConnectionSettings();
        }
    }
}

void MainWindow::onStartTest()
{
    this->proxy->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    this->proxy->update();
    this->show();
}

QString MainWindow::textFile(QString fname)
{
    QFile f(fname);
    if(f.open(QIODevice::ReadOnly))
    {
        QString str;
        str = f.readAll();
        f.close();
        return str;
    }

    return "";
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qApp->quit();
}

void MainWindow::setProxyWidget(QGraphicsProxyWidget *value)
{
    this->proxy = value;
}
