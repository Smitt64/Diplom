#ifndef QUESTEDITOR_H
#define QUESTEDITOR_H

#include <QDialog>
#include <QSharedPointer>
#include "../filesystem/filesystem.h"
#include "../questpanel/questpanel.h"
#include "../questeditorinterface.h"

namespace Ui {
    class QuestEditor;
}

class QuestEditor : public QDialog {
    Q_OBJECT
public:
    QuestEditor(QuestPanel *panel, QWidget *parent = 0);
    ~QuestEditor();

    Question *getQuestion();
    QStringList getGroups();
    QString getOldQuestName();
    void setQuestion(Question *question);
    void setGroups(QStringList value);

    void checkGroup(QString group);

    void setQuestTypes(QStringList value);

    void saveResources();

protected:
    bool modify;
    Question *quest;
    void changeEvent(QEvent *e);
    QuestPanel *pan;
    QuestEditorInterface *editor;

protected slots:
    void onEditQuestText();
    void onAddAnswer();
    void onEditAnswer();
    void onRemoveAnswer();
    void onUpdateType(int index);

private:
    Ui::QuestEditor *ui;
};

#endif // QUESTEDITOR_H
