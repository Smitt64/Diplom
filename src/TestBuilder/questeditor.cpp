#include "questeditor.h"
#include "ui_questeditor.h"
#include <qdebug.h>
#include <qmessagebox.h>
#include "texteditor.h"
#include <qlineedit.h>
#include <qdirmodel.h>
#include <qcompleter.h>

QuestEditor::QuestEditor(QuestPanel *panel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestEditor),
    modify(false),
    pan(panel)
{
    ui->setupUi(this);

    this->ui->comboBox->addItems(this->pan->getTestTypes());

    this->ui->groupsList->clear();

    for(int i = 0; i < panel->topicsCount(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(panel->getTopicNames()[i]);
        item->setCheckState(Qt::Unchecked);
        this->ui->groupsList->addItem(item);
    }

    if(this->ui->groupsList->count())
    {
        QListWidgetItem *item = this->ui->groupsList->item(0);

        if(item->text() == tr("(Без темы)"))
            item->setCheckState(Qt::Checked);
    }

    QCompleter *completer = new QCompleter(this->pan->answerList, this);
    this->ui->answerBox->addItems(this->pan->answerList);
    this->ui->answerBox->setCompleter(completer);

    connect(this->ui->toolButton, SIGNAL(clicked()), this, SLOT(onEditQuestText()));
    connect(this->ui->addButton, SIGNAL(clicked()), this, SLOT(onAddAnswer()));
    connect(this->ui->editButton, SIGNAL(clicked()), this, SLOT(onEditAnswer()));
    connect(this->ui->deleteButton, SIGNAL(clicked()), this, SLOT(onRemoveAnswer()));
    connect(this->ui->comboBox, SIGNAL(activated(int)), this, SLOT(onUpdateType(int)));
}

QuestEditor::~QuestEditor()
{
    delete ui;
}

void QuestEditor::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QuestEditor::onEditQuestText()
{
    TextEditor editor(this->pan, this);
    QString buf = this->ui->textBrowser->toHtml();

    editor.setDocument(this->ui->textBrowser->document());

    if(editor.exec() == QDialog::Accepted)
        this->ui->textBrowser->setHtml(editor.getHTML());
    else
    {
        if(!this->modify)
            this->ui->textBrowser->clear();
        else
            this->ui->textBrowser->setHtml(buf);

    }
}

void QuestEditor::onAddAnswer()
{
    TextEditor editor(this->pan, this);

    editor.loadResources();

    if(editor.exec() == QDialog::Accepted)
    {
        QString label;
        QString html = editor.getHTML();

        label = "answer" + QString::number(this->pan->counter);

        this->pan->counter++;

        QListWidgetItem *item = new QListWidgetItem;
        item->setText(label);
        item->setData(Qt::UserRole, html);
        item->setCheckState(Qt::Unchecked);

        this->ui->AnswerList->addItem(item);
    }
}

void QuestEditor::onEditAnswer()
{
    QListWidgetItem *item = this->ui->AnswerList->currentItem();

    TextEditor editor(this->pan, this);
    editor.loadResources();
    editor.setHtml(item->data(Qt::UserRole).toString());

    if(editor.exec() == QDialog::Accepted)
    {
        QString label;
        QString html = editor.getHTML();

        label = "answer" + QString::number(this->pan->counter);

        this->pan->counter++;

        item->setText(label);
        item->setData(Qt::UserRole, html);
    }
}

void QuestEditor::onRemoveAnswer()
{
    if(QMessageBox::question(this, tr("Удаление вопроса!"), tr("Удалить вариант ответа?"),
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
        == QMessageBox::Yes)
        delete this->ui->AnswerList->currentItem();
}

Question *QuestEditor::getQuestion()
{
    QString name = "quest";
    Question *retQuestion = NULL;

    QString type = QString::number(this->ui->comboBox->currentIndex());

    if(!this->modify)
    {
        name += QString::number(this->pan->counter);
        this->pan->counter++;
    }
    else
        name = this->quest->getName();

    QString questSrc = "source\\" + name + ".html";

    if(type == "0" || type == "1")
    {
        //Запись текста вопроса в архив
        FileSystem::getInst()->fsOpen(&this->pan->questFile);

        QByteArray html(this->ui->textBrowser->toHtml().toLocal8Bit().data());
        FileSystem::getInst()->fsAddTempFile(html, questSrc, &pan->questFile, 2);

        if(this->modify)
        {
            if(FileSystem::getInst()->fsHasFile(this->quest->getSrc(), &pan->questFile))
                FileSystem::getInst()->fsDelete(this->quest->getSrc(), &pan->questFile);

            for(int i = 0; i < this->quest->getAnsvCount(); i++)
            {
                if(FileSystem::getInst()->fsHasFile(this->quest->getAnswer(i)->getSource(),
                                                    &pan->questFile))
                    FileSystem::getInst()->fsDelete(this->quest->getAnswer(i)->getSource(),
                                                    &pan->questFile);
            }
        }
        else
            this->pan->counter++;
        FileSystem::getInst()->fsClose(&this->pan->questFile);
    }

    //Повсчет количества правильных ответов и добавление их в архив
    if(type == "0")
    {
        FileSystem::getInst()->fsOpen(&this->pan->questFile);
        int right = 0;
        QList<Answer*> Answers;

        for(int i = 0; i < this->ui->AnswerList->count(); i++)
        {
            QString label = this->ui->AnswerList->item(i)->text();
            label.remove('\n');
            label = label.left(10);
            label.replace(' ', "_");

            QString source = "source\\answers\\" + this->ui->AnswerList->item(i)->text() + ".html";
            bool checked = false;

            if(this->ui->AnswerList->item(i)->checkState() == Qt::Checked)
            {
                checked = true;
                right++;
            }

            this->pan->counter++;
            Answers.push_back(new Answer(label, source, checked));

            FileSystem::getInst()->fsAddTempFile(this->ui->AnswerList->item(i)->data(Qt::UserRole).toByteArray(),
                                             source, &pan->questFile, 2);
        }
        FileSystem::getInst()->fsClose(&pan->questFile);
        return new Question(name, questSrc, Answers, type, right);
    }
    else if(type == "1")
    {
        //FileSystem::getInst()->fsClose(&pan->questFile);
        return new Question(name, questSrc, this->ui->answerBox->currentText(), type);
    }
    else
    {
        Question *quest = this->editor->makeQuestion(this->pan->counter++);
        qDebug() << "" << quest->getSrc();
        return quest;
    }

    return NULL;
}

QStringList QuestEditor::getGroups()
{
    QStringList list;

    for(int i = 0; i < this->ui->groupsList->count(); i++)
    {
        if(this->ui->groupsList->item(i)->checkState() == Qt::Checked)
            list.push_back(this->ui->groupsList->item(i)->text());
    }

    return list;
}

QString QuestEditor::getOldQuestName()
{
    return this->quest->getName();
}

void QuestEditor::setQuestion(Question *question)
{
    this->modify = true;
    quest = question;

    FileSystem::getInst()->fsOpen(&this->pan->questFile);
    this->ui->textBrowser->setHtml(FileSystem::getInst()->fsGetFile(quest->getSrc(), &this->pan->questFile));

    if(this->quest->getType() == "0")
    for(int i = 0; i < quest->getAnsvCount(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(quest->getAnswer(i)->getName());

        if(quest->getAnswer(i)->isRight())
            item->setCheckState(Qt::Checked);
        else item->setCheckState(Qt::Unchecked);

        item->setData(Qt::UserRole, FileSystem::getInst()->fsGetFile(quest->getAnswer(i)->getSource(),
                                                       &this->pan->questFile));

        this->ui->AnswerList->addItem(item);
    }
    else if(this->quest->getType() == "1")
    {
        this->ui->comboBox->setCurrentIndex(1);
        this->onUpdateType(1);
        this->ui->answerBox->lineEdit()->setText(this->quest->getRightAnswer());
    }

    FileSystem::getInst()->fsClose(&this->pan->questFile);
}

void QuestEditor::onUpdateType(int index)
{
    if(index == 0 || index == 1)
    {
        this->ui->stackedWidget_2->setCurrentIndex(index);
        this->ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        this->ui->stackedWidget_2->setCurrentIndex(2);
        this->ui->stackedWidget->setCurrentIndex(1);

        editor = this->pan->getEditor(this->ui->comboBox->currentText());

        if(editor)
        {
            this->ui->stackedWidget->addWidget(editor);
            this->ui->stackedWidget->setCurrentWidget(editor);
        }
    }
}

void QuestEditor::saveResources()
{
    if(this->ui->comboBox->currentIndex() != 0 && this->ui->comboBox->currentIndex() != 1)
    {
        QuestEditorInterface *qedit = this->editor;

        int res = qedit->getResCount();

        for(int i = 0; i < res; i++)
        {
            QString name;
            QByteArray data;
            qedit->getResource(i, &name, &data);

            FileSystem::getInst()->fsOpen(&this->pan->questFile);
            qedit->getResource(i, &name, &data);
            qDebug() << "QuestEditor::saveResources()" << name << data.size();
            FileSystem::getInst()->fsAddTempFile(data, name, &this->pan->questFile);
            FileSystem::getInst()->fsClose(&this->pan->questFile);

        }
    }
}

void QuestEditor::setGroups(QStringList value)
{
    for(int i = 0; i < value.count(); i++)
        this->checkGroup(value[i]);
}

void QuestEditor::checkGroup(QString group)
{
    for(int i = 0; i < this->ui->groupsList->count(); i++)
        if(this->ui->groupsList->item(i)->text() == group)
            this->ui->groupsList->item(i)->setCheckState(Qt::Checked);
}

void QuestEditor::setQuestTypes(QStringList value)
{
    QStringList list = value;
    for(int i = 0; i < list.count(); i++)
        this->ui->comboBox->addItem(list[i]);
}
