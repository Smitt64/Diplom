#ifndef TBPLUGININTERFACE_H
#define TBPLUGININTERFACE_H

#include <QMenu>
#include <QtCore>
#include <QtXml>
#include <QtGui>
#include "questeditorinterface.h"
#include "showpanelinterface.h"

class Question;

class TBPluginInterface
{
public:
    virtual ~TBPluginInterface() { };
    virtual QString questType() = 0;
    virtual QDomElement writeQuestion(Question *question, QDomDocument document) = 0;
    virtual QuestEditorInterface *editor() = 0;
    virtual ShowPanelInterface *shown() = 0;
    virtual Question *loadQuestion(QDomElement element, QString name) = 0;
    virtual int maxUserScore() = 0;
    virtual int userScore(Question *question) = 0;

    virtual bool isQuestionPlugin() = 0;
};

Q_DECLARE_INTERFACE(TBPluginInterface, "TestBuilder.Plugin.Interface/1.0")

#endif // PLUGININTERFACE_H
