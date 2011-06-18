#include "questpanel.h"
#include "ui_questpanel.h"
#include <mainwindow.h>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QResizeEvent>
#include <qdom.h>
#include <qurl.h>
#include <QPlastiqueStyle>
#include <qpainter.h>
#include <QCryptographicHash>
#include <QTemporaryFile>
#include <qinputdialog.h>
#include <QTextCodec>
#include <QTextDecoder>
#include <QTextDocumentFragment>
#include <QTextStream>
#include <QtSql>

QuestPanel::QuestPanel(QString file, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuestPanel),
    state(0),
    first(true),
    firstQuestion(true),
    nextQuestion(""),
    questWidget(NULL),
    tabBar(NULL),
    showTab(true),
    navigation(true),
    sec(59),
    windowCaption("Examenator"),
    passwordOnOpen(""),
    passwordOnStart(""),
    passwordOnEnd(""),
    tabState(false),
    mixQuestions(false),
    mixAnswers(false),
    maxStarts(0),
    startPeriod(1),
    startLimiting(true),
    scoreType(0),
    subject(-1),
    student(-1),
    unsatisfactory(false)
#ifdef REDACTOR
    ,counter(0),
    redactorShowTab(true),
    redactorTimer(true),
    redactorNavigation(true),
    redactorEndButton(false),
    redactorShowRight(false)
#else
    ,curQuestion(0)
#endif
{
    ui->setupUi(this);
    questFile.archFName = file;
    this->ui->prevBtn->setGeometry(0, -23, 75, 25);
    this->ui->lcdNumber->setVisible(false);

    delete this->ui->infoBrowser;
    this->ui->infoBrowser = new TextBrowser(&this->questFile, this);
    this->ui->infoBrowser->setStyleSheet("border: 1px solid gray;"
                                         "border-radius: 15px;"
                                         "border-width: 2px;"
                                         "background-color: gainsboro;"
                                         "background-image: url(:/images/back.png);"
                                         "background-position: top right;"
                                         "background-repeat: no-repeat;");

    this->ui->infoBrowser->setVisible(false);

    //Настройки по умолчанию компонентов выбора студента
    grLabelDef = this->ui->groupLabel->geometry();
    grBoxDef = this->ui->groupsBox->geometry();
    stLabelDef = this->ui->studentLabel->geometry();
    stBoxDef = this->ui->studentsBox->geometry();
    startBtnDef = this->ui->startButton->geometry();

    //Загрузка модулей
    QDir pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("TestTypes");
    foreach(QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();

        if(plugin)
        {
            plugins.push_back(qobject_cast<TBPluginInterface*>(plugin));
        }
    }

    //Подключить события
    connect(this->ui->nextBtn, SIGNAL(clicked()), this, SLOT(nextEvent()));
    connect(this->ui->prevBtn, SIGNAL(clicked()), this, SLOT(prevEvent()));
    connect(this->ui->groupsBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onSelectGroup(QString)));
    connect(this->ui->startButton, SIGNAL(clicked()), this, SLOT(hideStartScreen()));
    connect(this->ui->infoButton, SIGNAL(clicked()), this, SLOT(onInfoButton()));

    this->setAutoFillBackground(false);
}



void QuestPanel::addResource(QString name, QByteArray value)
{
    QuestPanel::resources.push_back(name);

    FileSystem::getInst()->fsOpen(&questFile);
    FileSystem::getInst()->fsAddFile(value, name, &questFile, 2);
    FileSystem::getInst()->fsClose(&questFile);

}

bool QuestPanel::hasResource(QString name)
{
    for(int i = 0; i < QuestPanel::resources.count(); i++)
        if(QuestPanel::resources[i] == name)
            return true;
    return false;
}

QPixmap QuestPanel::getPixResource(QString name)
{
    QPixmap map;
    FileSystem::getInst()->fsOpen(&questFile);
    map.loadFromData(FileSystem::getInst()->fsGetFile(name, &questFile));
    FileSystem::getInst()->fsClose(&questFile);

    return map;
}

QuestPanel::~QuestPanel()
{
    delete ui;
}

void QuestPanel::setDefGeometryStart()
{
    this->ui->groupLabel->setGeometry(grLabelDef);
    this->ui->studentLabel->setGeometry(stLabelDef);
    this->ui->groupsBox->setGeometry(grBoxDef);
    this->ui->studentsBox->setGeometry(stBoxDef);
    this->ui->startButton->setGeometry(startBtnDef);
}

void QuestPanel::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QuestPanel::nextEvent()
{
#ifdef REDACTOR
    int id = this->tabBar->currentIndex();
    QStringList list = this->getQuestNames();
    if(this->questions.count() > 1)
#else
    if(this->questList.count())
#endif
    {
        this->ui->prevBtn->setEnabled(true);
#ifdef REDACTOR
        this->nextQuestion = list[id];
#else
        this->nextQuestion = questList[this->curQuestion];
#endif
        //this->showQuestion(nextQuestion);

        //Показать следующий вопрос
#ifdef REDACTOR
        if(id < this->questions.count() + 1)//if(id < this->questions.count() + 1)
            id ++;

        this->tabBar->setCurrentIndex(id);
#else
        if(this->curQuestion < this->questList.count() - 1)
        {
            this->curQuestion ++;
            this->tabBar->setCurrentIndex(this->curQuestion);
            this->showQuestion(nextQuestion);
        }
        else
            //if(this->curQuestion == this->questList.count() - 1)
        {
            //Последний вопрос
            this->onEndButton();
        }
#endif
    }

    //Послать сигнал
    emit nextButtonClick();
}

void QuestPanel::prevEvent()
{
#ifdef REDACTOR
    int id = this->tabBar->currentIndex();
    if(this->questions.count() > 1)
#else
    if(this->questList.count())
#endif
    {
#ifdef REDACTOR
        this->nextQuestion = this->getQuestNames()[id];
#else
        this->nextQuestion = questList[this->curQuestion];
#endif
        this->showQuestion(nextQuestion);

#ifdef REDACTOR
        if(id > 0)
            id --;
        else
            this->ui->prevBtn->setEnabled(false);
        this->tabBar->setCurrentIndex(id);
#else
        if(this->curQuestion > 0)
            this->curQuestion --;
        else
            this->ui->prevBtn->setEnabled(false);
        this->tabBar->setCurrentIndex(this->curQuestion);
#endif
    }
    //Послать сигнал
    emit prevButtonClick();
}

void QuestPanel::resizeEvent(QResizeEvent *event)
{
    QRect region = this->geometry();
    this->ui->infoButton->setGeometry(region.width() - 20, region.height() - 20, 16, 16);
    if(state == SELECTESER)
    {
        if(!first)
        {
            QRect comboRect = this->ui->studentsBox->geometry();
            QRect grLebel = this->ui->groupLabel->geometry();
            QRect stLebel = this->ui->studentLabel->geometry();
            QRect grCombo = this->ui->groupsBox->geometry();
            QRect stCombo = this->ui->studentsBox->geometry();
            QRect startBtn = this->ui->startButton->geometry();

            QPropertyAnimation *label1 = new QPropertyAnimation(this->ui->groupLabel, "pos");
            QPropertyAnimation *label2 = new QPropertyAnimation(this->ui->studentLabel, "pos");

            QPropertyAnimation *combo1 = new QPropertyAnimation(this->ui->groupsBox, "pos");
            QPropertyAnimation *combo2 = new QPropertyAnimation(this->ui->studentsBox, "pos");

            QPropertyAnimation *button = new QPropertyAnimation(this->ui->startButton, "pos");

            /*
              Анимация надписи "группа"
              */
            label1->setStartValue(QPoint(grLebel.left(), grLebel.top()));
            label1->setEndValue(QPoint((region.width() - 220) / 2,
                                       (region.height() - comboRect.height()) / 2
                                       - 4 * comboRect.height()- 20));
            //Тип кривой ослабления
            label1->setEasingCurve(QEasingCurve::OutQuint);
           // label1->start();

            /*
              Анимация списка групп
              */
            combo1->setStartValue(QPoint(grCombo.left(), grCombo.top()));
            combo1->setEndValue(QPoint((region.width() - 220) / 2,
                                       (region.height() - comboRect.height()) / 2
                                       - 3 * comboRect.height()- 25));
            //Тип кривой ослабления
            combo1->setEasingCurve(QEasingCurve::OutQuint);
            //combo1->start();

            /*
              Анимация надписи "Студент"
              */
            label2->setStartValue(QPoint(stLebel.left(), stLebel.top()));
            label2->setEndValue(QPoint((region.width() - 220) / 2,
                                       (region.height() - comboRect.height()) / 2
                                       - 2 * comboRect.height() - 23));
            //Тип кривой ослабления
            label2->setEasingCurve(QEasingCurve::OutQuint);
            //label2->start();

            /*
              Анимация списка студентов
              */
            combo2->setStartValue(QPoint(stCombo.left(), stCombo.top()));
            combo2->setEndValue(QPoint((region.width() - 220) / 2,
                                       (region.height() - comboRect.height()) / 2
                                       - 1 * comboRect.height()- 27));
            //Тип кривой ослабления
            combo2->setEasingCurve(QEasingCurve::OutQuint);
            //combo2->start();

            /*
              Анимация кнопки старт
              */
            button->setStartValue(QPoint(startBtn.left(), startBtn.top()));
            button->setEndValue(QPoint((region.width() - 220) / 2,
                                       (region.height() - comboRect.height()) / 2
                                       - comboRect.height() + 10));

            QSequentialAnimationGroup *aniGroup = new QSequentialAnimationGroup;
            aniGroup->addAnimation(label1);
            aniGroup->addAnimation(label2);
            aniGroup->addAnimation(combo1);
            aniGroup->addAnimation(combo2);
            aniGroup->addAnimation(button);
            aniGroup->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
    else if(state == SHOWQUESTION)
    {
        //Размеры окна и кнопок "Далее", "Назад" и "Завершить"

        QRect btn1Rect = this->ui->nextBtn->geometry();
        QRect btn2Rect = this->ui->prevBtn->geometry();
        QRect btn3Rect = this->ui->endBtn->geometry();

        /*
          Анимация кнопки "Далее"
        */
        QPropertyAnimation *button1 = new QPropertyAnimation(this->ui->nextBtn, "pos");
        //Скорость воспроизведения
        button1->setDuration(1000);
        //Начальное значение
        button1->setStartValue(QPoint(btn1Rect.x(), btn1Rect.y()));
        //Конечное значение
        button1->setEndValue(QPoint(region.width() - btn1Rect.width() - 10,
                                    region.height() - btn1Rect.height() - 10));
        //Тип кривой ослабления
        button1->setEasingCurve(QEasingCurve::OutQuint);//OutBounce

        /*
          Анимация кнопки "Назад"
        */
        QPropertyAnimation *button2 = new QPropertyAnimation(this->ui->prevBtn, "pos");
        //Скорость воспроизведения
        button2->setDuration(1000);
        //Начальное значение
        button2->setStartValue(QPoint(btn2Rect.x(), btn2Rect.y()));
        //Конечное значение
        button2->setEndValue(QPoint(region.width() - btn1Rect.width() - btn2Rect.width() - 15,
                                    region.height() - btn2Rect.height() - 10));
        //Тип кривой ослабления
        button2->setEasingCurve(QEasingCurve::OutQuint);//OutBounce

        /*
          Анимация кнопки "Завершить"
        */
        QPropertyAnimation *button3 = new QPropertyAnimation(this->ui->endBtn, "pos");
        //Скорость воспроизведения
        button3->setDuration(1000);
        //Начальное значение
        button3->setStartValue(QPoint(btn3Rect.x(), btn3Rect.y()));
        //Конечное значение
        button3->setEndValue(QPoint(region.width() - btn1Rect.width() - btn2Rect.width() - 20 - btn3Rect.width(),
                                    region.height() - btn2Rect.height() - 10));
        //Тип кривой ослабления
        button3->setEasingCurve(QEasingCurve::OutQuint);//OutBounce

        int tabHeight = 0;

#ifdef REDACTOR
            if(redactorShowTab)
#else
            if(showTab)
#endif
                tabHeight = this->tabBar->height();


        this->tabBar->setMaximumWidth(this->width() - 20);
        QPropertyAnimation *panelAnim = new QPropertyAnimation(this->ui->questionArea, "geometry");
        panelAnim->setStartValue(this->ui->questionArea->geometry());
        panelAnim->setEndValue(QRect(10, 10 + tabHeight, this->width() - 20,
                                     this->height() - 30 - this->ui->nextBtn->height() - tabHeight));


        QPropertyAnimation *timerAnim = new QPropertyAnimation(this->ui->lcdNumber, "pos");
        timerAnim->setStartValue(this->ui->lcdNumber->pos());
        timerAnim->setEndValue(QPoint(10, this->height() - this->ui->lcdNumber->height() - 10));
        timerAnim->setEasingCurve(QEasingCurve::OutQuint);


        QParallelAnimationGroup *aniGroup = new QParallelAnimationGroup;
        aniGroup->addAnimation(button1);
        aniGroup->addAnimation(button2);
        aniGroup->addAnimation(button3);
        aniGroup->addAnimation(panelAnim);
        aniGroup->addAnimation(timerAnim);

        connect(aniGroup, SIGNAL(finished()), this, SLOT(onEndResizeAnimation()));
        aniGroup->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else if(state == REPORTCREEN)
    {

    }
}

void QuestPanel::onEndResizeAnimation()
{
    if(this->questWidget)
    {
        int tabHeight = 0;
#ifdef REDACTOR
        if(redactorShowTab)
#else
        if(showTab)
#endif
            tabHeight = this->tabBar->height();

        this->questWidget->setGeometry(QRect(0, 0, this->width() - 60,
                                             this->height() - 50 - this->ui->nextBtn->height() -
                                             tabHeight));

        this->questWidget->calculateSize();

        this->tabBar->setGeometry(10, 10, this->width() - 20, this->tabBar->height());
        this->tabBar->update();
    }
}

void QuestPanel::showStartScreen()
{
    if(this->state == this->SELECTESER)
    {
        QSqlQuery query;
        FileSystem::getInst()->fsOpen(&questFile);
        query.prepare(FileSystem::getInst()->fsGetFile("query\\select_groups.sql", &questFile));
        FileSystem::getInst()->fsClose(&questFile);

        if(query.exec())
        {
            this->ui->groupsBox->clear();
            while(query.next())
                this->ui->groupsBox->addItem(query.record().value(0).toString());
        }
        this->state = 0;
        this->first = true;
        this->ui->nextBtn->setVisible(false);
        this->ui->prevBtn->setVisible(false);

        setDefGeometryStart();

        this->setProperty("isQuest", false);
        this->update();

        QRect clientRect = this->geometry();
        QRect comboRect = this->ui->studentsBox->geometry();

        QPropertyAnimation *groupsLabelAnimation = new QPropertyAnimation(this->ui->groupLabel, "pos");
        QPropertyAnimation *studentLabelAnimation = new QPropertyAnimation(this->ui->studentLabel, "pos");

        QPropertyAnimation *groupsBoxAnimation = new QPropertyAnimation(this->ui->groupsBox, "pos");
        QPropertyAnimation *groupsBoxAnimation2 = new QPropertyAnimation(this->ui->groupsBox, "geometry");

        QPropertyAnimation *studentsBoxAnimation = new QPropertyAnimation(this->ui->studentsBox, "pos");
        QPropertyAnimation *studentsBoxAnimation2 = new QPropertyAnimation(this->ui->studentsBox, "geometry");

        QPropertyAnimation *startBtnAnimation = new QPropertyAnimation(this->ui->startButton, "pos");
        QPropertyAnimation *startBtnAnimation2 = new QPropertyAnimation(this->ui->startButton, "geometry");

        /*
          Анимация надписи "группа"
          */
        groupsLabelAnimation->setStartValue(QPoint((clientRect.width() - 220) / 2,
                                         -comboRect.height()));
        groupsLabelAnimation->setEndValue(QPoint((clientRect.width() - 220) / 2,
                                       (clientRect.height() - comboRect.height()) / 2
                                       - 4 * comboRect.height()- 20));
        groupsLabelAnimation->setEasingCurve(QEasingCurve::OutBounce);

        /*
          Анимация надписи "Студент"
          */
        studentLabelAnimation->setStartValue(QPoint((clientRect.width() - 220) / 2,
                                         -comboRect.height()));
        studentLabelAnimation->setEndValue(QPoint((clientRect.width() - 220) / 2,
                                       (clientRect.height() - comboRect.height()) / 2
                                       - 2 * comboRect.height()- 20));
        studentLabelAnimation->setEasingCurve(QEasingCurve::OutBounce);

        /*
          Анимация списка групп
          */
        groupsBoxAnimation->setStartValue(QPoint((clientRect.width() - 220) / 2,
                                         -comboRect.height()));
        groupsBoxAnimation->setEndValue(QPoint((clientRect.width() - 220) / 2,
                                       (clientRect.height() - comboRect.height()) / 2
                                       - 3 * comboRect.height()- 25));
        groupsBoxAnimation->setEasingCurve(QEasingCurve::OutBounce);
        //---------------------------------------------------------
        QRect tempRect = QRect((clientRect.width() - 220) / 2, (clientRect.height() - comboRect.height()) / 2
                      - 3 * comboRect.height() - 25, 21, 22);
        groupsBoxAnimation2->setStartValue(tempRect);
        groupsBoxAnimation2->setEasingCurve(QEasingCurve::OutBounce);
        tempRect.setWidth(220);
        groupsBoxAnimation2->setEndValue(tempRect);
        /*
          Анимация списка студентов
          */
        studentsBoxAnimation->setStartValue(QPoint((clientRect.width() - 220) / 2,
                                         -comboRect.height()));
        studentsBoxAnimation->setEndValue(QPoint((clientRect.width() - 220) / 2,
                                       (clientRect.height() - comboRect.height()) / 2
                                       - 1 * comboRect.height()- 25));
        studentsBoxAnimation->setEasingCurve(QEasingCurve::OutBounce);
        //---------------------------------------------------------
        tempRect = QRect((clientRect.width() - 220) / 2, (clientRect.height() - comboRect.height()) / 2
                      - 1 * comboRect.height() - 25, 21, 22);
        studentsBoxAnimation2->setStartValue(tempRect);
        studentsBoxAnimation2->setEasingCurve(QEasingCurve::OutBounce);
        tempRect.setWidth(220);
        studentsBoxAnimation2->setEndValue(tempRect);

        /*
          Анимация кнопки старт
          */
        startBtnAnimation->setStartValue(QPoint((clientRect.width() - this->startBtnDef.width()) / 2,
                                         -this->startBtnDef.height()));
        startBtnAnimation->setEndValue(QPoint((clientRect.width() - this->startBtnDef.width()) / 2,
                                              (clientRect.height() - comboRect.height()) / 2
                                              - comboRect.height() + 10));
        startBtnAnimation->setEasingCurve(QEasingCurve::OutBounce);
        //---------------------------------------------------------
        tempRect = QRect((clientRect.width() - this->startBtnDef.width()) / 2,
                         (clientRect.height() - comboRect.height()) / 2
                         - comboRect.height() + 10, this->ui->startButton->width(),
                         this->ui->startButton->height());
        startBtnAnimation2->setStartValue(tempRect);

        tempRect.setLeft((clientRect.width() - 220) / 2);
        tempRect.setWidth(220);
        startBtnAnimation2->setEndValue(tempRect);
        startBtnAnimation2->setEasingCurve(QEasingCurve::OutBounce);


        groupsLabelAnimation->setDuration(150);
        studentLabelAnimation->setDuration(150);
        groupsBoxAnimation->setDuration(150);
        groupsBoxAnimation2->setDuration(500);
        startBtnAnimation2->setDuration(500);
        studentsBoxAnimation2->setDuration(500);
        groupsBoxAnimation2->setDuration(500);

        startScreen = new QSequentialAnimationGroup;
        startScreen->addAnimation(groupsLabelAnimation);
        startScreen->addAnimation(groupsBoxAnimation);
        startScreen->addAnimation(groupsBoxAnimation2);
        startScreen->addAnimation(studentLabelAnimation);
        startScreen->addAnimation(studentsBoxAnimation);
        startScreen->addAnimation(studentsBoxAnimation2);
        startScreen->addAnimation(startBtnAnimation);
        startScreen->addAnimation(startBtnAnimation2);
        startScreen->start(QAbstractAnimation::DeleteWhenStopped);

        connect(startScreen, SIGNAL(finished()), this, SLOT(onEndFirstScreenAnimation()));
    }
    else
    {
        this->state = this->SELECTESER;
        this->hideQuestion();
    }
}

void QuestPanel::showEvent(QShowEvent *event)
{

}

QuestPanel::OpenErr QuestPanel::updateTopics()
{
    QuestPanel::OpenErr result = QuestPanel::SUCCEEDED;
    if(FileSystem::getInst()->fsOpen(&questFile))
    {
#ifndef DEBUG_MSG
        qDebug() << "Разбор теста";
#endif
        QByteArray content = FileSystem::getInst()->fsGetFile("questions.xml", &questFile);

        QDomDocument doc;
        QString err;
        if(doc.setContent(content, false, &err))
        {
            QDomElement docElement = doc.documentElement();
            this->subject = docElement.attribute("subject", "-1").toInt();
#ifndef DEBUG_MSG
            qDebug() << "Subject: " << this->subject;
#endif
            //Разбор вопросов
            QDomElement questions = docElement.firstChildElement("questions");
            if(questions.isNull())
            {
                this->lastErr = "Не найдена главная ветвь в конфигурации теста!";
                return FAIL;
            }

            int sh = 0;
            for(QDomNode n = questions.firstChild(); !n.isNull(); n = n.nextSibling())
            {
                sh ++;
                QDomElement elem = n.toElement();

                QString qtype = elem.attribute("type");

                QString name = elem.attribute("name", "");

                if(name != "")
                {
                    QString src = elem.attribute("src");

                    Question *quest = NULL;

                    if(qtype == "0")
                    {
                        quest = new Question(elem.attribute("name"), src,
                                             qtype, elem.attribute("rcount").toInt());
                        this->questions.insert(quest->getName(), quest);

                        //Чтение ответов
                        for(QDomNode ns = elem.firstChild(); !ns.isNull(); ns = ns.nextSibling())
                        {
                            QDomElement elem = ns.toElement();
                            if(elem.tagName() == "answer")
                                quest->addAnswer(new Answer(elem.attribute("name"), elem.attribute("src"),
                                                            elem.attribute("right").toInt()));
                        }
                    }
                    else if(qtype == "1")
                    {
                        QString right = elem.attribute("right", "NULL");
                        quest = new Question(name, src, right, "1");
                        this->questions.insert(name, quest);
                    }
                    else
                    {
                        TBPluginInterface *plug = this->getPlugin(qtype);

                        if(plug)
                        {
                            QDomNode node = elem.firstChild();
                            quest = plug->loadQuestion(node.toElement(), name);

                            if(quest)
                            {
                                this->questions.insert(name, quest);
                            }
                        }
                        else
                        {
                            quest = new Question();
                            QString str;
                            QTextStream stream(&str, QIODevice::WriteOnly);
                            elem.save(stream, 3);
                            quest->setSettings(str.toLocal8Bit());
                            qDebug() << quest->settings();
                            this->questions.insert(name, quest);
                        }
                    }
                }
                else
                {
                    this->warnings += QString("Отсутствует имя вопроса. [Вопрос№%1]").arg(sh);
                    result = QuestPanel::WARNINGS;
                }
            }
            //Разбор списка тем
            QDomElement categores = docElement.firstChildElement("categories");

            qDebug() << categores.nodeValue();
            for(QDomNode n = categores.firstChild(); !n.isNull(); n = n.nextSibling())
            {
                QDomElement elem = n.toElement();
                QuetGroup *group = new QuetGroup(elem.attribute("name"));

                //Чтение вопросов
                for(QDomNode ns = elem.firstChild(); !ns.isNull(); ns = ns.nextSibling())
                {
                    QDomElement e = ns.toElement();
                    if(e.tagName() == "question")
                    {
                        QString questName = e.text();
                        QString index = e.attribute("index", "-1");
                        if(index != "-1")
                        {
                            if(this->questions.contains(questName))
                                group->addQuestion(questName, index.toInt());
                        }
                        else
                        {
                            if(this->questions.contains(questName))
                                group->addQuestion(questName);
                        }
                    }
                }
                group->sortQuestions();
                this->topics.push_back(group);
            }

            //Загрузка критериев оценки
            QDomElement scores = docElement.firstChildElement("scores");
            this->scoreType = scores.attribute("type", "0").toInt();
            for(QDomNode n = scores.firstChild(); !n.isNull(); n = n.nextSibling())
            {
                QDomElement e = n.toElement();
                PTSCORE buf = new SCORE;
                buf->text = e.text();
                buf->percent = e.attribute("percent", "0").toInt();
                buf->value = e.attribute("value", "0").toInt();

                this->testScores.push_back(buf);
            }

            //Загрузка параметров
            QDomElement settings = docElement.firstChildElement("settings");
#ifdef REDACTOR
            this->counter = settings.attribute("counter", 0).toInt();
#endif
            this->mixQuestions = (bool)settings.attribute("mixQuestions", 0).toInt();
            this->mixAnswers = (bool)settings.attribute("mixAnswers", 0).toInt();

            QDomElement startLimiting = settings.firstChildElement("startLimiting");
            this->startLimiting = (bool)startLimiting.attribute("on", "1").toInt();
            this->startPeriod = (int)startLimiting.attribute("period", "1").toInt();
            this->maxStarts = (int)startLimiting.attribute("maxStarts", "1").toInt();

#ifndef DEBUG_MSG
            qDebug() << "startLimiting: " << this->startLimiting
                    << "startPeriod: " << this->startPeriod
                    << "maxStarts: " << this->maxStarts;
#endif

            QDomElement timer = settings.firstChildElement("timer");
            QDomElement passwords = settings.firstChildElement("passwords");
            QDomElement navigation = settings.firstChildElement("navigation");

            QDomElement caption = settings.firstChildElement("caption");
            this->windowCaption = caption.text();

            this->onTime = timer.attribute("enabled", QString::number(0)).toInt();
            this->time = timer.text().toInt();
            this->questTime = this->time;

            this->navigation = navigation.attribute("enabled", QString::number(1)).toInt();
            this->showTab = navigation.attribute("showtab", QString::number(1)).toInt();
            this->endButton = navigation.attribute("endbutton", QString::number(0)).toInt();

            //Загрузка паролей
            this->passwordOnOpen = passwords.firstChildElement("onredact").text();
            this->passwordOnStart = passwords.firstChildElement("onstarttest").text();
            this->passwordOnEnd = passwords.firstChildElement("onendtest").text();

#ifndef DEBUG_MSG
            qDebug() << "Passwords: " << passwordOnOpen << passwordOnStart << passwordOnEnd;
#endif
            FileSystem::getInst()->fsClose(&questFile);
            this->updateAnswerList();
            this->ui->infoBrowser->setSource(QUrl("Comment.html"));
        }
        else
        {
            //QMessageBox::critical(this, "Ошибка разбора теста", err);
            FileSystem::getInst()->fsClose(&questFile);
            this->lastErr = err;
            return FAIL;
        }
    }
    else
    {
        QMessageBox::critical(this, "Ошибка!", "Не удалось загрузить конфигурацию теста!");
    }

#ifdef REDACTOR
    if(this->questions.count() > 0)
        this->nextQuestion = this->questions[this->getQuestNames()[0]]->getName();

    this->updateTabBar();
#endif

#ifndef DEBUG_MSG
    qDebug() << "Загрузка информации...";
#endif

    return result;
}

QString QuestPanel::getWindowCaption()
{
    return this->windowCaption;
}

QString QuestPanel::lastError()
{
    return this->lastErr;
}

void QuestPanel::updateAnswerList()
{
    FileSystem::getInst()->fsOpen(&questFile);
    this->answerList.clear();
    QByteArray list = FileSystem::getInst()->fsGetFile("answerlist.txt", &questFile);
    QTextStream stream(&list);
    stream.setCodec("Windows-1251");
    QString line;
    do
    {
        line = stream.readLine();
        if(!line.isNull())
            this->answerList.push_back(line);
    }while(!line.isNull());
    FileSystem::getInst()->fsClose(&questFile);
}

QStringList QuestPanel::getTopicNames()
{
    QStringList list;

    for(int i = 0; i < this->topics.count(); i++)
        list.push_back(this->topics[i]->getName());

    return list;
}

int QuestPanel::topicsCount()
{
    return this->topics.count();
}

void QuestPanel::onEndFirstScreenAnimation()
{
    first = false;
    this->startScreen = NULL;
}

void QuestPanel::onSelectGroup(QString item)
{
    QSqlQuery query;
    FileSystem::getInst()->fsOpen(&questFile);
    query.prepare(FileSystem::getInst()->fsGetFile("query\\select_students.sql", &questFile));
    FileSystem::getInst()->fsClose(&questFile);
    query.bindValue(":param", item);

    QStringList list;
    list.push_back(query.record().value(1).toString());
    if(query.exec())
    {
        this->ui->studentsBox->clear();
        while(query.next())
        {
            list.push_back(query.record().value(1).toString());
            this->ui->studentsBox->addItem(query.record().value(1).toString(),
                                           query.record().value(0).toInt());
        }
    }
    QCompleter *completer = new QCompleter(list, this);
    this->ui->studentsBox->setCompleter(completer);
}

#ifdef REDACTOR
QStringList QuestPanel::getTestTypes()
{
    QStringList list;
    for(int i = 0; i < this->plugins.count(); i++)
    {
        if(this->plugins[i]->isQuestionPlugin())
            list.push_back(this->plugins[i]->questType());
    }

    return list;
}

QDomElement QuestPanel::getSaveConfig(QString value, Question *quest, QDomDocument document)
{
    for(int i = 0; i < this->plugins.count(); i++)
    {
        if(this->plugins[i]->isQuestionPlugin() && this->plugins[i]->questType() == value)
        {
            return this->plugins[i]->writeQuestion(quest, document);
        }
    }

    return QDomElement();
}

QuestEditorInterface *QuestPanel::getEditor(QString questType)
{
    for(int i = 0; i < this->plugins.count(); i++)
    {
        if(this->plugins[i]->isQuestionPlugin() && this->plugins[i]->questType() == questType)
            return this->plugins[i]->editor();
    }

    return NULL;
}

QByteArray QuestPanel::writeXmlConfig()
{
    QDomDocument doc;

    QDomNode xmlNode = doc.createProcessingInstruction("xml",
                                                       "version=\"1.0\" encoding=\"Windows-1251\"");
    doc.appendChild(xmlNode);
    QDomElement root = doc.createElement("test");
    root.setAttribute("subject", this->subject);
    doc.appendChild(root);

    //Запись вопросов
    QDomElement questions = doc.createElement("questions");
    QStringList questNames = this->questions.keys();
    for(int i = 0; i < this->questions.count(); i++)
    {
        QDomElement quest = doc.createElement("quest");

        Question *buf = this->questions[questNames[i]];

        quest.setAttribute("name", buf->getName());
        quest.setAttribute("src", buf->getSrc());
        quest.setAttribute("type", buf->getType());

        //Запись ответов
        if(buf->getType() == "0")
        {
            quest.setAttribute("rcount", buf->getAnsvRCount());
            for(int j = 0; j < buf->getAnsvCount(); j++)
            {
                QDomElement answer = doc.createElement("answer");
                Answer *ans = buf->getAnswer(j);
                answer.setAttribute("name", ans->getName());
                answer.setAttribute("src", ans->getSource());
                answer.setAttribute("right", ans->isRight());

                quest.appendChild(answer);
            }
        }
        else if(buf->getType() == "1")
        {
            quest.setAttribute("right", buf->getRightAnswer());
        }
        else if(buf->getType() == "unknown")
        {
            QDomDocument d;
            QString err;
            if(d.setContent(buf->settings(), &err))
            {
                QDomElement element = d.documentElement();
                questions.appendChild(element);
            }
            else
                qDebug() << err;
        }
        else
        {
            QDomElement plugElement = this->getSaveConfig(buf->getType(), buf, doc);

            if(!plugElement.isNull())
            {
                quest.appendChild(plugElement);
            }
        }
        questions.appendChild(quest);
    }
    root.appendChild(questions);

    //Запись списка групп
    QDomElement topics = doc.createElement("categories");
    for(int i = 0; i < this->topicsCount(); i++)
    {
        QDomElement t = doc.createElement("topic");
        t.setAttribute("name", this->topics[i]->getName());

        //Записать вопросы в группу
        for(int j = 0; j < this->topics[i]->questCount(); j++)
        {
            QuetGroup *group = this->topics[i];
            QDomText n = doc.createTextNode(group->getQuestName(j));
            QDomElement q = doc.createElement("question");
            q.setAttribute("index", group->getIndex(group->getQuestName(j)));
            q.appendChild(n);

            t.appendChild(q);
        }

        topics.appendChild(t);
    }
    root.appendChild(topics);

    //Запись ресурсов
    QDomElement resources = doc.createElement("resources");
    for(int i = 0; i < QuestPanel::resources.count(); i++)
    {
        QDomText n = doc.createTextNode(QuestPanel::resources[i]);
        QDomElement f = doc.createElement("file");
        f.appendChild(n);
        resources.appendChild(f);
    }
    root.appendChild(resources);

    //Запись критериев оценки
    QDomElement scores = doc.createElement("scores");
    scores.setAttribute("type", this->scoreType);
    for(int i = 0; i < this->testScores.count(); i++)
    {
        QDomElement value = doc.createElement("level");
        value.setAttribute("percent", this->testScores[i]->percent);
        value.setAttribute("value", this->testScores[i]->value);
        value.appendChild(doc.createTextNode(this->testScores[i]->text));

        scores.appendChild(value);
    }
    root.appendChild(scores);

    //Запись параметров
    QDomElement settings = doc.createElement("settings");
    settings.setAttribute("counter", this->counter);
    settings.setAttribute("mixQuestions", this->mixQuestions);
    settings.setAttribute("mixAnswers", this->mixAnswers);
    settings.setAttribute("maxStarts", this->maxStarts);

    //Ограничения запуска
    QDomElement startLimiting = doc.createElement("startLimiting");
    startLimiting.setAttribute("on", this->startLimiting);
    startLimiting.setAttribute("maxStarts", this->maxStarts);
    startLimiting.setAttribute("period", this->startPeriod);
    settings.appendChild(startLimiting);

    //На время
    QDomElement time = doc.createElement("timer");
    time.appendChild(doc.createTextNode(QString::number(this->time)));
    time.setAttribute("enabled", this->onTime);
    settings.appendChild(time);
    //Навигация
    QDomElement navigation = doc.createElement("navigation");
    navigation.setAttribute("enabled", this->navigation);
    navigation.setAttribute("showtab", this->showTab);
    navigation.setAttribute("endbutton", this->endButton);
    settings.appendChild(navigation);

    QDomElement caption = doc.createElement("caption");
    caption.appendChild(doc.createTextNode(this->windowCaption));
    settings.appendChild(caption);

    //Запись паролей
    QDomElement passwords = doc.createElement("passwords");
    QDomElement onredact = doc.createElement("onredact");
    onredact.appendChild(doc.createTextNode(this->passwordOnOpen));

    QDomElement onstarttest = doc.createElement("onstarttest");
    onstarttest.appendChild(doc.createTextNode(this->passwordOnStart));

    QDomElement onendtest = doc.createElement("onendtest");
    onendtest.appendChild(doc.createTextNode(this->passwordOnEnd));

    passwords.appendChild(onredact);
    passwords.appendChild(onstarttest);
    passwords.appendChild(onendtest);

    settings.appendChild(passwords);
    root.appendChild(settings);

    QByteArray toSave;
    QTextStream stream(&toSave);
    stream.setCodec("Windows-1251");
    doc.save(stream, 4);

#ifndef DEBUG_MSG
    qDebug() << toSave;
#endif

    return toSave;
}

void QuestPanel::setNavigationSettings(bool isShowTab, bool isEndButton, bool isNavigation)
{
    this->showTab = isShowTab;
    this->endButton = isEndButton;
    this->navigation = isNavigation;
}
#endif

void QuestPanel::updateTabBar()
{
    if(this->tabBar)
    {
        this->tabBar->disconnect(this, SLOT(questTabChanged(int)));
        delete this->tabBar;
        this->tabBar = NULL;
    }

    this->tabBar = new QTabBar(this);

#ifdef REDACTOR
    QStringList list = this->getQuestNames();
    for(int i = 0; i < list.count(); i++)
    {
        this->tabBar->addTab(list[i]);
        this->tabBar->setTabData(i, list[i]);
    }
#else
    for(int i = 0; i < questList.count(); i++)
    {
        this->tabBar->addTab(QString::number(i + 1));
        this->tabBar->setTabData(i, questList[i]);
    }
#endif
    this->tabBar->setProperty("pos", QPoint(10, 10));
    connect(this->tabBar, SIGNAL(currentChanged(int)), this, SLOT(questTabChanged(int)));

    this->tabBar->setVisible(false);
}

void QuestPanel::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void QuestPanel::addTopic(QString topicName)
{
    this->topics.push_back(new QuetGroup(topicName));
}

#ifdef REDACTOR
void QuestPanel::removeTopic(QString name)
{
    if(this->hasTopic(name))
    {
        delete this->getTopic(name)->getTreeItem();
        this->topics.removeOne(this->getTopic(name));
        return;
    }
}
#endif

bool QuestPanel::hasTopic(QString topic)
{
    for(int i = 0; i < this->topics.count(); i++)
        if(this->topics[i]->getName() == topic)
            return true;
    return false;
}

Question *QuestPanel::getQuestion(QString name)
{
    return this->questions[name];
}

Question *QuestPanel::getQuestion(int index)
{
    return this->getQuestion(this->questions.keys().at(index));
}

QStringList QuestPanel::getQuestNames()
{
    return this->questions.uniqueKeys();
}

void QuestPanel::addQuestion(Question *quest)
{
    this->questions.insert(quest->getName(), quest);
}

QuetGroup *QuestPanel::getTopic(QString name)
{
    for(int i = 0; i < topics.count(); i++)
        if(this->topics[i]->getName() == name)
            return this->topics[i];

    return NULL;
}

QuetGroup *QuestPanel::getTopic(int index)
{
    return this->topics[index];
}

void QuestPanel::addQuestToTopic(QString topicName, QString questName)
{
    if(this->hasTopic(topicName))
    {
        this->getTopic(topicName)->addQuestion(questName);
    }
}

QString QuestPanel::getFile(QString fname)
{
    QFile file(fname);
    QString str("");
    if(file.open(QFile::ReadOnly))
        str = file.readAll();
    file.close();
    return str;
}

void QuestPanel::hideStartScreen()
{
#ifndef DEBUG_MSG
    qDebug() << "Hide start screen";
#endif

    if(this->questions.count() > 0)
    {
#ifdef REDACTOR
        this->nextQuestion = this->questions[this->getQuestNames()[0]]->getName();
#else
        if(this->questList.count() > 0)
            this->nextQuestion = this->questions[this->questList[0]]->getName();
        else
        {
            QMessageBox::warning(this, "Ошибка!", "Список вопросов пуст. \nПроверьте параметры запуска!");
            return;
        }
#endif

#ifndef REDACTOR
        QSqlQuery query;

        group = this->ui->groupsBox->currentText();
        //Существует ли такой студент
        student_name = this->ui->studentsBox->lineEdit()->text();

        query.prepare(QuestPanel::getFile(":/query/has_student.sql"));
        query.bindValue(":param", student_name);

        if(query.exec())
        {
            query.next();
            if(query.record().value(0).toInt() == 0)
            {
                QMessageBox msg(this);
                msg.setWindowTitle("Ошибка!");
                msg.setText("Студента '<B>" + student_name + "</B>' нет среди учащихся!");
                msg.setInformativeText("Проверьте правильность введенного имени...");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.exec();
                return;
            }
        }
        else
        {
            QMessageBox msg(this);
            msg.setWindowTitle("Ошибка!");
            msg.setText("Не удалось проверить наличие студента <B>" + student_name + "</B> среди учащихся!");
            msg.setDetailedText(query.lastError().text());
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();

            return;
        }

        this->student = this->ui->studentsBox->itemData(this->ui->studentsBox->currentIndex(), Qt::UserRole).toInt();

        if(this->startLimiting)
        {
            query.prepare(QuestPanel::getFile(":/query/select_test_state.sql"));
            query.bindValue(":s_param", this->student);
            query.bindValue(":test", this->testIdenti);

            if(query.exec())
            {
                int count = 0;
                if(query.size())
                {
                    query.last();
                    count = query.record().value(0).toInt();
                }
                else
                {
#ifndef DEBUG_MSG
                    qDebug() << "Can't get query size";
#endif
                }

                //Если не исчерпаны попытки пройти тест и записать/обновить сведения
                if(count < this->maxStarts)
                {
                    if(!this->passwordOnStart.isEmpty())
                    {
                        bool ok;
                        QString pass = QInputDialog::getText(this, "Введите пароль...",
                                                             "Подтверждение преподавателя:", QLineEdit::Password,
                                                             "", &ok);
                        if(!ok || pass.isEmpty() || this->passwordOnStart != pass)
                            return;
                    }

                    query.prepare("call UpdateTestState(?, ?, ?);");

                    query.bindValue(0, student);
                    query.bindValue(1, this->testIdenti);
                    query.bindValue(2, this->startPeriod);

                    if(!query.exec())
                    {
                        QMessageBox msg(this);
                        msg.setWindowTitle("Ошибка!");
                        msg.setText("Не удалось обновить состояние! Обратитесь к преподавателю!");
                        msg.setInformativeText("Проверьте настройки сервера.");
                        msg.setDetailedText(query.lastError().text());
                        msg.setStandardButtons(QMessageBox::Ok);
                        msg.exec();
                        return;
                    }
                }
                else
                {
                    QMessageBox::information(this, "Ошибка!",
                                             "Вам больше нельзя проходить этот тест!");
                    return;
                }
            }
            else
            {
                QMessageBox::information(this, "qОшибка!",
                                         query.lastError().text());
                return;
            }
        }
        else
        {
            if(!this->passwordOnStart.isEmpty())
            {
                bool ok;
                QString pass = QInputDialog::getText(this, "Введите пароль...",
                                                     "Подтверждение преподавателя:", QLineEdit::Password,
                                                     "", &ok);

                if(!ok || pass.isEmpty() || this->passwordOnStart != pass)
                    return;
            }
        }

        if(this->unsatisfactory)
            this->addUnsatisfactoryQuestions();

        emit startTestButton();
#endif

        QRect comboRect = this->ui->studentsBox->geometry();
        QRect grLebel = this->ui->groupLabel->geometry();
        QRect stLebel = this->ui->studentLabel->geometry();
        QRect grCombo = this->ui->groupsBox->geometry();
        QRect stCombo = this->ui->studentsBox->geometry();
        QRect startBtn = this->ui->startButton->geometry();

        QPropertyAnimation *label1 = new QPropertyAnimation(this->ui->groupLabel, "pos");
        QPropertyAnimation *label2 = new QPropertyAnimation(this->ui->studentLabel, "pos");

        QPropertyAnimation *combo1 = new QPropertyAnimation(this->ui->groupsBox, "pos");
        QPropertyAnimation *combo2 = new QPropertyAnimation(this->ui->studentsBox, "pos");

        QPropertyAnimation *button = new QPropertyAnimation(this->ui->startButton, "pos");

        label1->setStartValue(QPoint(grLebel.left(), grLebel.top()));
        grLebel.setLeft(-comboRect.width());
        label1->setEndValue(QPoint(grLebel.left(), grLebel.top()));

        label2->setStartValue(QPoint(stLebel.left(), stLebel.top()));
        stLebel.setLeft(-comboRect.width());
        label2->setEndValue(QPoint(stLebel.left(), stLebel.top()));

        combo1->setStartValue(QPoint(grCombo.left(), grCombo.top()));
        grCombo.setLeft(-comboRect.width());
        combo1->setEndValue(QPoint(grCombo.left(), grCombo.top()));

        combo2->setStartValue(QPoint(stCombo.left(), stCombo.top()));
        stCombo.setLeft(-comboRect.width());
        combo2->setEndValue(QPoint(stCombo.left(), stCombo.top()));

        button->setStartValue(QPoint(startBtn.left(), startBtn.top()));
        startBtn.setLeft(-comboRect.width());
        button->setEndValue(QPoint(startBtn.left(), startBtn.top()));

        QParallelAnimationGroup *group = new QParallelAnimationGroup;
        group->addAnimation(label1);
        group->addAnimation(label2);
        group->addAnimation(combo1);
        group->addAnimation(combo2);
        group->addAnimation(button);

        this->ui->infoButton->setVisible(false);
        connect(group, SIGNAL(finished()), this, SLOT(onEndQuestAnimation()));

        group->start();

        this->ui->nextBtn->setVisible(true);
        this->ui->prevBtn->setVisible(true);

#ifndef REDACTOR
        if(this->onTime)
        {
#endif
            QTimer::singleShot(this->time * 60000, this, SLOT(onTimer()));
            updateIndicator = new QTimer;
            updateIndicator->setInterval(1000);

            connect(updateIndicator, SIGNAL(timeout()), this, SLOT(onUpdateIndicator()));
            updateIndicator->start();

            this->ui->lcdNumber->setVisible(true);
            this->questTime --;
            this->ui->lcdNumber->display(QString::number(this->questTime) + ":00");
#ifndef REDACTOR
        }
#endif

#ifndef DEBUG_MSG
        qDebug() << "StartScreen is hide.";
#endif
    }
}

#ifdef REDACTOR
void QuestPanel::removeQuestion(QString name)
{
    Question *quest = this->getQuestion(name);
    //Удаление данных из архива
    FileSystem::getInst()->fsOpen(&questFile);
    FileSystem::getInst()->fsDelete(quest->getSrc(), &questFile);

    for(int i = 0; i < quest->getAnsvCount(); i++)
        FileSystem::getInst()->fsDelete(quest->getAnswer(i)->getSource(), &questFile);

    FileSystem::getInst()->fsClose(&questFile);

    this->questions.remove(name);
}

void QuestPanel::updateQuestion(QString name, QString newName, Question *quest)
{
    this->questions.remove(name);
    this->questions.insert(newName, quest);
}

void QuestPanel::showRight(bool state)
{
    redactorShowRight = state;
#ifndef DEBUG_MSG
    qDebug() << "Right status: " << state;
#endif
}

void QuestPanel::renameQuestion(QString oldName, QString newName)
{
    Question *quest = this->questions[oldName];
    quest->setName(newName);
    this->questions.remove(oldName);
    this->questions.insert(newName, quest);

    for(int i = 0; i < this->topics.count(); i++)
    {
        if(this->topics[i]->hasQuestion(oldName))
        {
            this->topics[i]->removeQuestion(oldName);
            this->topics[i]->addQuestion(newName);
        }
    }
}

QStringList QuestPanel::getQuestGroups(QString value)
{
    QStringList list;

    for(int i = 0; i < this->topics.count(); i++)
        if(this->topics[i]->hasQuestion(value))
            list.push_back(this->topics[i]->getName());

    return list;
}

void QuestPanel::setTimeSettings(bool isOnTiem, int value)
{
    this->onTime = isOnTiem;
    this->time = value;
}

#endif

void QuestPanel::onEndHidStartScreenAnimation()
{

}

void QuestPanel::selectTab(QString name)
{
    for(int i = 0; i < this->tabBar->count(); i++)
    {
        if(this->tabBar->tabData(i).toString() == name)
        {
            this->tabBar->disconnect(this, SLOT(questTabChanged(int)));
            this->tabBar->setCurrentIndex(i);
            connect(this->tabBar, SIGNAL(currentChanged(int)), this, SLOT(questTabChanged(int)));
            break;
        }
    }
}

void QuestPanel::showQuestion(QString name)
{
#ifndef DEBUG_MSG
    qDebug() << "Показать вопрос: " << name;
#endif
    if(this->state == this->SELECTESER)
    {
        this->nextQuestion = name;

        if(startScreen != NULL)
        {
#ifndef DEBUG_MSG
            qDebug() << "startScreen not NULL";
#endif
            if(startScreen->state() == QAbstractAnimation::Running)
            {
#ifndef DEBUG_MSG
                qDebug() << "startScreen state = QAbstractAnimation::Running";
#endif
                startScreen->setCurrentTime(startScreen->totalDuration());
#ifndef DEBUG_MSG
                if(startScreen == NULL)
                    qDebug() << "startScreen is NULL after stoping";
#endif

                this->hideStartScreen();
            }
            else
                this->hideStartScreen();
        }
        else
            this->hideStartScreen();
    }
    else
    {
#ifndef DEBUG_MSG
        qDebug() << "Showing question";
#endif
        if(firstQuestion)
        {
            int tabHeight = 0;

#ifdef REDACTOR
            if(redactorShowTab)
#else
            if(showTab)
#endif
                tabHeight = this->tabBar->height();

#ifdef REDACTOR
            this->tabBar->setVisible(redactorShowTab);
#else
            this->tabBar->setVisible(showTab);
#endif

            this->questWidget = new QuestionWidgt(this, this->ui->questionArea);
            this->questWidget->setGeometry(QRect(0, 0, this->width() - 60,
                                                 this->height() - 50 - this->ui->nextBtn->height() -
                                                 tabHeight));

#ifdef REDACTOR
            this->questWidget->setShowRight(this->redactorShowRight);
#endif
            if(!this->tabState)
                this->selectTab(name);

            if(this->questWidget)
            {
                this->questWidget->setQuestion(this->questions[name]);
                this->questWidget->updateAnswers();
            }

            QPropertyAnimation *panelAnim = new QPropertyAnimation(this->ui->questionArea, "geometry");
            panelAnim->setStartValue(QRect(this->width() / 2, this->height() / 2, 1, 1));

            panelAnim->setEndValue(QRect(10, 10 + tabHeight, this->width() - 20,
                                         this->height() - 30 - this->ui->nextBtn->height() - tabHeight));

            panelAnim->setDuration(700);

            this->ui->questionArea->setWidget(this->questWidget);

            this->ui->questionArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            this->ui->questionArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            connect(panelAnim, SIGNAL(finished()), this, SLOT(onEndQuestAnimation()));
            panelAnim->setDuration(150);
            panelAnim->start(QAbstractAnimation::DeleteWhenStopped);
        }
        else
        {
            //Спрятать вопрос
            hideQuestion();
            firstQuestion = false;
            this->nextQuestion = name;
        }
    }
}

void QuestPanel::onEndQuestAnimation()
{
    if(this->state == this->SELECTESER)
    {
       this->state = this->SHOWQUESTION;

       if(this->questions.count())
       {
           this->showTestControlButtons();
           this->showQuestion(this->questions.keys()[0]);
       }

       firstQuestion = false;
    }

    this->ui->questionArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->ui->questionArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    if(!this->nextQuestion.isEmpty())
    {
        if(firstQuestion)
        {
            firstQuestion = false;
        }
        else
        {
            firstQuestion = true;
            if(this->questWidget)
            {
                this->questWidget->writeAnswers();
                delete this->questWidget;
            }

            if(!this->nextQuestion.isEmpty())
            {
                this->showQuestion(this->nextQuestion);
            }
        }
    }
}

void QuestPanel::hideQuestion()
{
    this->ui->questionArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->ui->questionArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPropertyAnimation *anim = new QPropertyAnimation(this->ui->questionArea, "geometry");
    anim->setStartValue(this->ui->questionArea->geometry());
    anim->setEndValue(QRect(this->width() / 2, this->height() / 2, 1, 1));

    if(this->state == this->SHOWQUESTION)
        connect(anim, SIGNAL(finished()), this, SLOT(onEndQuestAnimation()));
    else
    {
        this->ui->endBtn->setVisible(false);
        this->tabBar->setVisible(false);
        this->ui->lcdNumber->setVisible(false);
        connect(anim, SIGNAL(finished()), this, SLOT(showStartScreen()));
    }

    anim->setDuration(150);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void QuestPanel::showTestControlButtons()
{
    //Размеры окна и кнопок "Далее" и "Назад"
    QRect region = this->geometry();
    QRect btn1Rect = this->ui->nextBtn->geometry();
    QRect btn2Rect = this->ui->prevBtn->geometry();
    QRect btn3Rect = this->ui->endBtn->geometry();

    /*
      Анимация кнопки "Далее"
     */
    QPropertyAnimation *button1 = new QPropertyAnimation(this->ui->nextBtn, "pos");
    //Скорость воспроизведения
    button1->setDuration(1000);
    //Начальное значение
    button1->setStartValue(QPoint(-btn1Rect.width(),
                                  region.height() - btn1Rect.height() - 10));

    //Конечное значение
    button1->setEndValue(QPoint(region.width() - btn1Rect.width() - 10,
                                region.height() - btn1Rect.height() - 10));
    //Тип кривой ослабления
    button1->setEasingCurve(QEasingCurve::OutBounce);//OutBounce

    /*
      Анимация кнопки "Назад"
      */
    QPropertyAnimation *button2 = new QPropertyAnimation(this->ui->prevBtn, "pos");
#ifdef REDACTOR
    if(redactorNavigation)
#else
    if(navigation)
#endif
    {
        this->ui->prevBtn->setVisible(true);
        //Скорость воспроизведения
        button2->setDuration(1000);
        //Начальное значение
        button2->setStartValue(QPoint( - btn2Rect.width(), //- btn2Rect.width() - 15,
                                      region.height() - btn2Rect.height() - 10));

        //Конечное значение
        button2->setEndValue(QPoint(region.width() - btn1Rect.width() - btn2Rect.width() - 15,
                                    region.height() - btn2Rect.height() - 10));
        //Тип кривой ослабления
        button2->setEasingCurve(QEasingCurve::OutBounce);//OutBounce
    }
    else
        this->ui->prevBtn->setVisible(false);

    bool state = false;
#ifdef REDACTOR
    state = redactorNavigation;
#else
    state = navigation;
#endif

    QPropertyAnimation *end = new QPropertyAnimation(this->ui->endBtn, "pos");
#ifdef REDACTOR
    if(redactorEndButton)
#else
    if(endButton)
#endif
    {
        this->ui->endBtn->setVisible(true);
        end->setStartValue(QPoint(-btn3Rect.width(), region.height() - btn3Rect.height() - 10));

        end->setEndValue(QPoint(region.width() - btn1Rect.width() -
                                (state ? btn2Rect.width() + 20 : 15) - btn3Rect.width(),
                                region.height() - btn3Rect.height() - 10));
        end->setEasingCurve(QEasingCurve::OutBounce);
        end->setDuration(1000);
    }
    else
        this->ui->endBtn->setVisible(false);

    QPropertyAnimation *lcd = new QPropertyAnimation(this->ui->lcdNumber, "pos");
#ifdef REDACTOR
    if(redactorTimer)
#else
    if(time)
#endif
    {
        this->ui->lcdNumber->setVisible(true);
        lcd->setStartValue(QPoint(this->width() + this->ui->lcdNumber->width(), this->height() - 10 -
                                  this->ui->lcdNumber->height()));
        lcd->setEndValue(QPoint(10, this->height() - 10 - this->ui->lcdNumber->height()));
        lcd->setEasingCurve(QEasingCurve::OutBounce);
        lcd->setDuration(2000);
    }
    else
        this->ui->lcdNumber->setVisible(false);

    QPropertyAnimation *tab = new QPropertyAnimation(this->tabBar, "size");

#ifdef REDACTOR
    if(redactorShowTab)
#else
    if(showTab)
#endif
    {
        this->tabBar->setVisible(true);
        this->tabBar->setMaximumWidth(this->width() - 20);

#ifdef REDACTOR
        this->tabBar->setVisible(redactorShowTab);
        this->ui->lcdNumber->setVisible(redactorTimer);
#else
        this->tabBar->setVisible(showTab);
        this->ui->lcdNumber->setVisible(onTime);
#endif

        tab->setStartValue(this->tabBar->size());
        tab->setEndValue(QSize(this->width() - 20, this->tabBar->height()));
        tab->setDuration(700);
    }
    else
        this->tabBar->setVisible(false);

    QParallelAnimationGroup *aniGroup = new QParallelAnimationGroup;//Sequential
    aniGroup->addAnimation(button1);
#ifdef REDACTOR
    if(redactorNavigation)
#else
    if(navigation)
#endif
    aniGroup->addAnimation(button2);
#ifdef REDACTOR
    if(redactorShowTab)
#else
    if(showTab)
#endif
        aniGroup->addAnimation(tab);
#ifdef REDACTOR
    if(redactorTimer)
#else
    if(onTime)
#endif
        aniGroup->addAnimation(lcd);
#ifdef REDACTOR
    if(redactorEndButton)
#else
    if(endButton)
#endif
        aniGroup->addAnimation(end);

    aniGroup->start();
}

void QuestPanel::questTabChanged(int index)
{
    this->tabState = true;
    this->showQuestion(this->tabBar->tabData(index).toString());
    this->tabState = false;
#ifndef REDACTOR
    this->curQuestion = index;
#endif
}

#ifdef REDACTOR
void QuestPanel::setShowTab(bool state)
{
    this->redactorShowTab = state;
    this->showQuestion(this->nextQuestion);
    this->showTestControlButtons();
}

void QuestPanel::setShowEndButton(bool state)
{
    this->redactorEndButton = state;
    this->showTestControlButtons();
}

void QuestPanel::setNextButton(bool state)
{
    this->redactorNavigation = state;

    if(!this->redactorNavigation)
        this->redactorShowTab = false;

    this->showTestControlButtons();
}

#endif

void QuestPanel::onTimer()
{

}

void QuestPanel::onUpdateIndicator()
{
#ifndef REDACTOR
    if(this->questTime == 0 && this->sec == 0)
    {
        this->endTesting();
    }
    else
    {
#endif
        if(this->sec > 1)
            this->sec --;
        else
        {
            this->questTime --;
            this->sec = 59;
        }

        this->ui->lcdNumber->display(QString("%1:%L2")
                                     .arg(this->questTime, 2, 10, QLatin1Char('0'))
                                     .arg(this->sec, 2, 10, QLatin1Char('0')));
#ifndef REDACTOR
    }
#endif
}

#ifdef REDACTOR
void QuestPanel::removeFromTopic(QString topicName, QString questName)
{
    for(int i = 0; i < this->topicsCount(); i++)
        if(this->getTopic(i)->getName() == topicName)
        {
            QuetGroup *group = this->getTopic(i);
            group->removeQuestion(questName);
        }
}
#endif

void QuestPanel::buildQuestionList(QString param, int rnd_state, bool mixQuests, bool mixAnsw)
{
#ifndef DEBUG_MSG
    qDebug() << param;
#endif

    tstParam = param;

    if(tstParam == "all")
    {
        qDebug() << "all";
        questList = this->questions.keys();
    }
    else
    {
        //Список групп используемых для тестирования
        QStringList temp;
        QString p = param;

        while(!p.isNull())
        {
            int pos = p.indexOf(';');
            QString buf;

            if(pos != -1)
            {
                buf = p.left(pos);
                p.remove(buf + ";");
                temp.push_back(buf);
            }
            else
            {
                temp.push_back(p);
                param.remove(p);
                break;
            }
        }
#ifndef DEBUG_MSG
        qDebug() << temp;
#endif

        questList.clear();
        for(int i = 0; i < temp.count(); i++)
        {
#ifndef DEBUG_MSG
            qDebug() << temp[i];
#endif
            if(this->hasTopic(temp[i]))
            {
#ifndef DEBUG_MSG
                qDebug() << "Toppic" << temp[i] << " is avaibal";
#endif
                QuetGroup *group = this->getTopic(temp[i]);
                group->sortQuestions();

                if(rnd_state >= group->questCount() || rnd_state == 0)
                {
                    for(int j = 0; j < group->questCount(); j++)
                        if(!questList.contains(group->getQuestName(j)))
                            questList.push_back(group->getQuestName(j));
                }
                else
                {
                    int selected = 0;
                    while(selected < rnd_state)
                    {
                        int r = qrand() % rnd_state;
                        if(!questList.contains(group->getQuestName(r)))
                        {
                            questList.push_back(group->getQuestName(r));
                            selected++;
                        }
                    }
                }
            }
        }
    }

    //Построение идентификатора теста: Заголовок окна + список групп + rnd сид
    testIdenti = QString(this->windowCaption.toAscii() + "_" + tstParam + "_0x" + QString::number(rnd_state, 16) +
                         "_0x" + QString::number((int)mixQuests, 16));

    testIdenti = QString(QCryptographicHash::hash(QByteArray(this->testIdenti.toLocal8Bit()), QCryptographicHash::Md5).toHex());

    //Перемешивание вопросов
    if(this->mixQuestions || mixQuests)
    {
        for(int i = 0; i < questList.count(); i++)
        {
            for(int i = 0; i < 5; i++)
            {
                int id1 = qrand() % this->questList.count();
                int id2 = qrand() % this->questList.count();
                qSwap(questList[id1], questList[id2]);
            }
        }
    }

    //Перемешивание ответов
    if(this->mixAnswers || mixAnsw)
    {
        for(int i = 0; i < questList.count(); i++)
            this->questions[questList[i]]->mixAnswers(5);
    }


    if(this->questList.count() > 0)
    {
        this->updateTabBar();
        this->nextQuestion = this->questList[0];
#ifdef REDACTOR
        if(this->redactorEndButton)
#else
        if(this->endButton)
#endif
        {

        }
    }


    qDebug() << "testIdenti: " << testIdenti;
#ifndef DEBUG_MSG
    qDebug() << "Quest list: " << questList;
#endif

#ifndef REDACTOR
    //endTesting();
#endif
}

void QuestPanel::addUnsatisfactoryQuestions()
{
    QSqlQuery query;

    QFile f(":/select_unsatisfactory_questions.sql");
    if(f.open(QIODevice::ReadOnly))
    {
        QString str;
        str = f.readAll();
        f.close();

        if(!str.isEmpty())
        {
            query.prepare(str);
            query.bindValue(":subject", this->subject);
            query.bindValue(":student", this->student);

            if(query.exec())
            {
                while(query.next())
                {
                    QByteArray data = qUncompress(query.value(0).toByteArray());
                    QDomDocument doc("report_data");
                    if(doc.setContent(data))
                    {
                        QDomElement docElem = doc.documentElement();
                        QDomNode n = docElem.firstChild();
                        while(!n.isNull())
                        {
                            QDomElement e = n.toElement();
                            QString name = e.attribute("name", "");

                            if(!name.isEmpty() && !this->questList.contains(name))
                                this->questList.insert(qrand() % this->questList.count(), name);
                            n = n.nextSibling();
                        }
                    }
                }
            }
        }
    }
}

void QuestPanel::setUnsatisfactory(bool value)
{
    this->unsatisfactory = value;
}

void QuestPanel::onInfoButton()
{
    static bool shown = false;
    QPropertyAnimation *anim = new QPropertyAnimation(this->ui->infoBrowser, "geometry");

    //Показать информацию
    if(!shown)
    {
        anim->setStartValue(this->ui->infoButton->geometry());
        anim->setEndValue(QRect(40, 40, this->width() - 80, this->height() - 80));
        this->ui->infoBrowser->setVisible(true);
    }
    else
    {
        anim->setStartValue(this->ui->infoBrowser->geometry());
        anim->setEndValue(this->ui->infoButton->geometry());
        connect(anim, SIGNAL(finished()), this, SLOT(onEndShowTestInfo()));
    }

    shown = !shown;

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void QuestPanel::onEndShowTestInfo()
{
    this->ui->infoBrowser->setVisible(false);
}

#ifndef REDACTOR
void QuestPanel::endTesting()
{
    this->state = REPORTCREEN;
    //Подсчет набраных пользователем баллов
    int scores = 0;
    //Количество вопросов в тесте
    int questCount = this->questList.count();
    for(int i = 0; i < this->questions.count(); i++)
        scores += this->getQuestion(i)->getUserScore();

    //Максимальное количество баллов
    int maxScore = 0;
    for(int i = 0; i < questCount; i++)
    {
        Question *quest = this->getQuestion(i);
        if(quest->getType() == "0")
            maxScore += quest->getAnsvRCount();
        else if(quest->getType() == "1")
            maxScore++;
        else
            maxScore += this->getPlugin(quest->getType())->maxUserScore();
    }
    //процент правильных ответов
    int percent = (int)(((float)scores / (float)maxScore) * 100);

    int j /*= this->testScores.count() - 1*/;
    int count = this->testScores.count() - 1;
    //for(j; j > 0; j--)
    while(j < count)
    {
        if(percent >= this->testScores[j + 1]->percent)
            break;
        j++;
    }

    //Генерировать отчет
    FileSystem::getInst()->fsOpen(&questFile);
    QString report = FileSystem::getInst()->fsGetFile("report_template.html", &questFile);
    FileSystem::getInst()->fsClose(&questFile);

    if(!report.isEmpty())
    {
        if(this->onTime)
        {
            report = report.replace("${time}", QString("%1 мин.").arg(this->time));
        }
        else
        {
            report = report.replace("${time}", "<I>Неограничено.</I>");
        }

        report = report.replace("${quests_count}", QString("%1").arg(questCount));
        if(report.contains("${scores}"))
        {
            QString info;
            for(int i = 0; i < this->testScores.count(); i++)
            {
                info += QString("%1% - %2(%3)")
                        .arg(this->testScores[i]->percent)
                        .arg(this->testScores[i]->value)
                        .arg(this->testScores[i]->text) + "\n";
            }
            report = report.replace("${scores}", info);
        }
        report = report.replace("${percent}", QString("%1%, %2(%3)")
                                .arg(percent)
                                .arg(this->testScores[j]->value)
                                .arg(this->testScores[j]->text));

        report = report.replace("${student}", student_name);
        report = report.replace("${group}", group);
        report = report.replace("${student_score}", this->testScores[j]->text);
        report = report.replace("${percent}", QString::number(percent));
    }

    QSqlQuery query;
    query.prepare(QuestPanel::getFile(":/query/select_subject.sql"));
    query.bindValue(0, this->subject);

    if(query.exec())
    {
        query.next();
        report = report.replace("${subject}", query.value(0).toString());

        QTextEdit *edit = new QTextEdit;
        edit->setHtml(report);
        if(edit->find("${body}"))
        {
            FileSystem::getInst()->fsOpen(&questFile);
            QString html = "<p>";

            for(int i = 0; i < this->questList.count(); i++)
            {
                html += QString("<p><p><H3>Вопрос %1</H3></p>").arg(i + 1);
                Question *quest = this->getQuestion(this->questList[i]);
                QByteArray data = FileSystem::getInst()->fsGetFile(quest->getSrc(), &questFile);
                html += data;
                html += QString("<B>Ответ студента: </B>%1")
                        .arg(quest->getRightAnswer());
            }
            FileSystem::getInst()->fsClose(&questFile);
            html += "</p>";

            edit->insertHtml(html);
        }

        QString html = edit->toHtml();

        this->ui->infoBrowser->setHtml(html);
    }

#ifndef DEBUG_MSG
    QFile file("c:/123.html");
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream f(&file);
        f << edit->toHtml();
    }
    qDebug() << "Score: " << this->testScores[i]->text
            << "(" << this->testScores[i]->value << ")";
#endif
    QByteArray reportData;

    //Запись имен вопросов
    QDomDocument doc("report_data");
    QDomNode xmlNode = doc.createProcessingInstruction("xml",
                                                       "version=\"1.0\" encoding=\"Windows-1251\"");
    doc.appendChild(xmlNode);
    QDomElement root = doc.createElement("root");
    for(int i = 0; i < this->questList.count(); i++)
    {
        QDomElement quest = doc.createElement("quest");
        quest.setAttribute("name", this->questList[i]);
        QDomText n = doc.createTextNode(this->questions[this->questList[i]]->getUserAnswer());
        quest.appendChild(n);

        root.appendChild(quest);
    }

    QTextStream stream(&reportData);
    stream.setCodec("Windows-1251");
    doc.save(stream, 4);

    reportData = qCompress(reportData, 2);

    query.prepare("call UpdateTestResults(:subj, :testIdenty, :studID, :scType, :rCount, "
                                         ":rPercent, :uScore, :altScore, :uReport, :tst_name);");
    query.bindValue(":subj", this->subject);
    query.bindValue(":testIdenty", this->testIdenti);
    query.bindValue(":studID", this->student);
    query.bindValue(":scType", this->scoreType);
    query.bindValue(":rCount", scores);
    query.bindValue(":rPercent", percent);
    query.bindValue(":uScore", this->testScores[j]->value);
    query.bindValue(":altScore", this->testScores[j]->text);
    query.bindValue(":uReport", reportData);
    query.bindValue(":tst_name", QString("%1\n%2")
                    .arg(windowCaption)
                    .arg(tstParam));

    if(!query.exec())
    {
        QMessageBox msg(this);
        msg.setWindowTitle("Ошибка!");
        msg.setText("Не удалось обновить результаты тестирования!");
        msg.setInformativeText("Обратитесь к преподавателю!");
        msg.setDetailedText(query.lastError().text());
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }

    this->hideQuestWindow();

    emit testEnd();
}

void QuestPanel::onEndButton()
{
    QMessageBox msg(this);
    msg.setText("Вы действительно хотите завершит тест?");
    msg.setWindowTitle("Завершение тестирования...");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::No);
    msg.setIcon(QMessageBox::Question);

    if(msg.exec() == QMessageBox::Yes)
    {
        if(!passwordOnEnd.isEmpty())
        {
            bool ok;
            QString text = QInputDialog::getText(this, "Завершение тестирования.",
                                                 "Введите пароль для подтверждения завершения:",
                                                 QLineEdit::Password, "", &ok);
            if(ok && !text.isEmpty())
            {
                if(text == passwordOnEnd)
                {
                    this->endTesting();
                }
                else
                    QMessageBox::critical(this, "Ошибка!", "Пароль неверен!");
            }
        }
        else
            this->endTesting();
    }
}

void QuestPanel::hideQuestWindow()
{
    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    QSequentialAnimationGroup *group2 = new QSequentialAnimationGroup;

    QPropertyAnimation *anim1 = new QPropertyAnimation(this->ui->questionArea, "pos");
    anim1->setStartValue(this->ui->questionArea->pos());
    anim1->setEndValue(QPoint(-this->ui->questionArea->width() - 10,
                              this->ui->questionArea->y()));
    group->addAnimation(anim1);

    QPropertyAnimation *anim2 = new QPropertyAnimation(this->ui->nextBtn, "pos");
    anim2->setStartValue(this->ui->nextBtn->pos());
    anim2->setEndValue(QPoint(-this->ui->nextBtn->width() - 10,
                              this->ui->nextBtn->y()));
    group->addAnimation(anim2);

    if(this->navigation)
    {
        QPropertyAnimation *nav = new QPropertyAnimation(this->ui->prevBtn, "pos");
        nav->setStartValue(this->ui->prevBtn->pos());
        nav->setEndValue(QPoint(-this->ui->prevBtn->width() - 10,
                                  this->ui->prevBtn->y()));
        group->addAnimation(nav);
    }

    if(this->showTab)
    {
        QPropertyAnimation *tab = new QPropertyAnimation(this->tabBar, "pos");
        tab->setStartValue(this->tabBar->pos());
        tab->setEndValue(QPoint(-this->tabBar->width() - 10,
                                  this->tabBar->y()));
        group->addAnimation(tab);
    }

    if(this->onTime)
    {
        QPropertyAnimation *lcd = new QPropertyAnimation(this->ui->lcdNumber, "pos");
        lcd->setStartValue(this->ui->lcdNumber->pos());
        lcd->setEndValue(QPoint(-this->ui->lcdNumber->width() - 10,
                                  this->ui->lcdNumber->y()));
        group->addAnimation(lcd);
    }

    QPropertyAnimation *info = new QPropertyAnimation(this->ui->infoBrowser, "geometry");
    info->setStartValue(QRect(this->width() / 2, this->height() / 2, 0, 0));
    info->setEndValue(QRect(10, 10, this->width() - 20, this->height() - 20));

    group2->addAnimation(group);
    group2->addAnimation(info);

    group2->start(QAbstractAnimation::DeleteWhenStopped);

    connect(group2, SIGNAL(finished()), this, SLOT(onInfoButton()));
}
#endif

ShowPanelInterface *QuestPanel::getViewPanel(QString name)
{
    for(int i = 0; i < this->plugins.count(); i++)
    {
        if(this->plugins.at(i)->isQuestionPlugin() && this->plugins.at(i)->questType() == name)
            return this->plugins.at(i)->shown();
    }

    return NULL;
}

TBPluginInterface *QuestPanel::getPlugin(QString name)
{
    for(int i = 0; i < this->plugins.count(); i++)
    {
        if(this->plugins.at(i)->isQuestionPlugin() && this->plugins.at(i)->questType() == name)
            return this->plugins.at(i);
    }

    return NULL;
}
