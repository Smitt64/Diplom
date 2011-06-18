#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "testproperties.h"
#include <qtoolbutton.h>
#include <QPropertyAnimation>
#include <qtreewidget.h>
#include <texteditor.h>
#include <qgraphicsview.h>
#include <questeditor.h>
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <groupeditor.h>
#include <qtooltip.h>
#include <qprinter.h>
#include <qprintdialog.h>
#include <qradiobutton.h>
#include <qwizard.h>
#include <qbuttongroup.h>
#include "settings.h"

FSHANDLE *MainWindow::resources;
QStringList MainWindow::tempFiles;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    panel(NULL)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::setupInterface()
{
    QSettings settings("Depot", "TestBuilder", this);
    setupDockPanel();
    questToolBar = new QToolBar("Вопросы", this);
    this->addToolBar(questToolBar);

    newAction = new QAction(QIcon(pixFile("interface\\toolbar\\DocumentHS.png")), "Создать", this);
    newAction->setShortcut(QKeySequence::New);
    openAction = new QAction(QIcon(pixFile("interface\\toolbar\\openHS.png")), "Открыть...", this);
    openAction->setShortcut(QKeySequence::Open);
    saveAction = new QAction(QIcon(pixFile("interface\\toolbar\\saveHS.png")), "Сохранить...", this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction = new QAction(QIcon(pixFile("interface\\toolbar\\SaveAllHS.png")), "Сохранить как...", this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    printAction = new QAction(QIcon(pixFile("interface\\toolbar\\PrintHS.png")), "Печатать тест...", this);
    printAction->setShortcut(QKeySequence::Print);

    closeAction = new QAction("Закрыть", this);
    closeAction->setShortcut(QKeySequence::Close);

    testProp = new QAction(QIcon(pixFile("interface\\toolbar\\PropertiesHS.png")), "Настройки...", this);

    showRight = new QAction(QIcon(pixFile("interface\\toolbar\\Flag_redHS.png")), "Правильные ответы", this);
    showRight->setCheckable(true);

    showQuestTab = new QAction(QIcon(pixFile("interface\\toolbar\\InsertTabControlHS.png")), "Строка вопросов", this);
    showQuestTab->setCheckable(true);
    showQuestTab->setChecked(true);

    showEndButton = new QAction(QIcon(pixFile("interface\\toolbar\\BreakpointHS.png")), "Кнопка 'Завершить'", this);
    showEndButton->setCheckable(true);
    showEndButton->setChecked(false);

    showNextButton = new QAction(QIcon(pixFile("interface\\toolbar\\PreviousPageHS.png")), "Перемещение по вопросам", this);
    showNextButton->setCheckable(true);
    showNextButton->setChecked(true);

    toolBtn = new QToolButton(this);
    toolBtn->setIcon(QIcon(pixFile("interface\\toolbar\\FormRunHS.png")));
    toolBtn->setPopupMode(QToolButton::InstantPopup);
    QMenu *prevMenu = new QMenu("Предпросмотр", toolBtn);
    prevMenu->setIcon(QIcon(pixFile("interface\\toolbar\\FormRunHS.png")));
    toolBtn->setMenu(prevMenu);

    viewMainPage = new QAction("Выбор группы и студента", this);
    viewMainPage->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_M));
    viewComment = new QAction("Коментарий", this);

    prevMenu->addAction(viewMainPage);
    prevMenu->addAction(viewComment);

    questToolBar->addWidget(toolBtn);

    //Список последних файлов
    for(int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    this->ui->questMainMenu->addMenu(prevMenu);
    //Меню создать
    this->ui->mainToolBar->addAction(newAction);
    this->ui->fileMenu->addAction(newAction);
    //Меню открыть
    this->ui->mainToolBar->addAction(openAction);
    this->ui->fileMenu->addAction(openAction);
    //Меню закрыть
    this->ui->fileMenu->addAction(closeAction);
    this->ui->fileMenu->addSeparator();
    //Меню сохранить
    this->ui->mainToolBar->addAction(saveAction);
    this->ui->fileMenu->addAction(saveAction);
    //Меню сохранить как
    this->ui->mainToolBar->addAction(saveAsAction);
    this->ui->fileMenu->addAction(saveAsAction);
    //Меню печать

    this->ui->mainToolBar->addSeparator();
    this->ui->mainToolBar->addAction(printAction);
    this->ui->fileMenu->addAction(printAction);
    this->ui->fileMenu->addSeparator();
    //Разделитель
    this->ui->mainToolBar->addSeparator();
    this->ui->fileMenu->addSeparator();

    for(int i = 0; i < MaxRecentFiles; ++i)
        this->ui->fileMenu->addAction(recentFileActs[i]);

    this->ui->fileMenu->addSeparator();

    updateRecentFileActions();

    QAction *exitAction = new QAction("Выход", this);
    exitAction->setShortcut(QKeySequence::Quit);
    this->ui->fileMenu->addAction(exitAction);

    questToolBar->addAction(testProp);
    questToolBar->addAction(showRight);
    questToolBar->addSeparator();
    questToolBar->addAction(showNextButton);
    questToolBar->addAction(showQuestTab);
    questToolBar->addAction(showEndButton);

    qApp->setStyle(settings.value("style", "Windows").toString());

    foreach(QString styleName, QStyleFactory::keys())
    {
        QAction *action= this->ui->styleMenu_2->addAction(styleName, this, SLOT(onChangeStyle()));
        action->setCheckable(true);
        if(qApp->style()->objectName().toLower() == styleName.toLower())
        {
            action->setChecked(true);
        }
    }

    this->ui->questMainMenu->addAction(showRight);
    this->ui->questMainMenu->addAction(showNextButton);
    this->ui->questMainMenu->addAction(showQuestTab);
    this->ui->questMainMenu->addAction(showEndButton);

    this->saveAction->setEnabled(false);
    this->saveAsAction->setEnabled(false);
    this->printAction->setEnabled(false);

    this->updateTestToolControls(false);


    this->setDockOptions(QMainWindow::ForceTabbedDocks | QMainWindow::VerticalTabs
                         | QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);
    setCorner(Qt::TopLeftCorner, Qt::TopDockWidgetArea);
    this->tabifyDockWidget(this->ui->questlistDock, this->ui->questDock);

    //Подключение событий
    connect(newAction, SIGNAL(triggered()), this, SLOT(onNew()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(onOpen()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(onSave()));
    connect(testProp, SIGNAL(triggered()), this, SLOT(onTestProperty()));//onShowRight,showRight
    connect(printAction, SIGNAL(triggered()), this, SLOT(onPrint()));
    connect(showQuestTab, SIGNAL(triggered(bool)), this, SLOT(onShowTab(bool)));
    connect(showNextButton, SIGNAL(triggered(bool)), this, SLOT(onShowNextButton(bool)));
    connect(showEndButton, SIGNAL(triggered(bool)), this, SLOT(onShowEndButton(bool)));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(closeAction, SIGNAL(triggered()), this, SLOT(onCloseTest()));
    connect(this->ui->connectAction, SIGNAL(triggered()), this, SLOT(onConnectionSettings()));

    connect(this->ui->about_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(this->ui->about, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::about()
{
    QMessageBox msg(this);
    msg.setIconPixmap(this->windowIcon().pixmap(70, QIcon::Normal, QIcon::On));
    msg.setWindowTitle("О программе...");
    msg.setText(MainWindow::textFile("interface\\about.html"));
    msg.exec();
}

void MainWindow::onChangeStyle()
{
    QAction *action = qobject_cast<QAction*>(sender());

    for(int i = 0; i < this->ui->styleMenu_2->actions().count(); i++)
        this->ui->styleMenu_2->actions().at(i)->setChecked(false);

    action->setChecked(true);
    qApp->setStyle(action->text());

    QSettings settings("Depot", "TestBuilder", this);
    settings.setValue("style", action->text());
}

void MainWindow::setStyleHelper(QWidget *widget, QStyle *style)
{
    //Применить тему оформления
    widget->setStyle(style);
    widget->setPalette(style->standardPalette());
    foreach (QObject *child, widget->children())
    {
        if(QWidget *childWidget = qobject_cast<QWidget*>(child))
            setStyleHelper(childWidget, style);
    }
}

void MainWindow::addTempFile(QString fname)
{
    MainWindow::tempFiles.push_back(fname);
}

QByteArray MainWindow::fileData(QString name)
{
    QByteArray data;
    FileSystem::getInst()->fsOpen(MainWindow::resources);
    data = FileSystem::getInst()->fsGetFile(name, MainWindow::resources);
    FileSystem::getInst()->fsClose(MainWindow::resources);
    return data;
}

QPixmap MainWindow::pixFile(QString name)
{
    FileSystem::getInst()->fsOpen(MainWindow::resources);
    QPixmap data;
    data.loadFromData(FileSystem::getInst()->fsGetFile(name, MainWindow::resources));
    FileSystem::getInst()->fsClose(MainWindow::resources);

    return data;
}

QString MainWindow::textFile(QString name)
{
    QByteArray data;
    FileSystem::getInst()->fsOpen(MainWindow::resources);
    data = FileSystem::getInst()->fsGetFile(name, MainWindow::resources);
    FileSystem::getInst()->fsClose(MainWindow::resources);

    return QString(data);
}

void MainWindow::initResSystem()
{
    MainWindow::resources = new FSHANDLE;
    MainWindow::resources->archFName = QString(QDir::current().path() + "/TestBuilder.arc");


#ifdef DEBUG_MSG
    qDebug() << "Файл ресурсов" << MainWindow::resources->archFName;
#endif

}

void MainWindow::onNew()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Создать..."),
                                QDir::currentPath(),
                                tr("Файл теста (*.tst)"));

    if(!fileName.isEmpty())
    {
#ifdef DEBUG_MSG
        qDebug() << "Новый файл теста: " << fileName;
#endif
        FileSystem::getInst()->fsCreate(fileName, &this->testFile);
        QByteArray dta = fileData("query\\select_groups_template.sql");
        FileSystem::getInst()->fsAddFile(dta, QString("query\\select_groups.sql"), &this->testFile, 2);
        dta = fileData("query\\select_students.sql");
        FileSystem::getInst()->fsAddFile(dta, QString("query\\select_students.sql"), &this->testFile, 2);

        dta = fileData("templates\\editor.xml");
        FileSystem::getInst()->fsAddFile(dta, QString("editor.xml"), &this->testFile, 2);

        dta = fileData("templates\\questions.xml");
        FileSystem::getInst()->fsAddFile(dta, QString("questions.xml"), &this->testFile, 2);

        dta = fileData("templates\\answerlist.txt");
        FileSystem::getInst()->fsAddFile(dta, QString("answerlist.txt"), &this->testFile, 2);

        dta = fileData("templates\\report_template.html");
        FileSystem::getInst()->fsAddFile(dta, QString("report_template.html"), &this->testFile, 2);

        dta = fileData("templates\\Comment.html");
        FileSystem::getInst()->fsAddFile(dta, QString("Comment.html"), &this->testFile, 2);
        FileSystem::getInst()->fsClose(&this->testFile);

        if(this->panel)
            viewMainPage->disconnect(this->panel, SLOT(showStartScreen()));

        this->panel = new QuestPanel(fileName, this);
        this->panel->updateTopics();
        this->setCentralWidget(this->panel);

        if(this->panel)
        {
            connect(viewMainPage, SIGNAL(triggered()), this->panel, SLOT(showStartScreen()));
            connect(showRight, SIGNAL(triggered(bool)), this->panel, SLOT(showRight(bool)));
        }

        this->panel->showStartScreen();
        this->panel->showRight(false);

        this->updateTestToolControls(true);
        this->saveAction->setEnabled(true);
        this->saveAsAction->setEnabled(true);
        this->printAction->setEnabled(true);
        updateQuestTree();

        this->setWindowTitle(QString("TestBuilder - %1").arg(fileName));
    }
}

void MainWindow::openTest(QString fileName)
{
    if(this->panel)
    {
        viewMainPage->disconnect(this->panel, SLOT(showStartScreen()));
        delete this->panel;
    }

    this->testFile.archFName = fileName;
    this->panel = new QuestPanel(fileName, this);
    this->panel->updateTopics();

    if(!this->panel->passwordOnOpen.isEmpty())
    {
        bool ok;
        QString pass = QInputDialog::getText(this, "Тест защищен...",
                                             "Введите пароль для разблокировки:", QLineEdit::Password,
                                             "", &ok);
        if(!ok || pass.isEmpty() || this->panel->passwordOnOpen != pass)
        {
            return;
        }
    }

    this->panel->showStartScreen();
    this->setCentralWidget(this->panel);

    this->saveAction->setEnabled(true);
    this->saveAsAction->setEnabled(true);
    this->printAction->setEnabled(true);

    connect(viewMainPage, SIGNAL(triggered()), this->panel, SLOT(showStartScreen()));
    connect(showRight, SIGNAL(triggered(bool)), this->panel, SLOT(showRight(bool)));

    this->panel->showRight(false);

    this->updateTestToolControls(true);
    updateQuestTree();
    updateQuestList();

    this->setWindowTitle(QString("TestBuilder - %1").arg(fileName));
    setCurrentFile(fileName);
}

void MainWindow::onCloseTest()
{
    if(this->panel)
    {
        FileSystem::getInst()->fsOpen(&this->testFile);
        FileSystem::getInst()->fsDeleteTempFiles(&this->testFile);
        FileSystem::getInst()->fsClose(&this->testFile);

        viewMainPage->disconnect(this->panel, SLOT(showStartScreen()));

        this->saveAction->setEnabled(false);
        this->saveAsAction->setEnabled(false);
        this->printAction->setEnabled(false);

        this->questTree->clear();
        this->questList->clear();

        this->updateTestToolControls(false);

        this->setWindowTitle("TestBuilder");

        delete this->panel;
        this->panel = NULL;
    }
}

void MainWindow::onOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть..."),
                                QDir::currentPath(),
                                tr("Файл теста (*.tst)"));

    qDebug() << "Открытие: " << fileName;
    if(!fileName.isEmpty())
    {
        openTest(fileName);
    }
}

void MainWindow::onSave()
{
    FileSystem::getInst()->fsOpen(&this->testFile);
    QByteArray xmlConf = this->panel->writeXmlConfig();

    FileSystem::getInst()->fsFlushTempFiles(&this->testFile);

    if(FileSystem::getInst()->fsHasFile("questions.xml", &this->testFile))
        FileSystem::getInst()->fsDelete("questions.xml", &this->testFile);

    FileSystem::getInst()->fsAddFile(xmlConf, "questions.xml", &this->testFile, 2);
    FileSystem::getInst()->fsClose(&this->testFile);
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if(action)
    {
        openTest(action->data().toString());
    }
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings("Depot", "TestBuilder", this);
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for(int i = 0; i < numRecentFiles; ++i)
    {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for(int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::setCurrentFile(QString fileName)
{
    QSettings settings("Depot", "TestBuilder", this);
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while(files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);

    this->updateRecentFileActions();
}

void MainWindow::updateQuestTree()
{
    this->questTree->clear();

    QStringList list = this->panel->getTopicNames();

    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, "Темы");
    item->setIcon(0, QIcon(pixFile("interface\\icons\\book.png")));
    item->setData(0, Qt::UserRole, 1);
    this->questTree->addTopLevelItem(item);

    for(int i = 0; i < this->panel->topicsCount(); i++)
    {
        QTreeWidgetItem *subItem = new QTreeWidgetItem;
        subItem->setText(0, list[i]);
        subItem->setData(0, Qt::UserRole, 0);
        subItem->setIcon(0, QIcon(pixFile("interface\\icons\\group.png")));

        this->panel->getTopic(list[i])->setTreeItem(subItem);
        //Добавление вопросов
        for(int j = 0; j < this->panel->getTopic(list[i])->questCount(); j++)
        {
            QTreeWidgetItem *questItem = new QTreeWidgetItem;
            QString text = list[i] + ": " + this->panel->getTopic(list[i])->getQuestName(j);
            questItem->setText(0, text);
            questItem->setIcon(0, QIcon(pixFile("interface\\icons\\question.png")));
            QString name = QString::number(i) + ";" + QString::number(j);
            questItem->setData(0, Qt::UserRole, name);

            subItem->addChild(questItem);
        }

        item->addChild(subItem);
    }
#ifndef DEBUG_MSG
    qDebug() << "Обновление дерева...";
#endif
}

void MainWindow::updateQuestList()
{
    QStringList questions = this->panel->getQuestNames();

    this->questList->clear();

    for(int i = 0; i < questions.count(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(questions[i]);
        item->setIcon(QIcon(pixFile("interface\\icons\\question.png")));

        this->questList->addItem(item);
    }

    if(this->questList->count())
        this->questList->setCurrentRow(0);

#ifndef DEBUG_MSG
    qDebug() << "Обновление списка...";
#endif
}

void MainWindow::onConnectionSettings()
{
    Settings dlg(this);

    if(dlg.exec() == QDialog::Accepted)
    {
        QSettings settings("Depot", "TestBuilder", this);
        settings.setValue("server", dlg.getServer());
        settings.setValue("port", dlg.getPort());

        if(this->db.databaseName() == "depot")
        {
            if(QMessageBox::question(this, "Подключение.", "Переподключиться к базе данных?",
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            {
                this->db.close();
                connectToDataBase();
            }

        }

#ifndef DEBUG_MSG
        qDebug() << this->db.databaseName();
#endif
    }
}

void MainWindow::connectToDataBase()
{
    QSettings settings("Depot", "TestBuilder", this);
    //Строка подключения
    /*QString conStr = QString("DRIVER={MySql ODBC 5.1 Driver};SERVER=:host;PORT=:port;DATABASE=depot;")
                     + QString("USER=redactor;PASSWORD=NXkNEoZHQB");//redactor  NXkNEoZHQB

    conStr.replace(":host", settings.value("server", "127.0.0.1").toString());
    conStr.replace(":port", settings.value("port", "3306").toString());
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(conStr);

    if(!db.open())
    {
        QMessageBox dlg(this);
        dlg.setText("Не удалось подключиться к базе данных!");
        dlg.setDetailedText(db.lastError().text());
        dlg.setIcon(QMessageBox::Critical);
        dlg.exec();
    }*/

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("server", "127.0.0.1").toString());
    db.setPort(settings.value("port", "3306").toInt());
    db.setPassword("NXkNEoZHQB");//NXkNEoZHQB
    db.setUserName("redactor");//redactor
    db.setDatabaseName("depot");
    if(!db.open())
    {
        QMessageBox dlg(this);
        dlg.setText("Не удалось подключиться к базе данных!");
        dlg.setInformativeText("Проверьте пареметры подключения.");
        dlg.setDetailedText(db.lastError().text());
        dlg.setIcon(QMessageBox::Critical);
        if(dlg.exec() == QDialog::Accepted)
        {
            onConnectionSettings();
        }
    }
}

void MainWindow::setupDockPanel()
{
    /*
      Структура
      */
    removeBtn = new QToolButton;
    dockBar = new QToolBar(this);
    questTree = new QTreeWidget;
    questTree->setHeaderHidden(true);
    questTree->setWordWrap(true);

    QMainWindow *wnd = new QMainWindow;
    wnd->setMinimumSize(150, 0);
    wnd->setCentralWidget(questTree);

    dockBar->setFloatable(false);
    dockBar->setMovable(false);
    wnd->addToolBar(dockBar);

    this->ui->questDock->setWidget(wnd);

    addQuestGroup = new QAction(QIcon(pixFile("interface\\toolbar\\AddGroup.png")), "Добавить тему", this);
    addQuestGroup->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    addQuestion = new QAction(QIcon(pixFile("interface\\toolbar\\AddQuestion.png")), "Добавить вопрос", this);
    addQuestion->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    editQuestGroup = new QAction(QIcon(pixFile("interface\\toolbar\\LegendHS.png")), "Редактировать список вопросов", this);
    editQuestGroup->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));

    renameQuestion = new QAction(QIcon(pixFile("interface\\toolbar\\RenameQuestion.png")), "Переименовать вопрос", this);

    editQuestion = new QAction(QIcon(pixFile("interface\\toolbar\\EditInformationHS.png")), "Редактировать вопрос", this);

    removeGroup = new QAction(QIcon(pixFile("interface\\toolbar\\remove.png")), "Удалить тему", this);
    removeQuestion = new QAction(QIcon(pixFile("interface\\toolbar\\remove.png")), "Удалить вопрос", this);
    removeFromGroup = new QAction(QIcon(pixFile("interface\\toolbar\\removeFromGroup.png")), "Удалить вопрос из группы", this);

    moveUp = new QAction(QIcon(pixFile("interface\\toolbar\\Up.png")), "Переместить вверх.", this);
    moveUp->setEnabled(false);
    moveDown = new QAction(QIcon(pixFile("interface\\toolbar\\Down.png")), "Переместить вниз.", this);
    moveDown->setEnabled(false);

    dockBar->addAction(addQuestGroup);
    dockBar->addAction(addQuestion);
    dockBar->addAction(editQuestGroup);
    dockBar->addAction(editQuestion);
    dockBar->addSeparator();
    dockBar->addAction(moveUp);
    dockBar->addAction(moveDown);
    dockBar->addSeparator();
    dockBar->addAction(removeFromGroup);
    dockBar->addAction(removeGroup);

    /*
      Вопросы
      */
    questList = new QListWidget;
    QToolBar *dockBar2 = new QToolBar;
    QMainWindow *wnd2 = new QMainWindow;
    wnd2->setMinimumSize(150, 0);
    wnd2->setCentralWidget(questList);

    dockBar2->setFloatable(false);
    dockBar2->setMovable(false);
    wnd2->addToolBar(dockBar2);
    this->ui->questlistDock->setWidget(wnd2);

    dockBar2->addAction(addQuestion);
    dockBar2->addAction(editQuestion);
    dockBar2->addAction(renameQuestion);
    dockBar2->addSeparator();
    dockBar2->addAction(removeQuestion);

    this->ui->groupMenu->addAction(addQuestGroup);
    this->ui->groupMenu->addAction(removeFromGroup);
    this->ui->groupMenu->addAction(removeGroup);

    this->ui->questionMenu->addAction(addQuestion);
    this->ui->questionMenu->addAction(editQuestion);
    this->ui->questionMenu->addAction(renameQuestion);
    this->ui->questionMenu->addSeparator();
    this->ui->questionMenu->addAction(removeQuestion);

    connect(addQuestGroup, SIGNAL(triggered()), this, SLOT(onAddQuestGroup()));
    connect(editQuestGroup, SIGNAL(triggered()), this, SLOT(onEditQuestGroup()));
    connect(addQuestion, SIGNAL(triggered()), this, SLOT(onAddQuestion()));
    connect(renameQuestion, SIGNAL(triggered()), this, SLOT(onRenameQuestion()));

    connect(removeGroup, SIGNAL(triggered()), this, SLOT(onRemoveGroup()));
    connect(removeQuestion, SIGNAL(triggered()), this, SLOT(onRemoveQuestion()));
    connect(editQuestion, SIGNAL(triggered()), this, SLOT(onChangeQuestion()));
    connect(removeFromGroup, SIGNAL(triggered()), this, SLOT(onRemoveFromGroup()));
    connect(moveUp, SIGNAL(triggered()), this, SLOT(onMoveUp()));
    connect(moveDown, SIGNAL(triggered()), this, SLOT(onMoveDown()));
    connect(questList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onQuestListItemSelect(QListWidgetItem*)));
    connect(questTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onChangeQuestTreeItem(QTreeWidgetItem*, int)));
}

void MainWindow::onRenameQuestion()
{
    QString oldName = this->questList->currentItem()->text();

    bool ok = false;
    QString text = QInputDialog::getText(this, "Переименовать вопрос...",
                                         "Введите новое имя для вопроса:", QLineEdit::Normal,
                                         "", &ok);
    if(ok && !text.isEmpty())
    {
        this->panel->renameQuestion(oldName, text);

        this->questList->currentItem()->setText(text);
        QList<QTreeWidgetItem*> list = this->questTree->findItems(oldName,
                                                                  Qt::MatchContains | Qt::MatchRecursive);

        for(int i = 0; i < list.count(); i++)
            list[i]->setText(0, list[i]->parent()->text(0) + ": " + text);
    }
    this->panel->updateTabBar();
}

void MainWindow::onEditQuestGroup()
{
    if(curItem->data(0, Qt::UserRole).toInt() == 0)
    {
        GroupEditor editor(this->panel, this);
        editor.setGroup(curItem->text(0));
        if(editor.exec() == QDialog::Accepted)
        {
            QuetGroup *group = this->panel->getTopic(curItem->text(0));
            QStringList list = editor.getQuestions();

            QString groupName = editor.getName();

            if(groupName.isEmpty())
            {
                QMessageBox::warning(this, "Предепреждение!",
                                     "Имя группы не может быть пустым!");
                return;
            }

            if(groupName != group->getName())
            {
                group->clear();
                group->setName(groupName);
                this->curItem->setText(0, groupName);

                while(this->curItem->child(0))
                    this->curItem->removeChild(this->curItem->child(0));

                for(int i = 0; i < list.count(); i++)
                {
                    group->addQuestion(list[i]);
                    QTreeWidgetItem *item = new QTreeWidgetItem;
                    item->setText(0, groupName + ": " + list[i]);
                    item->setIcon(0, QIcon(pixFile("interface\\icons\\question.png")));
                    item->setData(0, Qt::UserRole, item->text(0));
                    this->curItem->addChild(item);
                }
            }
        }
    }
}

void MainWindow::updateTestToolControls(bool state)
{
    testProp->setEnabled(state);
    addQuestGroup->setEnabled(state);
    editQuestGroup->setEnabled(state);
    addQuestion->setEnabled(state);
    showRight->setEnabled(state);
    removeGroup->setEnabled(state);
    removeQuestion->setEnabled(state);
    editQuestion->setEnabled(state);
    toolBtn->setEnabled(state);
    renameQuestion->setEnabled(state);

    removeBtn->setEnabled(state);
    removeFromGroup->setEnabled(state);
    showQuestTab->setEnabled(state);
    showEndButton->setEnabled(state);
    showNextButton->setEnabled(state);
}

void MainWindow::onShowTab(bool value)
{
    this->panel->setShowTab(value);
}

void MainWindow::onShowEndButton(bool value)
{
    this->panel->setShowEndButton(value);
}

void MainWindow::onShowNextButton(bool value)
{
    this->panel->setNextButton(value);

    if(!value)
        this->showQuestTab->setChecked(false);
    this->showQuestTab->setEnabled(value);
}

void MainWindow::onShowRight(bool value)
{
    this->panel->showRight(value);
}

void MainWindow::onTestProperty()
{
    FileSystem::getInst()->fsOpen(&this->testFile);
    TestProperties dlg(this->panel, this);
    dlg.setEditorParam(FileSystem::getInst()->fsGetFile("editor.xml", &this->testFile));
    dlg.setAnswerList(FileSystem::getInst()->fsGetFile("answerlist.txt", &this->testFile));
    dlg.setComment(FileSystem::getInst()->fsGetFile("Comment.html", &this->testFile));
    dlg.setReportTemplate(FileSystem::getInst()->fsGetFile("report_template.html", &this->testFile));
    dlg.setPassWords(this->panel->passwordOnOpen, this->panel->passwordOnStart, this->panel->passwordOnEnd);
    dlg.setWindowCaption(this->panel->windowCaption);
    dlg.setMixParams(this->panel->mixQuestions, this->panel->mixAnswers);
    dlg.setMaxTestsStarts(this->panel->maxStarts);
    dlg.setStartPeriod(this->panel->startPeriod);
    dlg.setStartLimiting(this->panel->startLimiting);
    dlg.setSubject(this->panel->subject);

    FileSystem::getInst()->fsClose(&this->testFile);

    if(dlg.exec() == QDialog::Accepted)
    {
        FileSystem::getInst()->fsOpen(&this->testFile);
        FileSystem::getInst()->fsDelete("query\\select_groups.sql", &this->testFile);
        FileSystem::getInst()->fsAddFile(dlg.getgroupsSql(), "query\\select_groups.sql", &this->testFile, 2);

        FileSystem::getInst()->fsDelete("editor.xml", &this->testFile);
        FileSystem::getInst()->fsAddFile(dlg.getEditorParam(), "editor.xml", &this->testFile, 2);

        FileSystem::getInst()->fsDelete("answerlist.txt", &this->testFile);
        FileSystem::getInst()->fsAddFile(dlg.getAnswerList(), "answerlist.txt", &this->testFile, 2);

        FileSystem::getInst()->fsDelete("Comment.html", &this->testFile);
        FileSystem::getInst()->fsAddFile(dlg.getComment(), "Comment.html", &this->testFile, 2);

        FileSystem::getInst()->fsDelete("report_template.html", &this->testFile);
        FileSystem::getInst()->fsAddFile(dlg.getReportTemplate(), "report_template.html", &this->testFile, 2);

        this->panel->updateAnswerList();

        this->panel->testScores = dlg.getScores();

        this->panel->setTimeSettings(dlg.isOnTime(), dlg.getTime());
        this->panel->setNavigationSettings(dlg.isShowTab(), dlg.isEndButton(), dlg.isNavigation());

        this->panel->passwordOnOpen = dlg.getOpenPass();
        this->panel->passwordOnStart = dlg.getStartPass();
        this->panel->passwordOnEnd = dlg.getEndPass();

        this->panel->mixQuestions = dlg.hasMixQuestions();
        this->panel->mixAnswers = dlg.hasMixAnswers();
        this->panel->maxStarts = dlg.getMaxTestStarts();
        this->panel->startPeriod = dlg.getStartPeriod();
        this->panel->startLimiting = dlg.hasStartLimiting();

        this->panel->windowCaption = dlg.getWindowCaption();

        this->panel->subject = dlg.getSubject();

        FileSystem::getInst()->fsClose(&this->testFile);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    onCloseTest();
}

void MainWindow::onAddQuestGroup()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Новая тема",
                                         "Введите название темы:", QLineEdit::Normal,
                                         "", &ok);
    if(ok && !text.isEmpty())
    {
        if(this->panel->hasTopic(text))
            QMessageBox::critical(this, "Ошибка!", "Тема '" + text +
                                  "' уже существует");
        else
        {
            this->panel->addTopic(text);
            QTreeWidgetItem *subItem = new QTreeWidgetItem;
            subItem->setText(0, text);
            subItem->setData(0, Qt::UserRole, 0);
            subItem->setIcon(0, QIcon(pixFile("interface\\icons\\group.png")));

            this->panel->getTopic(text)->setTreeItem(subItem);

            this->questTree->topLevelItem(0)->addChild(subItem);
        }
    }
}

void MainWindow::onAddQuestion()
{
    QuestEditor editor(this->panel, this);
    //editor.setQuestTypes(this->panel->getTestTypes());

    if(editor.exec() == QDialog::Accepted)
    {
        editor.saveResources();

        Question *quest = editor.getQuestion();
        this->panel->addQuestion(quest);

        QStringList qrList = editor.getGroups();

        for(int i = 0; i < qrList.count(); i++)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem;
            item->setText(0, qrList[i] + ": " + quest->getName());
            item->setData(0, Qt::UserRole, item->text(0));
            item->setIcon(0, QIcon(pixFile("interface\\icons\\question.png")));

            this->panel->getTopic(qrList[i])->getTreeItem()->addChild(item);
            this->panel->addQuestToTopic(qrList[i], quest->getName());
        }
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(quest->getName());
        item->setIcon(QIcon(pixFile("interface\\icons\\question.png")));
        this->questList->addItem(item);

        this->panel->updateTabBar();
    }
}

void MainWindow::onChangeQuestTreeItem(QTreeWidgetItem *item, int column)
{
    bool ok;
    int type = item->data(0, Qt::UserRole).toInt(&ok);

    this->curItem = item;
    if(ok && type == 0)
    {
        removeGroup->setEnabled(true);
        editQuestGroup->setEnabled(true);
        removeFromGroup->setEnabled(false);
        editQuestion->setEnabled(false);
        moveUp->setEnabled(false);
        moveDown->setEnabled(false);
    }
    else if(!ok)
    {
        removeGroup->setEnabled(false);
        editQuestGroup->setEnabled(false);
        removeQuestion->setEnabled(true);
        removeFromGroup->setEnabled(true);
        moveUp->setEnabled(true);
        editQuestion->setEnabled(true);
        moveDown->setEnabled(true);


        QString text = item->text(0);
        text = text.remove(' ');
        int index = text.indexOf(':');
        text = text.remove(0, index + 1);
        this->panel->showQuestion(text);

        QList<QListWidgetItem*> list = this->questList->findItems(text, Qt::MatchContains);
        if(list.count())
            this->questList->setCurrentItem(list[0]);
    }
}

void MainWindow::onQuestListItemSelect(QListWidgetItem *item)
{
    this->panel->showQuestion(item->text());
    QList<QTreeWidgetItem*> list = this->questTree->findItems(item->text(), Qt::MatchContains | Qt::MatchRecursive);
    if(list.count() > 0)
        this->questTree->setCurrentItem(list[0]);
}

void MainWindow::onRemoveFromGroup()
{
    this->panel->removeFromTopic(this->curItem->parent()->text(0), this->curItem->text(0));
    delete this->curItem;
}

void MainWindow::onRemoveGroup()
{
    QString group = this->questTree->currentItem()->text(0);
    if(QMessageBox::question(this, "Удаление темы.", "Удалить тему '" + group
                          +"'?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {

        this->panel->removeTopic(group);
        return;
    }
}

void MainWindow::onRemoveQuestion()
{
    QString questName = this->questList->currentItem()->text();

    //Удалить вопрос из групп
    for(int i = 0; i < this->panel->topicsCount(); i++)
    {
        if(this->panel->getTopic(i)->hasQuestion(questName))
            this->panel->getTopic(i)->removeQuestion(questName);
    }

    this->panel->removeQuestion(questName);

    this->updateQuestTree();
    this->updateQuestList();
}

void MainWindow::onChangeQuestion()
{
    QuestEditor editor(this->panel, this);
    QString quest = this->getQuestName(this->questList->currentItem()->text());

    editor.setQuestion(this->panel->getQuestion(quest));
    editor.setGroups(this->panel->getQuestGroups(quest));
   // editor.setQuestTypes(this->panel->getTestTypes());

    if(editor.exec() == QDialog::Accepted)
    {
        Question *quest = editor.getQuestion();

        this->panel->updateQuestion(this->questList->currentItem()->text(),
                                    quest->getName(), quest);

        //Обновление групп
        for(int i = 0; i < this->panel->topicsCount(); i++)
            if(this->panel->getTopic(i)->hasQuestion(this->questList->currentItem()->text()))
                this->panel->getTopic(i)->removeQuestion(this->questList->currentItem()->text());;
        //Обновление структуры
        QList<QTreeWidgetItem*> list = this->questTree->findItems(this->questList->currentItem()->text(),
                                                                  Qt::MatchContains | Qt::MatchRecursive);

        for(int i = 0; i < list.count(); i++)
            delete list[i];

        QStringList groups = editor.getGroups();
        for(int i = 0; i < groups.count(); i++)
        {
            list = this->questTree->findItems(groups[i], Qt::MatchContains | Qt::MatchRecursive);
            QTreeWidgetItem *item = new QTreeWidgetItem;
            item->setText(0, groups[i] + ": " + quest->getName());
            qDebug() << item->text(0);
            item->setData(0, Qt::UserRole, item->text(0));
            item->setIcon(0, QIcon(pixFile("interface\\icons\\question.png")));

            this->panel->getTopic(groups[i])->addQuestion(quest->getName());

            list[0]->addChild(item);
        }

        this->questList->currentItem()->setText(quest->getName());
        this->questTree->sortByColumn(0, Qt::AscendingOrder);
    }
}

void MainWindow::onPrint()
{
    QWizard wizard;
    QWizardPage page1(&wizard);
    QLineEdit edit(&page1);

    wizard.addPage(&page1);
    wizard.exec();
    /*QPrinter printer;
    QPrintDialog printDialog(&printer, this);
    if(printDialog.exec() == QDialog::Accepted)
    {
    }*/
}

void MainWindow::onMoveUp()
{
    QTreeWidgetItem *group = this->curItem->parent();
    int index = group->indexOfChild(this->curItem);

    if(index)
    {
        QuetGroup *gr = this->panel->getTopic(group->text(0));
        group->removeChild(this->curItem);
        group->insertChild(index - 1, this->curItem);
        this->questTree->setCurrentItem(this->curItem);

        for(int i = 0; i < group->childCount(); i++)
            gr->setIndex(this->getQuestName(group->child(i)->text(0)), i);
    }
}

QString MainWindow::getQuestName(QString value)
{
    QString name = value;
    name = name.remove(' ');
    int index2 = name.indexOf(':');
    name = name.remove(0, index2 + 1);

    return name;
}

void MainWindow::onMoveDown()
{
    QTreeWidgetItem *group = this->curItem->parent();
    int index = group->indexOfChild(this->curItem);

    if(index < group->childCount())
    {
        QuetGroup *gr = this->panel->getTopic(group->text(0));
        group->removeChild(this->curItem);
        group->insertChild(index + 1, this->curItem);
        this->questTree->setCurrentItem(this->curItem);

        for(int i = 0; i < group->childCount(); i++)
            gr->setIndex(this->getQuestName(group->child(i)->text(0)), i);
    }
}
