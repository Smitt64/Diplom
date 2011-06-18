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
    if(window == "������������� � ������")
        return new Speciality(parent);
    if(window == "��������")
        return new Students(parent);

    return new QWidget();
}

QStringList DataBaseManager::windows()
{
    QStringList list;
    list << "���������� ��������" << "������������� � ������" << "��������";
    return list;
}

QString DataBaseManager::menu()
{
    return "���� ������";
}

QIcon DataBaseManager::getIcon(QString id)
{
    QIcon icon;
    if(id == "������������� � ������")
    {
        return QIcon(":/res/groups.png");
    }
    else if(id == "���������� ��������")
    {
        return QIcon(":/res/secury.png");
    }
    return icon;
}

QList<QAction*> DataBaseManager::commands()
{
    QList<QAction*> list;
    QAction *translateStudents = new QAction("��������� �� ��������� ����", this);
    QAction *sep = new QAction(this);
    QAction *createGroup = new QAction("������� ������...", this);
    sep->setSeparator(true);

    connect(translateStudents, SIGNAL(triggered()), this, SLOT(onTransStudents()));
    list << sep << createGroup << translateStudents;
    return list;
}

void DataBaseManager::onTransStudents()
{
    QMessageBox msg;
    msg.setWindowTitle("�������");
    msg.setText("��������� ��������� �� ��������� ����?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    if(msg.exec() == QMessageBox::Yes)
    {
        QSqlQuery query;
        query.prepare("call translateStudents();");

        if(!query.exec())
            QMessageBox::critical(NULL, "������!", "�� ������� �������� ���������!");
    }
}

void DataBaseManager::buildSpecList()
{

}

Q_EXPORT_PLUGIN2(DBManager, DataBaseManager);
