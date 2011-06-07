#include "groupeditor.h"
#include "ui_groupeditor.h"

GroupEditor::GroupEditor(QuestPanel *panel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupEditor),
    questPanel(panel)
{
    ui->setupUi(this);

    QStringList topics = panel->getTopicNames();
    QStringList quest = panel->getQuestNames();
}

GroupEditor::~GroupEditor()
{
    delete ui;
}

void GroupEditor::changeEvent(QEvent *e)
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

void GroupEditor::setGroup(QString name)
{
    this->ui->lineEdit->setText(name);
    this->groupName = name;

    QStringList quest = this->questPanel->getQuestNames();
    for(int i = 0; i < quest.count(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(quest[i]);
        item->setCheckState(Qt::Unchecked);

        if(this->questPanel->getTopic(name)->hasQuestion(quest[i]))
            item->setCheckState(Qt::Checked);

        this->ui->listWidget->addItem(item);
    }
}

QStringList GroupEditor::getQuestions()
{
    QStringList questions;

    for(int i = 0; i < this->ui->listWidget->count(); i++)
        if(this->ui->listWidget->item(i)->checkState() == Qt::Checked)
            questions.push_back(this->ui->listWidget->item(i)->text());

    return questions;
}

QString GroupEditor::getName()
{
    return this->ui->lineEdit->text();
}
