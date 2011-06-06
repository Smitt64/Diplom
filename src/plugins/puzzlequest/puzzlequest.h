#ifndef PUZZLEQUEST_H
#define PUZZLEQUEST_H

#include <QObject>
#include "../../testbuilderplugininterface.h"

class Question;

class PuzzleQuest : public QObject, TBPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(TBPluginInterface)

public:
    virtual QString questType();
    virtual bool isQuestionPlugin();

    virtual QDomElement writeQuestion(Question *question, QDomDocument document);
    virtual Question *loadQuestion(QDomElement element, QString name);

    virtual QuestEditorInterface *editor();
    virtual ShowPanelInterface *shown();
    virtual int maxUserScore();
    virtual int userScore(Question *question);
};

#endif // PUZZLEQUEST_H
