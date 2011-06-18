#include "reports.h"

#include <QtGui>
#include <QWidget>
#include <QPushButton>
#include <QSqlQuery>
#include <QMessageBox>
#include "globals.h"
#include "ncreport.h"
#include <QSqlRecord>
#include <QSqlField>
#include <QWizard>
#include "wizardsubject.h"
#include "selecttest.h"

QWidget *Reports::central(QString window, QWidget *parent)
{
    /*if(window == "Специальности и Группы")
        return new Speciality(parent);
    if(window == "Студенты")
        return new Students(parent);*/

    return new QWidget();
}

QStringList Reports::windows()
{
    QStringList list;
    //list << "Управление доступом" << "Специальности и Группы" << "Студенты";
    return list;
}

QString Reports::menu()
{
    return "Отчеты";
}

QIcon Reports::getIcon(QString id)
{
    QIcon icon;
    /*if(id == "Специальности и Группы")
    {
        return QIcon(":/res/groups.png");
    }
    else if(id == "Управление доступом")
    {
        return QIcon(":/res/secury.png");
    }*/
    return icon;
}

QList<QAction*> Reports::commands()
{
    QList<QAction*> list;
    QAction *students = new QAction("Список студентов...", this);
    QAction *tstRes = new QAction("Результат тестирования...", this);

    list << students << tstRes;

    connect(students, SIGNAL(triggered()), this, SLOT(showStudentList()));
    connect(tstRes, SIGNAL(triggered()), this, SLOT(testResults()));
    return list;
}

void Reports::showStudentList()
{
    NCReport *report = new NCReport(QString::null, 0);
    QFile f(":/stud_list");

    if(f.open(QIODevice::ReadOnly))
    {
        QString str = f.readAll();
        report->setParseMode(NCReport::fromString);
        report->setReportDef(str);
        report->setFileEncoding("Windows-1251");
        f.close();

        report->setDeleteReportAfterPreview(true);
        report->setPreviewIsMaximized(false);
        report->setOutput(NCReport::Preview);
        report->runReport();

        bool error = report->wasError();
        QString err = report->ErrorMsg();

        if (error)
            QMessageBox::information(0, "Report error", err);
    }
}

void Reports::testResults()
{
    QWizard wizard;
    wizard.setWizardStyle(QWizard::ModernStyle);
    wizard.setWindowTitle("Результат тестирования");

    intro_page = new QWizardPage;
    subject_page = new QWizardPage;
    seltst = new SelectTest;

    label = new QLabel(intro_page);
    label->setText("Настройка отчета о пройденом тетировании...");

    ws = new WizardSubject(subject_page);
    seltst->setSubject(ws->subject());

    wizard.addPage(intro_page);
    wizard.addPage(subject_page);
    wizard.addPage(seltst);

    //connect(&wizard, SIGNAL(accepted()));
    if(wizard.exec() == QDialog::Accepted)
    {
        QString group = ws->getGroup();
        QString subject = ws->subject()->currentText();
        QString tstID = seltst->getTestIdentity();

        NCReport *report = new NCReport(QString::null, 0);
        QFile f(":/tst_results");

        if(f.open(QIODevice::ReadOnly))
        {
            QSqlQuery query;
            query.exec(QString("call makeGroupScores('%1', '%2', '%3');")
                       .arg(group, subject, tstID));

            QString str = f.readAll();
            report->setParseMode(NCReport::fromString);
            //report->addParameter("gr", group);
            //report->addParameter("sub", subject);
            //report->addParameter("ident", tstID);

            report->setReportDef(str);
            report->setFileEncoding("Windows-1251");
            f.close();

            report->setDeleteReportAfterPreview(true);
            report->setPreviewIsMaximized(false);
            report->setOutput(NCReport::Preview);
            report->runReport();

            bool error = report->wasError();
            QString err = report->ErrorMsg();

            if (error)
                QMessageBox::information(0, "Report error", err);
            query.prepare("drop table jornay;");
            query.exec();
        }
    }
}

void Reports::tstResButtonClicked(int id)
{
    if(id = QWizard::FinishButton)
    {

    }
}

Q_EXPORT_PLUGIN2(Reports, Reports);
