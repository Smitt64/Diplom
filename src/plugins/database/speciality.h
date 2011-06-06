#ifndef SPECIALITY_H
#define SPECIALITY_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
    class Speciality;
}

class Speciality : public QWidget
{
    Q_OBJECT

public:
    explicit Speciality(QWidget *parent = 0);
    ~Speciality();

    static QString getFile(QString fname);

protected slots:
    void onAddSpeciality();
    void onUpdateSpeciality();
    void onRemoveSpeciality();

    void updateSpecialityList();
    void onAddGroup();

private:
    Ui::Speciality *ui;
    QSqlQueryModel *specModel;
};

#endif // SPECIALITY_H
