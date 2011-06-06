#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include "databasemanager.h"
#include "../../plugininterface.h"

class DataBaseManager : public QObject, PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

public:
    virtual QString menu();
    virtual QWidget *central(QString window, QWidget *parent = 0);
    virtual QStringList windows();
    virtual QList<QAction*> commands();
    virtual QIcon getIcon(QString id);

private slots:
    void buildSpecList();
    void onTransStudents();
};

#endif // DATABASEMANAGER_H
