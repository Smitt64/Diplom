#ifndef QUESTPANEL_H
#define QUESTPANEL_H

#include <QtCore>
#include <QWidget>
#include <qmap.h>
#include <qtextdocument.h>
#include <qtabbar.h>
#include <qtimer.h>
#include <qcompleter.h>
#include <qsequentialanimationgroup.h>
#include <qsqlrecord.h>
#include "../filesystem/filesystem.h"
#include "../Questions.h"
#include "../questpanel/answerwidget.h"
#include "../questpanel/questionwidgt.h"
#include "../questpanel/textbrowser.h"
#include "../testbuilderplugininterface.h"
#include "../questeditorinterface.h"
#include "../showpanelinterface.h"

#ifdef REDACTOR
    #include <qtreewidget.h>
#endif

class QDomElement;
class QDomDocument;

namespace Ui {
    class QuestPanel;
}

class QuestPanel : public QWidget {
    Q_OBJECT

#ifdef REDACTOR
    friend class TextEditor;
    friend class QuestEditor;
    friend class MainWindow;
    friend class TextEditWidget;
#endif
    friend class AnswerWidget;
    friend class QuestionWidgt;

public:
    enum TestState
    {
        SELECTESER = 0,
        SHOWQUESTION = 2,
        REPORTCREEN = 3,
    };

    enum OpenErr
    {
        SUCCEEDED,
        WARNINGS,
        FAIL,
    };

    QuestPanel(QString file, QWidget *parent = 0);
    ~QuestPanel();

    QStringList getTopicNames();
    void addTopic(QString topicName);
    void addQuestToTopic(QString topicName, QString questName);
    bool hasTopic(QString topic);
    QuetGroup *getTopic(QString name);
    QuetGroup *getTopic(int index);

#ifdef REDACTOR
    void removeTopic(QString name);
    void removeQuestion(QString name);
    void updateQuestion(QString name, QString newName, Question *quest);
    void removeFromTopic(QString topicName, QString questName);
    QByteArray writeXmlConfig();

    void renameQuestion(QString oldName, QString newName);
    QStringList getQuestGroups(QString value);

    void setTimeSettings(bool isOnTiem, int value);
    void setNavigationSettings(bool isShowTab, bool isEndButton, bool isNavigation);

    QStringList getTestTypes();
    QDomElement getSaveConfig(QString value, Question *quest, QDomDocument document);
    QuestEditorInterface *getEditor(QString questType);
#endif

    ShowPanelInterface *getViewPanel(QString name);
    TBPluginInterface *getPlugin(QString name);

    void addQuestion(Question *quest);
    Question *getQuestion(QString name);
    Question *getQuestion(int index);
    QStringList getQuestNames();

    //Составить список вопросов
    void buildQuestionList(QString param, int rnd_state, bool mixQuests, bool mixAnsw);
    void addUnsatisfactoryQuestions();

    QString getWindowCaption();
    //Количество тем
    int topicsCount();

    void addResource(QString name, QByteArray value);
    bool hasResource(QString name);
    QPixmap getPixResource(QString name);

    //Работа с файловой системой

    void showQuestion();
    void setUnsatisfactory(bool value);

    static QString getFile(QString fname);

    QString lastError();

public slots:
#ifdef REDACTOR
    void showRight(bool state);
    void setShowTab(bool state);
    void setShowEndButton(bool state);
    void setNextButton(bool state);
    void updateTabBar();
#endif

    void showStartScreen();
    void hideStartScreen();
    OpenErr updateTopics();
    void updateAnswerList();

    void showQuestion(QString name);

    void onTimer();
    void onUpdateIndicator();

    void onEndResizeAnimation();

signals:
    void nextButtonClick();
    void prevButtonClick();

    void startTestButton();
    void testEnd();

protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

    QRect grLabelDef, grBoxDef, stLabelDef, stBoxDef, startBtnDef;

    QList<QuetGroup*> topics;
    QMap<QString, Question*> questions;
    QuestionWidgt *questWidget;
    QTabBar *tabBar;
    QString windowCaption, tstParam;

    QAnimationGroup *startScreen;

    //Текущее окно
    int state;
    bool first;

    //Режим изменения таба
    bool tabState;

    //Параметры теста
    bool showTab;//Показывать строку навигации
    bool endButton;//Кнопка "Завершить"
    bool navigation;//Кнопка назад

    bool onTime;//На время
    int time;//время

    int questTime;
    int sec;

    //Выбранный студент
    int student;

    QString nextQuestion;
    QTimer *updateIndicator;

    //Сделать видимыми/невидимыми компоненты первого окна
    void setDefGeometryStart();

#ifndef REDACTOR
    void updateTabBar();
#endif
    void selectTab(QString name);

    //Идентификатор теста
    QString testIdenti;

    virtual void paintEvent(QPaintEvent *event);

protected slots:
    void nextEvent();
    void prevEvent();
    void onEndFirstScreenAnimation();
    void onEndHidStartScreenAnimation();
    void onSelectGroup(QString item);
    void onEndQuestAnimation();
    void onInfoButton();

    void hideQuestion();

    void showTestControlButtons();
    void questTabChanged(int index);
    void onEndShowTestInfo();



#ifndef REDACTOR
    void hideQuestWindow();
    void onEndButton();
    void endTesting();
#endif

private:
    Ui::QuestPanel *ui;
    bool firstQuestion;
    bool mixQuestions;
    bool mixAnswers;
    bool unsatisfactory;
    bool startLimiting;
    int maxStarts;
    int startPeriod;
    short scoreType;
    int subject;
    FSHANDLE questFile;
    QStringList resources;
    QStringList answerList;
    QStringList questList;

    QList<PTSCORE> testScores;

    QString lastErr;
    QString warnings;
    QString student_name;
    QString group;

    //Пароль на открытие для редактирования
    QString passwordOnOpen;
    //Пароль на старт тестирования
    QString passwordOnStart;
    //Пароль на завершение тестирования
    QString passwordOnEnd;

    QList<TBPluginInterface*> plugins;

#ifdef REDACTOR
    bool redactorTimer;
    int redactorTime;
    bool redactorShowTab;
    bool redactorNavigation;
    bool redactorEndButton;
    int counter;
    bool redactorShowRight;
#else
    int curQuestion;
#endif
};

#endif // QUESTPANEL_H
