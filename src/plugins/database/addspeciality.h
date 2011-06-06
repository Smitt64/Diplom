#ifndef ADDSPECIALITY_H
#define ADDSPECIALITY_H

#include <QDialog>

namespace Ui {
    class addSpeciality;
}

class addSpeciality : public QDialog
{
    Q_OBJECT

public:
    explicit addSpeciality(QWidget *parent = 0);
    ~addSpeciality();

    int getKode();
    QString getCaption();
    int getDuring();
    QString getPrefix();
    int getNumber();

    void setCaption(QString value);
    void setKod(int value);
    void setDuring(int value);
    void setPrefix(QString value);
    void setNumber(int value);


private:
    Ui::addSpeciality *ui;
};

#endif // ADDSPECIALITY_H
