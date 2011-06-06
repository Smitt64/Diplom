#include "puzzlequest.h"
#include "editor.h"
#include "showpanel.h"
#include "../../Questions.h"

QString PuzzleQuest::questType()
{
    return QString("Puzzle");
}

bool PuzzleQuest::isQuestionPlugin()
{
    return true;
}

QDomElement PuzzleQuest::writeQuestion(Question *question, QDomDocument document)
{
    Question *q = question;

    if(q)
    {
        QDomDocument doc = document;

        QDataStream stream(&q->settings(), QIODevice::ReadOnly);
        int w, h;

        stream >> w >> h;

        QDomElement element = doc.createElement("PuzzleQuestion");
        element.setAttribute("pixmap", q->getSrc());
        element.setAttribute("dx", w);
        element.setAttribute("dy", h);
        return element;
    }

    return QDomElement();
}

Question *PuzzleQuest::loadQuestion(QDomElement element, QString name)
{
    Question *quest = {0};

    if(!element.isNull() && element.tagName() == "PuzzleQuestion")
    {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);

        quest = new Question(name, element.attribute("pixmap", ""), QString("Puzzle"), 0);

        stream << element.attribute("dx", "5").toInt() << element.attribute("dy", "5").toInt();

        quest->setSettings(data);
    }

    return quest;
}

QuestEditorInterface *PuzzleQuest::editor()
{
    return new Editor;
}

ShowPanelInterface *PuzzleQuest::shown()
{
    return new ShowPanel;
}

int PuzzleQuest::userScore(Question *question)
{
    return 1;
}

int PuzzleQuest::maxUserScore()
{
    return 1;
}

Q_EXPORT_PLUGIN2(PuzzleQuestion, PuzzleQuest);
