#include "speciality.h"
#include "ui_speciality.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlDriver>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <addspeciality.h>
#include <QTableWidgetItem>
#include <addgroup.h>
#include <qsqlrecord.h>

Speciality::Speciality(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Speciality)
{
    ui->setupUi(this);

    this->setWindowTitle("Специальности и Группы");
    setAttribute(Qt::WA_DeleteOnClose);

    specModel = new QSqlQueryModel;
    QSqlQueryModel *model2 = new QSqlQueryModel;
    model2->setQuery(getFile(":/res/select_groups.sql"));

    updateSpecialityList();
    this->ui->tableView_2->setModel(model2);
    this->ui->tableView->setColumnWidth(1, 300);

    connect(this->ui->addSpec, SIGNAL(clicked()), this, SLOT(onAddSpeciality()));
    connect(this->ui->editSpec, SIGNAL(clicked()), this, SLOT(onUpdateSpeciality()));
    connect(this->ui->remSpec, SIGNAL(clicked()), this, SLOT(onRemoveSpeciality()));
    connect(this->ui->addGroup, SIGNAL(clicked()), this, SLOT(onAddGroup()));
}

Speciality::~Speciality()
{
    delete ui;
}

QString Speciality::getFile(QString fname)
{
    QString str;

    QFile file(fname);
    if(file.open(QIODevice::ReadOnly))
        str = file.readAll();
    file.close();

    return str;
}

void Speciality::onAddSpeciality()
{
    addSpeciality dlg(this);

    if(dlg.exec() == QDialog::Accepted)
    {
        QString caption = dlg.getCaption();
        int kod = dlg.getKode();

        qDebug() << kod << caption;
        if(!caption.isEmpty() && kod != 0)
        {
            QSqlQuery query;
            query.prepare("call updateSpeciality(0, :kod, :caption, :during, :pref, :num, 0, '');");
            query.bindValue(":kod", kod);
            query.bindValue(":caption", caption);
            query.bindValue(":during", dlg.getDuring());
            query.bindValue(":pref", dlg.getPrefix());
            query.bindValue(":num", dlg.getNumber());

            if(query.exec())
                updateSpecialityList();
            else
            {
                qDebug() << query.lastError();
                QMessageBox::critical(this, "Ошибка!", "Не удалось добавить специальность!");
            }
        }
        else
            QMessageBox::warning(this, "Добавление специальности.",
                                 "Не указано название или код специальности!");
    }
}

void Speciality::onUpdateSpeciality()
{
    QModelIndex index = this->ui->tableView->currentIndex();

    QString oCaption = specModel->record(index.row()).value(1).toString();
    int oKod = specModel->record(index.row()).value(0).toInt();

    addSpeciality dlg(this);
    dlg.setWindowTitle("Изменение специальности");
    dlg.setCaption(oCaption);
    dlg.setKod(oKod);
    dlg.setDuring(specModel->record(index.row()).value(2).toInt());

    QSqlQuery q;
    if(q.exec("select prefix, num from specialty where caption = '" + oCaption + "' and kod = " + QString::number(oKod)))
    {
        q.next();
        dlg.setPrefix(q.value(0).toString());
        dlg.setNumber(q.value(1).toInt());
    }
    else
        qDebug() << q.lastError();

    if(dlg.exec() == QDialog::Accepted)
    {
        QString caption = dlg.getCaption();
        int kod = dlg.getKode();
        if(!caption.isEmpty() && kod != 0)
        {
            QSqlQuery query;
            query.prepare("call updateSpeciality(1, :kod, :caption, :during, :pref, :num, :oldKod, :oldCaption);");
            query.bindValue(":kod", kod);
            query.bindValue(":caption", caption);
            query.bindValue(":during", dlg.getDuring());
            query.bindValue(":pref", dlg.getPrefix());
            query.bindValue(":num", dlg.getNumber());
            query.bindValue(":oldKod", oKod);
            query.bindValue(":oldCaption", oCaption);

            if(query.exec())
                updateSpecialityList();
            else
                QMessageBox::critical(this, "Ошибка!", "Не удалось обновить специальность!");
        }
    }
}

void Speciality::onRemoveSpeciality()
{
    QModelIndex index = this->ui->tableView->currentIndex();

    QString caption = specModel->record(index.row()).value(1).toString();
    int kod = specModel->record(index.row()).value(0).toInt();

    QMessageBox msg(this);
    msg.setWindowTitle("Удаление специальности");
    msg.setText("Удаление специальности '" + QString::number(kod) + "'?");
    msg.setInformativeText("Удалить специальность '" + caption + "'?");
    msg.setIcon(QMessageBox::Question);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::No);

    if(msg.exec() == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.prepare("call updateSpeciality(2, :kod, :caption, 0, '', 0, 0, '');");
        query.bindValue(":kod", kod);
        query.bindValue(":caption", caption);

        if(query.exec())
            updateSpecialityList();
        else
            QMessageBox::critical(this, "Ошибка!", "Не удалось удалить специальность!");
    }
}

void Speciality::updateSpecialityList()
{
    specModel->setQuery("SELECT specialty.kod as 'Код', specialty.caption as 'название', specialty.during as 'Срок обучения' FROM specialty");
    this->ui->tableView->setModel(specModel);
}

void Speciality::onAddGroup()
{
    AddGroup dlg(this);

    if(dlg.exec() == QDialog::Accepted)
    {

    }
}


