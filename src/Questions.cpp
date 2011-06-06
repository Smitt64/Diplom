#include "Questions.h"
#include <qdebug.h>

//=====================================================
QuetGroup::QuetGroup(QString caption) :
        name(caption)
{
}

QString QuetGroup::getName()
{
    return this->name;
}

void QuetGroup::addQuestion(QString name)
{
    this->questions.push_back(name);
    this->indexes.insert(name, questions.count());
}

void QuetGroup::addQuestion(QString name, int index)
{
    this->questions.push_back(name);
    this->indexes.insert(name, index);
}

int QuetGroup::QuetGroup::questCount()
{
    return this->questions.count();
}

QString QuetGroup::getQuestName(int index)
{
    return this->questions[index];
}

void QuetGroup::sortQuestions()
{
#ifndef DEBUG_MSG
    qDebug() << "Befor: " << questions;
#endif
    for(int i = 0; i < questions.count(); i++)
    {
        for(int j = 0; j < questions.count(); j++)
            if(indexes[questions[i]] < indexes[questions[j]])
                questions.swap(i, j);
    }
#ifndef DEBUG_MSG
    qDebug() << "After: " << questions;
#endif
}

#ifdef REDACTOR
void QuetGroup::clear()
{
    this->questions.clear();
}

void QuetGroup::setName(QString value)
{
    this->name = value;
}

void QuetGroup::setIndex(QString name, int index)
{
    if(indexes.contains(name))
    {
#ifndef DEBUG_MSG
        qDebug() << name << ": " << index;
#endif
        indexes[name] = index;
    }
}

int QuetGroup::getIndex(QString name)
{
    return this->indexes[name];
}

#endif

bool QuetGroup::hasQuestion(QString name)
{
    for(int i = 0; i < this->questions.count(); i++)
        if(this->questions[i] == name)
            return true;
    return false;
}

#ifdef REDACTOR
void QuetGroup::removeQuestion(QString name)
{
    for(int i = 0; i < this->questions.count(); i++)
        if(this->questions[i] == name)
            this->questions.removeAt(i);
}

void QuetGroup::setTreeItem(QTreeWidgetItem *treeItem)
{
    this->item = treeItem;
}

QTreeWidgetItem *QuetGroup::getTreeItem()
{
    return this->item;
}
#endif

//=====================================================
Answer::Answer(QString aName, QString source, bool right) :
        src(source), fRight(right), name(aName), fIsChecked(false)
{

}

QString Answer::getSource()
{
    return this->src;
}

QString Answer::getName()
{
    return this->name;
}

bool Answer::isRight()
{
    return this->fRight;
}

bool Answer::isChecked()
{
    return this->fIsChecked;
}

void Answer::setChecked(bool state)
{
    this->fIsChecked = state;
}

//=====================================================
Question::Question()
    : ansvRCount(-1),
      qType("unknown"),
      rightText(""),
      userAnswer(""),
      src("")
{

}

Question::Question(QString qName, QString textSrc, QString type, int rightCount) :
        src(textSrc), name(qName), ansvRCount(rightCount), qType(type),
        rightText(""), userAnswer("")
{

}

Question::Question(QString qName, QString textSrc, QList<Answer*> a,
         QString type, int rightCount) :
        src(textSrc), name(qName), ansvRCount(rightCount), qType(type),
        Answers(a), rightText(""), userAnswer("")
{

}

Question::Question(QString qName, QString textSrc, QString right, QString type) :
        name(qName), rightText(right), qType(type), src(textSrc), userAnswer("")
{

}

void Question::setData(QByteArray value)
{
    this->otherData = value;
}

void Question::setSettings(QByteArray value)
{
    this->sett = value;
}

QByteArray Question::settings()
{
    return this->sett;
}

QByteArray Question::data()
{
    return this->otherData;
}


void Question::setName(QString value)
{
    this->name = value;
}

QString Question::getRightAnswer()
{
    if(this->getType() == "1")
        return this->rightText;
    else if(this->getType() == "0")
    {
        QString text;
        for(int i = 0; i < this->Answers.count(); i++)
        {
            if(this->Answers[i]->isChecked())
            {
                if(this->Answers[i]->isRight())
                    text += QString("[%1] ").arg(i);
                else
                    text += QString("%1 ").arg(i);
            }
        }
        return text;
    }
    return "0";
}

QString Question::getSrc()
{
    return this->src;
}

QString Question::getName()
{
    return this->name;
}

QString Question::getUserAnswer()
{
    return this->userAnswer;
}

void Question::setTextAnswer(QString value)
{
    this->userAnswer = value;
}

QString Question::getType()
{
    return this->qType;
}

int Question::getAnsvRCount()
{
    return this->ansvRCount;
}

int Question::getAnsvCount()
{
    return this->Answers.count();
}

void Question::setRightAnswer(QString rightAnswer)
{
    this->rightText = rightAnswer;
}

int Question::getUserScore()
{
    int score = 0;

    //Вопрос свариантами ответов
    if(this->qType == 0)
    {
        for(int i = 0; i < this->Answers.count(); i++)
        {
            if(this->Answers[i]->isRight() == this->Answers[i]->isChecked() &&
               this->Answers[i]->isRight())
                score ++;
        }
    }
    else
    {
        if(this->rightText.toLower() == this->userAnswer.toLower())
            score ++;
    }

    return score;
}

Answer *Question::getAnswer(int index)
{
    return this->Answers[index];
}

void Question::addAnswer(Answer *ansv)
{
    this->Answers.push_back(ansv);
}

void Question::mixAnswers(int sid)
{
   for(int i = 0; i < sid; i++)
   {
       int id1 = qrand() % this->Answers.count();
       int id2 = qrand() % this->Answers.count();

       qSwap<Answer*>(this->Answers[id1], this->Answers[id2]);
   }
}
//=====================================================
