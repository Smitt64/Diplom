#ifndef STUDENTS_H
#define STUDENTS_H

#include <QWidget>

namespace Ui {
    class Students;
}

class Students : public QWidget
{
    Q_OBJECT

public:
    explicit Students(QWidget *parent = 0);
    ~Students();

private slots:
    void onChangeFilter(QString filter);

private:
    Ui::Students *ui;
};

#endif // STUDENTS_H
