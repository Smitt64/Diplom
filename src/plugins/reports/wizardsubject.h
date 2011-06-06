#ifndef WIZARDSUBJECT_H
#define WIZARDSUBJECT_H

#include <QWidget>

namespace Ui {
    class WizardSubject;
}

class QComboBox;

class WizardSubject : public QWidget
{
    Q_OBJECT

public:
    explicit WizardSubject(QWidget *parent = 0);
    ~WizardSubject();

    QComboBox *subject();
    QString getGroup();

private:
    Ui::WizardSubject *ui;

protected slots:
    void onUpdateSpec(QString value);
};

#endif // WIZARDSUBJECT_H
