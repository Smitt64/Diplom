#ifndef QUESTIONS_H
#define QUESTIONS_H

#include <qbytearray>
#include <qlist.h>
#include <qmap.h>
#include <qstringlist.h>

#ifdef REDACTOR
#include <qtreewidget.h>
#endif

class Answer
{
public:
    Answer(QString name, QString source, bool right);

    QString getSource();
    QString getName();
    bool isRight();

    bool isChecked();
    void setChecked(bool state);

protected:
    QString src, name;
    bool fRight;
    bool fIsChecked;
};
//=====================================================
class Question
{
public:
    enum QuestType
    {
        VARIANT_QUESTION = 0,
        LIST_QUESTION = 1,
    };
    Question();
    Question(QString qName, QString textSrc,
             QString type = "0", int rightCount = 0);

    Question(QString qName, QString textSrc,
             QString right, QString type = "1");

    Question(QString qName, QString textSrc, QList<Answer*> a,
             QString type = "0", int rightCount = 0);

    QString getSrc();
    QString getRightAnswer();
    QString getName();
    QString getUserAnswer();

    QString getType();
    int getAnsvRCount();
    int getAnsvCount();

    int getUserScore();

    void setName(QString value);
    void addAnswer(Answer *ansv);
    void setRightAnswer(QString rightAnswer);
    void setTextAnswer(QString value);
    Answer *getAnswer(int index);
    //QString getUserAnswer();

    void mixAnswers(int sid);

    void setData(QByteArray value);
    void setSettings(QByteArray value);
    QByteArray data();
    QByteArray settings();

protected:
    QList<Answer*> Answers;
    int ansvRCount;
    QString qType;
    QString rightText;
    QString userAnswer;
    QString src, name;
    QByteArray otherData;
    QByteArray sett;
};
//=====================================================
class QuetGroup
{
public:
    QuetGroup(QString caption);

    QString getName();
    void addQuestion(QString name);
    void addQuestion(QString name, int index);
#ifdef REDACTOR
    void setTreeItem(QTreeWidgetItem *treeItem);
    QTreeWidgetItem *getTreeItem();
    void removeQuestion(QString name);
    void clear();
    void setIndex(QString name, int index);
    int getIndex(QString name);
    void setName(QString value);
#endif
    int questCount();
    QString getQuestName(int index);

    bool hasQuestion(QString name);

    void sortQuestions();
protected:
    QStringList questions;
    QMap<QString, int> indexes;
#ifdef REDACTOR
    QTreeWidgetItem *item;
#endif
    QString name;
};
//=====================================================

typedef struct TestScore
{
    QString text;
    int percent;
    int value;
}SCORE, *PTSCORE;

#endif
