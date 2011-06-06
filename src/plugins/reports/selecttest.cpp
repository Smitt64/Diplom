#include "selecttest.h"
#include <QComboBox>
#include <QSqlQuery>
#include <QDebug>
#include "globals.h"
#include "ncreport.h"
#include <QSqlError>
#include <QSqlResult>
#include <QSqlQueryModel>
#include <QMessageBox>
#include "ui_selecttest.h"

SelectTest::SelectTest(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SelectTest)
{
    ui->setupUi(this);

    QSqlQuery query;
    //query.prepare();

    connect(this->ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onChangeTest(QString)));
}

SelectTest::~SelectTest()
{
    delete ui;
}

void SelectTest::setSubject(QComboBox *value)
{
    this->subject = value;

    onUpdateSubject(this->subject->currentText());
    connect(subject, SIGNAL(currentIndexChanged(QString)), this, SLOT(onUpdateSubject(QString)));
}

void SelectTest::onUpdateSubject(QString value)
{
    this->ui->comboBox->clear();

    QSqlQuery query;
    query.prepare("SELECT DISTINCT "
                  "test_results.test_name, "
                  "test_results.test "
                  "FROM "
                  "test_results "
                  "INNER JOIN subject_specialty ON subject_specialty.`subject` = test_results.`subject` "
                  "INNER JOIN `subject` ON `subject`.id = subject_specialty.`subject` "
                  "INNER JOIN specialty ON specialty.id = subject_specialty.specialty "
                  "WHERE "
                  "((YEAR(now()) - YEAR(test_results.`data`)) <= specialty.during) AND "
                  "`subject`.`subject` = :param;");

    query.bindValue(":param", subject->currentText());

    if(query.exec())
    {
        while(query.next())
        {
            this->ui->comboBox->addItem(query.value(0).toString(), query.value(1));
        }
    }
    else
        qDebug() << query.lastError().text();

    /*QSqlQuery query;
    query.exec("call makeGroupScores('46 про 07', 'Информационная безопасность', '4fff137e9e737b54e8661009da623ac5');");

    NCReport *report = new NCReport(QString::null, 0);
    QFile f(":/tst_results");

    if(f.open(QIODevice::ReadOnly))
    {
        qDebug() << "report";
        QString str = f.readAll();
        report->setParseMode(NCReport::fromString);
        report->addParameter("gr", "46 про 07");
        report->addParameter("sub", "Информационная безопасность");
        report->addParameter("ident", "4fff137e9e737b54e8661009da623ac5");

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
    query.prepare("drop table jornay;");*/
}

void SelectTest::onChangeTest(QString value)
{
    this->ui->label_2->setText("Идентификатор: " + this->ui->comboBox->itemData(this->ui->comboBox->currentIndex()).toString());
}

QString SelectTest::getTestIdentity()
{
    return this->ui->comboBox->itemData(this->ui->comboBox->currentIndex()).toString();
}
