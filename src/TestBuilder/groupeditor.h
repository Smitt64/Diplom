#ifndef GROUPEDITOR_H
#define GROUPEDITOR_H

#include <QDialog>
#include "../questpanel/questpanel.h"

namespace Ui {
    class GroupEditor;
}

class GroupEditor : public QDialog {
    Q_OBJECT
public:
    GroupEditor(QuestPanel *panel, QWidget *parent = 0);
    ~GroupEditor();

    void setGroup(QString name);
    QStringList getQuestions();
    QString getName();

protected:
    QuestPanel *questPanel;
    QString groupName;
    void changeEvent(QEvent *e);

private:
    Ui::GroupEditor *ui;
};

#endif // GROUPEDITOR_H
