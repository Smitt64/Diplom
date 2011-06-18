#include "questionwidgt.h"
#include "ui_questionwidgt.h"
#include "qdebug.h"
#include "qcompleter.h"
#include <qlineedit.h>
#include <../filesystem/filesystem.h>
#include <../questpanel/questpanel.h>

#define TO_QUEST_PANEL(widget)((QuestPanel*)(widget))

QuestionWidgt::QuestionWidgt(QWidget *panelW, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuestionWidgt),
    panel(panelW),
    group(NULL),
    plug(NULL)
#ifdef REDACTOR
    ,showRight(false)
#endif
{
    ui->setupUi(this);
    this->group = new QButtonGroup;

    for(int i = 0; i < this->answers.count(); i++)
        delete this->answers[i];

    this->questBrowser = new TextBrowser(&TO_QUEST_PANEL(panelW)->questFile, this);

    this->questBrowser->setObjectName(QString::fromUtf8("questBrowser"));
    this->questBrowser->setGeometry(QRect(10, 10, 381, 121));
    this->questBrowser->setFrameShape(QFrame::NoFrame);
    this->questBrowser->setFrameShadow(QFrame::Plain);
    this->questBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->questBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->questBrowser->setAutoFormatting(QTextEdit::AutoAll);
    this->questBrowser->setAcceptRichText(false);
    this->questBrowser->setTextInteractionFlags(Qt::NoTextInteraction);
    this->questBrowser->setOpenLinks(false);

    this->questBrowser->viewport()->setAutoFillBackground(false);
    this->ui->label->setVisible(false);
    this->ui->questBox->setVisible(false);

    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::transparent);
    this->setPalette(p);
    this->setVisible(true);
}

QuestionWidgt::~QuestionWidgt()
{
    for(int i = 0; i < this->answers.count(); i++)
        delete this->answers[i];

    this->answers.clear();

    delete this->group;

    delete ui;
}

void QuestionWidgt::calculateSize()
{
    //Положение следующего компонента
    int yPos = 0;

    QString type = this->question->getType();
    if(type == "0" || type == "1")
    {
        QSize sz(this->geometry().width(),
                 this->geometry().height());
        this->questBrowser->setMaximumSize(sz);
        this->questBrowser->setMinimumSize(sz);

        this->questBrowser->document()->adjustSize();
        sz = this->questBrowser->document()->size().toSize() + QSize(8, 8);
        this->questBrowser->document()->setPageSize(QSizeF(sz));

        sz.setWidth(this->width() + 20);

        this->questBrowser->setMaximumSize(sz);
        this->questBrowser->setMinimumSize(sz);

        sz.setHeight(this->questBrowser->document()->size().height() + 5);
        this->questBrowser->setMaximumSize(sz);
        this->questBrowser->setMinimumSize(sz);

        this->questBrowser->setProperty("pos", QPoint(0, 0));

        for(int i = 0; i < answers.count(); i++)
        {
            answers[i]->setGeometry(0, 0, 0, 0);
        }

        yPos = this->questBrowser->height() + 5;

        if(type == "0")
        {
            for(int i = 0; i < answers.count(); i++)
            {
                answers[i]->setGeometry(QRect(0, yPos, this->width(), 10));
                answers[i]->setupGeometry();

                yPos += answers[i]->height() + 10;
            }
            //Подгон размера поля
            this->adjustSize();
        }
        else if(this->question->getType() == "1")
        {
            this->ui->questBox->setVisible(true);
            this->ui->label->setVisible(true);

            this->ui->questBox->setProperty("pos", QPoint(60, yPos));
            this->ui->label->setProperty("pos", QPoint(10, yPos));
            //Подгон размера поля
            this->adjustSize();
        }
        else
        {
            //this->plug->setGeometry(this->geometry());
        }
    }
    this->adjustSize();
}

void QuestionWidgt::setQuestion(Question *quest)
{
    this->question = quest;

    int yPos = 0;
    QString qtype = this->question->getType();
    if(qtype == "0" || qtype == "1")
    {
        this->questBrowser->setSource(this->question->getSrc());

        //Положение следующего компонента
        //Подгон размера вопроса
        QSize sz(this->geometry().width(),
                 this->geometry().height());
        this->questBrowser->setMaximumSize(sz);
        this->questBrowser->setMinimumSize(sz);

        this->questBrowser->document()->adjustSize();
        sz = this->questBrowser->document()->size().toSize() + QSize(8, 8);
        this->questBrowser->document()->setPageSize(QSizeF(sz));

        sz.setWidth(this->width() + 20);

        this->questBrowser->setMaximumSize(sz);
        this->questBrowser->setMinimumSize(sz);

        sz.setHeight(this->questBrowser->document()->size().height() + 5);
        this->questBrowser->setMaximumSize(sz);
        this->questBrowser->setMinimumSize(sz);

        this->questBrowser->setProperty("pos", QPoint(0, 0));

        this->adjustSize();

        yPos = this->questBrowser->height() + 5;
    }

    if(qtype == "0")
    {
        AnswerWidget::Type type = AnswerWidget::RADIOBOX;

        if(this->question->getAnsvRCount() > 1)
        {
            type = AnswerWidget::CHECKBOX;
            this->group->setExclusive(false);
        }
        else
            this->group->setExclusive(true);

        this->answers.clear();

        //Добавление ответов
        for(int i = 0; i < this->question->getAnsvCount(); i++)
        {
            AnswerWidget *ans = new AnswerWidget(&TO_QUEST_PANEL(this->panel)->questFile,
                                                this->question->getAnswer(i)->getSource(),
                                                type, this);
            ans->setAnswer(this->question->getAnswer(i));
            ans->setRightAnswer(this->question->getAnswer(i)->isRight());

#ifdef REDACTOR
            if(this->showRight)
            {
                ans->setShowRight(this->showRight);

            }
#endif
            ans->setGeometry(QRect(0, yPos, this->parentWidget()->geometry().width(), 10));
            ans->setupGeometry();

            this->group->addButton(ans->checkWidget());
            ans->setChecked(this->question->getAnswer(i)->isChecked());
            ans->setVisible(true);

            yPos += ans->height() + 10;

            this->answers.push_back(ans);
        }
    }
    else if(qtype == "1")
    {
        QCompleter *complete = new QCompleter(TO_QUEST_PANEL(this->panel)->answerList, this);
        this->ui->questBox->setCompleter(complete);
        this->ui->questBox->addItems(TO_QUEST_PANEL(this->panel)->answerList);

        this->ui->questBox->setVisible(true);
        this->ui->label->setVisible(true);

        this->ui->questBox->setProperty("pos", QPoint(60, yPos));
        this->ui->label->setProperty("pos", QPoint(10, yPos));

        if(!this->question->getUserAnswer().isEmpty())
            this->ui->questBox->lineEdit()->setText(this->question->getUserAnswer());
    }
    else if(qtype != "unknown")
    {
        plug = TO_QUEST_PANEL(this->panel)->getViewPanel(this->question->getType());

        if(plug)
        {
            this->questBrowser->setVisible(false);
            this->ui->label->setVisible(false);
            this->ui->questBox->setVisible(false);

            connect(plug, SIGNAL(needResource(QString,ShowPanelInterface*)), this, SLOT(loadResource(QString,ShowPanelInterface*)));
            plug->setQuestion(quest);
            QHBoxLayout *frameLayout = new QHBoxLayout(this);
            frameLayout->addWidget(plug);
            plug->setVisible(true);

            /*QRect rect = this->parentWidget()->geometry();
            rect.setLeft(0);
            rect.setTop(0);
            rect.setWidth(rect.width() - 50);
            rect.setHeight(rect.height() - 50);
            this->plug->setGeometry(rect);
            this->setGeometry(rect);*/

        }
    }
    //Подгон размера поля
    this->adjustSize();
}

void QuestionWidgt::loadResource(QString name, ShowPanelInterface *ptr)
{
    FileSystem::getInst()->fsOpen(&TO_QUEST_PANEL(this->panel)->questFile);
    qDebug() << name;
    if(FileSystem::getInst()->fsHasFile(name, &TO_QUEST_PANEL(this->panel)->questFile))
    {
        ptr->setResource(name, FileSystem::getInst()->fsGetFile(name, &TO_QUEST_PANEL(this->panel)->questFile));
    }
    FileSystem::getInst()->fsClose(&TO_QUEST_PANEL(this->panel)->questFile);
}

void QuestionWidgt::changeEvent(QEvent *e)
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

void QuestionWidgt::writeAnswers()
{
    if(this->question->getType() == "0")
    {
        QString answ("");
        for(int i = 0; i < this->question->getAnsvCount(); i++)
        {
            if(this->answers[i]->isChecked())
                answ += QString::number(i) + ";";
        }
        this->question->setTextAnswer(answ);
    }
    else if(this->question->getType() == "1")
        this->question->setTextAnswer(this->ui->questBox->currentText());
    else
    {
        if(this->plug)
            this->plug->saveQuestionData(this->question);
    }
}

void QuestionWidgt::updateAnswers()
{
    if(this->question)
    {
        QString qtype = this->question->getType();
        if(qtype == "0")
        {
            for(int i = 0; i < this->answers.count(); i++)
                this->answers[i]->setChecked(this->question->getAnswer(i)->isChecked());
        }
        else if(qtype == "1")
        {
        }
        else if(qtype != "unknown")
        {
            if(this->plug)
                this->plug->loadQuestionData(this->question);
        }
    }
}

void QuestionWidgt::resizeEvent(QResizeEvent *event)
{
    /*if(this->plug)
    {
        QRect rect = this->parentWidget()->geometry();
        rect.setLeft(0);
        rect.setTop(0);
        rect.setWidth(rect.width() - 50);
        rect.setHeight(rect.height() - 50);
        this->plug->setGeometry(rect);
        this->setGeometry(rect);
    }*/
}

#ifdef REDACTOR
void QuestionWidgt::setShowRight(bool state)
{
    showRight = state;
}
#endif
