#include "addspeciality.h"
#include "ui_addspeciality.h"

addSpeciality::addSpeciality(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addSpeciality)
{
    ui->setupUi(this);
}

addSpeciality::~addSpeciality()
{
    delete ui;
}

int addSpeciality::getKode()
{
    return this->ui->lineEdit->text().toInt();
}

QString addSpeciality::getCaption()
{
    return this->ui->lineEdit_2->text();
}

int addSpeciality::getDuring()
{
    return this->ui->spinBox->value();
}

void addSpeciality::setCaption(QString value)
{
    this->ui->lineEdit_2->setText(value);
}

void addSpeciality::setKod(int value)
{
    this->ui->lineEdit->setText(QString::number(value));
}

void addSpeciality::setDuring(int value)
{
    this->ui->spinBox->setValue(value);
}

QString addSpeciality::getPrefix()
{
    return this->ui->lineEdit_3->text();
}

void addSpeciality::setPrefix(QString value)
{
    this->ui->lineEdit_3->setText(value);
}

int addSpeciality::getNumber()
{
    return this->ui->lineEdit_4->text().toInt();
}

void addSpeciality::setNumber(int value)
{
    this->ui->lineEdit_4->setText(QString::number(value));
}
