#ifndef QUESTEDITORINTERFACE_H
#define QUESTEDITORINTERFACE_H

#include <QtGui>
#include <QtXml>
#include "Questions.h"

class QuestEditorInterface : public QWidget
{  
public:
    virtual ~QuestEditorInterface() {};
    virtual void getResource(int id, QString *name, QByteArray *data) = 0;
    virtual int getResCount() = 0;
    virtual Question *makeQuestion(int counter) = 0;
};

#endif // PUZZLEQUEST_H
