#ifndef ANSWERWIDGET_H
#define ANSWERWIDGET_H

#include <QWidget>
#include <../filesystem/filesystem.h>
#include <../questpanel/textbrowser.h>
#include <qabstractbutton.h>
#include <../Questions.h>

namespace Ui {
    class AnswerWidget;
}

class AnswerWidget : public QWidget {
    Q_OBJECT
public:
    enum Type
    {
        CHECKBOX = 0,
        RADIOBOX = 1,
    };
    //AnswerWidget(QString source, Type t, QWidget *parent = 0);
    AnswerWidget(FSHANDLE *handle, QString source, Type t, QWidget *parent = 0);
    ~AnswerWidget();

    void setRightAnswer(bool state);
#ifdef REDACTOR
    void setShowRight(bool state);
#endif

    void setControlWidth(int iWidth);
    void setupGeometry();

    void setAnswer(Answer *value);

    QAbstractButton *checkWidget();

    bool isChecked();
    void setChecked(bool state);
protected:
    virtual void paintEvent(QPaintEvent *event);
    void changeEvent(QEvent *e);
    FSHANDLE *hHandle;
    TextBrowser *textBrowser;

    Type type;
    int cWidth;
    QAbstractButton *check;

private slots:
    void onCheck(bool value);

private:
    Ui::AnswerWidget *ui;

    Answer *a;
    bool rightAnswer;
#ifdef REDACTOR
    bool showRight;
#endif
};

#endif // ANSWERWIDGET_H
