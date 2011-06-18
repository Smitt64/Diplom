#include "settings.h"
#include "qsettings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    QSettings settings("Depot", "TestBuilder", this);
    this->ui->lineEdit->setText(settings.value("server", "127.0.0.1").toString());
    this->ui->spinBox->setValue(settings.value("port", "3306").toInt());
}

Settings::~Settings()
{
    delete ui;
}

QString Settings::getServer()
{
    return this->ui->lineEdit->text();
}

int Settings::getPort()
{
    return this->ui->spinBox->value();
}
