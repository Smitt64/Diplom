#ifndef SELECTTEST_H
#define SELECTTEST_H

#include <QWizardPage>

namespace Ui {
    class SelectTest;
}

class QComboBox;

class SelectTest : public QWizardPage
{
    Q_OBJECT

public:
    explicit SelectTest(QWidget *parent = 0);
    ~SelectTest();

    void setSubject(QComboBox *value);
    QString getTestIdentity();
private:
    QComboBox *subject;
    Ui::SelectTest *ui;

private slots:
    void onUpdateSubject(QString);
    void onChangeTest(QString);
};

#endif // SELECTTEST_H
