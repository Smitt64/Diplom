#ifndef ADDGROUP_H
#define ADDGROUP_H

#include <QDialog>

namespace Ui {
    class AddGroup;
}

class QSqlQueryModel;

class AddGroup : public QDialog
{
    Q_OBJECT

public:
    explicit AddGroup(QWidget *parent = 0);
    ~AddGroup();

protected slots:
    void updateName(QString);

private:
    QSqlQueryModel *specModel;
    Ui::AddGroup *ui;
};

#endif // ADDGROUP_H
