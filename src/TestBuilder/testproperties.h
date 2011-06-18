#ifndef TESTPROPERTIES_H
#define TESTPROPERTIES_H

#include <QDialog>
#include <qlistwidget.h>
#include <qlist.h>
#include "../Questions.h"
#include "texteditor.h"

namespace Ui {
    class TestProperties;
}

class TestProperties : public QDialog {
    Q_OBJECT
public:
    TestProperties(QWidget *panel, QWidget *parent = 0);
    TestProperties(QByteArray prop, QWidget *parent = 0);
    ~TestProperties();

    QByteArray getgroupsSql();
    QByteArray getAnswerList();

    QByteArray getEditorParam();
    QByteArray getComment();
    QByteArray getReportTemplate();

    QString getWindowCaption();

    void setAnswerList(QByteArray list);
    void setEditorParam(QByteArray param);
    void setReportTemplate(QByteArray temp);

    void setWindowCaption(QString value);

    void setPassWords(QString onOpen, QString onStart, QString onEnd);

    void setComment(QByteArray comment);

    void setMixParams(bool mQ, bool mA);
    void setMaxTestsStarts(int value);
    void setStartPeriod(int value);
    void setStartLimiting(bool value);

    void setSubject(int id);

    QString getOpenPass();
    QString getStartPass();
    QString getEndPass();

    bool isOnTime();
    int getTime();

    bool isShowTab();
    bool isEndButton();
    bool isNavigation();

    bool hasMixQuestions();
    bool hasMixAnswers();
    bool hasStartLimiting();

    int getMaxTestStarts();
    int getStartPeriod();

    int getSubject();

    QList<PTSCORE> getScores();

protected:
    void changeEvent(QEvent *e);
    void checkCource(QString name);
    void checkSpec(int data);

protected slots:
    void courceListUpdated(QListWidgetItem *item);
    void specialListUpdated(QListWidgetItem *item);

    void onAddAnswer();
    void onRemoveAnswer();
    void onLoadButton();
    void onEditComment();
    void onEditReport();

    void onChangeMarkType(int index);

    void onOpenPassGroup(bool on);
    void onClickReportLink(QString link);

private:
    QString groupSQL;
    TextEditor *editor;
    Ui::TestProperties *ui;
};

#endif // TESTPROPERTIES_H
