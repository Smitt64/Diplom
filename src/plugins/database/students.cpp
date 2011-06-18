#include "students.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlDriver>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include "speciality.h"
#include <QStatusBar>
#include "ui_students.h"

Students::Students(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Students)
{
    ui->setupUi(this);

    QSqlQuery query;
    query.prepare(Speciality::getFile(":/res/select_groups.sql"));
    if(query.exec())
    {
        while(query.next())
            this->ui->groupsBox->addItem(query.value(0).toString());
    }
    onChangeFilter("");

    this->setWindowTitle("Управление студентами");

    connect(this->ui->surnameEdit, SIGNAL(textChanged(QString)), this, SLOT(onChangeFilter(QString)));
    connect(this->ui->nameEdit, SIGNAL(textChanged(QString)), this, SLOT(onChangeFilter(QString)));
    connect(this->ui->patroEdit, SIGNAL(textChanged(QString)), this, SLOT(onChangeFilter(QString)));
    connect(this->ui->groupsBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onChangeFilter(QString)));
}

void Students::onChangeFilter(QString /*filter*/)
{
    QSqlQuery query;
    query.prepare(Speciality::getFile(":/res/select_students.sql"));
    query.bindValue(":f", this->ui->surnameEdit->text() + "%");
    query.bindValue(":i", this->ui->nameEdit->text() + "%");
    query.bindValue(":o", this->ui->patroEdit->text() + "%");
    query.bindValue(":group",
                    (this->ui->groupsBox->currentIndex() == 0 ? "%" : this->ui->groupsBox->currentText()));

    if(query.exec())
    {
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery(query);
        this->ui->tableView->setModel(model);
    }
}

Students::~Students()
{
    delete ui;
}
