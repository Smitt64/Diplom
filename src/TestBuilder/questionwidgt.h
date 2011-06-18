#ifndef QUESTIONWIDGT_H
#define QUESTIONWIDGT_H

#include <QWidget>

namespace Ui {
    class QuestionWidgt;
}

class QuestionWidgt : public QWidget {
    Q_OBJECT
public:
    QuestionWidgt(QWidget *parent = 0);
    ~QuestionWidgt();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QuestionWidgt *ui;
};

#endif // QUESTIONWIDGT_H
