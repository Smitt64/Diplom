#include "ConSettings.h"
#include "qsettings.h"
#include "ui_consettings.h"

ConSettings::ConSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConSettings)
{
    ui->setupUi(this);

    QSettings settings("Depot", "TestBuilder", this);
    this->ui->lineEdit->setText(settings.value("server", "127.0.0.1").toString());
    this->ui->spinBox->setValue(settings.value("port", "3306").toInt());

    this->setVisible(true);
}

ConSettings::~ConSettings()
{
    delete ui;
}

QString ConSettings::getServer()
{
    return this->ui->lineEdit->text();
}

int ConSettings::getPort()
{
    return this->ui->spinBox->value();
}
