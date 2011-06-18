#include <QtGui>
#include "databasemanager.h"
#include <QWidget>
#include <QPushButton>
#include "speciality.h"
#include <QSqlQuery>
#include "students.h"
#include <QMessageBox>

QWidget *DataBaseManager::central(QString window, QWidget *parent)
{
    if(window == "Специальности и Группы")
        return new Speciality(parent);
    if(window == "Студенты")
        return new Students(parent);

    return new QWidget();
}

QStringList DataBaseManager::windows()
{
    QStringList list;
    list << "Управление доступом" << "Специальности и Группы" << "Студенты";
    return list;
}

QString DataBaseManager::menu()
{
    return "База данных";
}

QIcon DataBaseManager::getIcon(QString id)
{
    QIcon icon;
    if(id == "Специальности и Группы")
    {
        return QIcon(":/res/groups.png");
    }
    else if(id == "Управление доступом")
    {
        return QIcon(":/res/secury.png");
    }
    return icon;
}

QList<QAction*> DataBaseManager::commands()
{
    QList<QAction*> list;
    QAction *translateStudents = new QAction("Перевести на следующий курс", this);
    QAction *sep = new QAction(this);
    QAction *createGroup = new QAction("Создать группу...", this);
    sep->setSeparator(true);

    connect(translateStudents, SIGNAL(triggered()), this, SLOT(onTransStudents()));
    list << sep << createGroup << translateStudents;
    return list;
}

void DataBaseManager::onTransStudents()
{
    QMessageBox msg;
    msg.setWindowTitle("Перевод");
    msg.setText("Перевести студентов на следующий курс?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    if(msg.exec() == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.prepare("call translateStudents();");

        if(!query.exec())
            QMessageBox::critical(NULL, "Ошибка!", "Не удалось превести студентов!");
    }
}

void DataBaseManager::buildSpecList()
{

}

Q_EXPORT_PLUGIN2(DBManager, DataBaseManager);
