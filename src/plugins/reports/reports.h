#ifndef REPORTS_H
#define REPORTS_H

#include <QObject>
#include "../../plugininterface.h"

class QWizardPage;
class QLabel;
class SelectTest;
class WizardSubject;

class Reports : public QObject, PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    virtual QString menu();
    virtual QWidget *central(QString window, QWidget *parent = 0);
    virtual QStringList windows();
    virtual QList<QAction*> commands();
    virtual QIcon getIcon(QString id);

protected slots:
    void showStudentList();
    void testResults();

    void tstResButtonClicked(int id);

private:
    QWizardPage *intro_page;
    QWizardPage *subject_page;
    SelectTest *seltst;
    QLabel *label;
    WizardSubject *ws;
};

#endif // REPORTS_H
