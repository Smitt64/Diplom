#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include "../../questeditorinterface.h"

namespace Ui {
    class Editor;
}

class Editor : public QuestEditorInterface
{
    Q_OBJECT

public:
    Editor(QWidget *parent = 0);
    ~Editor();

    virtual int getResCount();
    virtual void getResource(int id, QString *name, QByteArray *data);
    virtual Question *makeQuestion(int counter);

private:
    Ui::Editor *ui;
    int res;
    QString fname;
    QPixmap map;

private slots:

private slots:
    void onOpenImage();
};

#endif // EDITOR_H
