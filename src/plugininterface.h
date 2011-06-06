#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QMenu>

class PluginInterface
{
public:
    virtual QString menu() = 0;
    virtual QStringList windows() = 0;
    virtual QList<QAction*> commands() = 0;
    virtual ~PluginInterface() { };
    virtual QIcon getIcon(QString id) = 0;
    virtual QWidget *central(QString window, QWidget *parent = 0) = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "depot.Plugin.Interface/1.0")

#endif // PLUGININTERFACE_H
