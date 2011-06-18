#ifndef QUESTIONWIDGT_H
#define QUESTIONWIDGT_H

#include <QWidget>
#include <qlist.h>
#include <qbuttongroup.h>
#include "../questpanel/textbrowser.h"
#include "../Questions.h"
#include "../questpanel/answerwidget.h"
#include "../showpanelinterface.h"

namespace Ui {
    class QuestionWidgt;
}

class QuestionWidgt : public QWidget {
    Q_OBJECT
public:
    QuestionWidgt(QWidget *panelW, QWidget *parent = 0);
    ~QuestionWidgt();

    void setQuestion(Question *quest);

    void calculateSize();

    void writeAnswers();
    void updateAnswers();

#ifdef REDACTOR
    void setShowRight(bool state);
#endif

protected:
    virtual void resizeEvent(QResizeEvent *event);
    QWidget *panel;
    QButtonGroup *group;
    Question *question;
    QList<AnswerWidget*> answers;
    void changeEvent(QEvent *e);
    TextBrowser *questBrowser;
    ShowPanelInterface *plug;

protected slots:
    void loadResource(QString name, ShowPanelInterface *ptr);

private:
    Ui::QuestionWidgt *ui;
#ifdef REDACTOR
    bool showRight;
#endif
};

#endif // QUESTIONWIDGT_H
