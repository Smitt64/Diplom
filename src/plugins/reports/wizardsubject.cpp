#include "wizardsubject.h"
#include "ui_wizardsubject.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

WizardSubject::WizardSubject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WizardSubject)
{
    ui->setupUi(this);

    QSqlQuery query;

    if(query.exec("select caption from groups;"))
    {
        while(query.next())
        {
            this->ui->comboBox->addItem(query.value(0).toString());
        }
    }

    onUpdateSpec(this->ui->comboBox->currentText());

    connect(this->ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onUpdateSpec(QString)));
}

WizardSubject::~WizardSubject()
{
    delete ui;
}

void WizardSubject::onUpdateSpec(QString value)
{
    this->ui->comboBox_2->clear();
    QSqlQuery query;
    query.prepare("SELECT DISTINCT "
                  "`subject`.`subject`"
                  "FROM "
                  "groups "
                  "INNER JOIN specialty ON specialty.id = groups.special "
                  "INNER JOIN subject_specialty ON specialty.id = subject_specialty.specialty "
                  "INNER JOIN `subject` ON `subject`.id = subject_specialty.`subject` "
                  "WHERE "
                  "groups.caption = :param "
                  "ORDER BY "
                  "`subject`.`subject` ASC;");

    query.bindValue(":param", value);

    if(query.exec())
    {
        while(query.next())
           this->ui->comboBox_2->addItem(query.value(0).toString());
    }
    else
        QMessageBox::critical(this, "Îøèáêà!", query.lastError().text());
}

QComboBox *WizardSubject::subject()
{
    return this->ui->comboBox_2;
}

QString WizardSubject::getGroup()
{
    return this->ui->comboBox->currentText();
}
