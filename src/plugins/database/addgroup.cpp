#include "addgroup.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "ui_addgroup.h"

AddGroup::AddGroup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddGroup)
{
    ui->setupUi(this);

    this->ui->dateEdit->setDate(QDate(QDate::currentDate().year(), 9, 1));

    QSqlQuery query;

    specModel = new QSqlQueryModel;
    specModel->setQuery("SELECT specialty.caption, specialty.prefix FROM specialty;");

    this->ui->specBox->setModel(specModel);

    updateName("");

    connect(this->ui->numberEdit, SIGNAL(textChanged(QString)), this, SLOT(updateName(QString)));
    connect(this->ui->specBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateName(QString)));
}

AddGroup::~AddGroup()
{
    delete ui;
}

void AddGroup::updateName(QString)
{
    QDate date = QDate::currentDate();

    int cource = date.year() - this->ui->dateEdit->date().year() + 1;
    this->ui->cource->setText("Курс: " + QString::number(cource));

    this->ui->nameLabel->setText(QString("%1%2 %3 %4")
                                 .arg(cource)
                                 .arg(this->ui->numberEdit->text())
                                 .arg(this->specModel->record(this->ui->specBox->currentIndex()).value(1).toString())
                                 .arg(this->ui->dateEdit->date().toString("yy"))
                                 );
}
