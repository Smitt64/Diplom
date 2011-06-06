#ifndef SHOWPANELINTERFACE_H
#define SHOWPANELINTERFACE_H

#include <QtGui>
#include <QtXml>
#include "Questions.h"

class ShowPanelInterface : public QWidget
{
public:
    virtual ~ShowPanelInterface() {};
    virtual void setQuestion(Question *value) = 0;
    virtual void setResource(QString name, QByteArray resource) = 0;
    virtual void saveQuestionData(Question *value) = 0;
    virtual void loadQuestionData(Question *value) = 0;

signals:
    void needResource(QString name, ShowPanelInterface *ptr);
};

#endif
