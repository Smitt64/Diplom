#include "testproperties.h"
#include "mainwindow.h"
#include <qdom.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qcompleter.h>
#include <qsqlquery.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include "../questpanel/questpanel.h"
#include "ui_testproperties.h"

#define TO_QUESTPANEL(widget)((QuestPanel*)widget)

TestProperties::TestProperties(QWidget *panel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestProperties)
{
    ui->setupUi(this);

    courceListUpdated(NULL);

    QSqlQuery query;
    query.prepare(MainWindow::textFile("query\\select_subjects.sql"));
    if(query.exec())
    {
        QStringList list;
        while(query.next())
        {
            list.push_back(query.value(1).toString());
            this->ui->subjectBox->addItem(list.back(), query.value(0).toInt());
        }
        QCompleter *completer = new QCompleter(list);
        this->ui->subjectBox->setCompleter(completer);
    }
    else
    {
        QMessageBox::critical(this, "Ошибка!", "Не удалось оновить список предметов!\n" +
                              query.lastError().text());
    }

    query.prepare(MainWindow::textFile("query\\select_specialty.sql"));
    if(query.exec())
    {
        while(query.next())
        {
            QListWidgetItem *item = new QListWidgetItem;
            item->setText(query.value(1).toString());
            item->setCheckState(Qt::Checked);
            item->setData(Qt::UserRole, query.value(2).toInt());

            this->ui->specialtyList->addItem(item);
        }
    }
    else
    {
        QMessageBox::critical(this, "Ошибка!", "Не удалось оновить список специальностей!\n" +
                              query.lastError().text());
    }


    editor = new TextEditor(TO_QUESTPANEL(panel), this);
    courceListUpdated(NULL);

    connect(this->ui->courceList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(courceListUpdated(QListWidgetItem*)));
    connect(this->ui->specialtyList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(specialListUpdated(QListWidgetItem*)));

    connect(this->ui->addAnsBtn, SIGNAL(clicked()), this, SLOT(onAddAnswer()));
    connect(this->ui->removeAnsBtn, SIGNAL(clicked()), this, SLOT(onRemoveAnswer()));
    connect(this->ui->loadButton, SIGNAL(clicked()), this, SLOT(onLoadButton()));
    connect(this->ui->editComment, SIGNAL(clicked()), this, SLOT(onEditComment()));
    connect(this->ui->reportBtn, SIGNAL(clicked()), this, SLOT(onEditReport()));
    connect(this->ui->markBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeMarkType(int)));
    connect(this->ui->reportInfo, SIGNAL(linkActivated(QString)), this, SLOT(onClickReportLink(QString)));
}

TestProperties::TestProperties(QByteArray prop, QWidget *parent)
{
    ui->setupUi(this);
}

TestProperties::~TestProperties()
{
    delete ui;
}

void TestProperties::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TestProperties::courceListUpdated(QListWidgetItem *item)
{
    groupSQL = MainWindow::textFile("query\\select_groups.sql");

    bool first = true;
    for(int i = 0; i < this->ui->courceList->count(); i++)
    {
        if(this->ui->courceList->item(i)->checkState() == Qt::Checked)
        {
            if(first)
            {
                groupSQL += "where (groups.cource = " + QString::number(i + 1);
                first = false;
            }
            else
                groupSQL += " or groups.cource = " + QString::number(i + 1);
        }
    }
    groupSQL += ")";

    first = true;
    for(int i = 0; i < this->ui->specialtyList->count(); i++)
    {
        if(this->ui->specialtyList->item(i)->checkState() == Qt::Checked)
        {
            if(first)
            {
                groupSQL += "and (specialty.id = " +
                        QString::number(this->ui->specialtyList->item(i)->data(Qt::UserRole).toInt());
                first = false;

            }
            else
                groupSQL += " or specialty.id = " +
                        QString::number(this->ui->specialtyList->item(i)->data(Qt::UserRole).toInt());
        }

    }groupSQL += ")";

    QSqlQuery query;
    query.prepare(groupSQL);

    if(query.exec())
    {
        this->ui->groups->clear();
        while(query.next())
        {
            this->ui->groups->addItem(query.value(0).toString());
        }
    }
}

void TestProperties::specialListUpdated(QListWidgetItem *item)
{
    QString sql = MainWindow::textFile("query\\select_subjects.sql");

    if(this->ui->specialtyList->count() > 0)
    {
        sql += "and (";

        bool first = false;
        for(int i = 0; i < this->ui->specialtyList->count(); i++)
        {
            if(this->ui->specialtyList->item(i)->checkState() == Qt::Checked)
            {
                if(!first)
                {
                    sql += "specialty.id = " +
                           QString::number(this->ui->specialtyList->item(i)->data(Qt::UserRole).toInt());
                    first = true;
                }
                else
                    sql += " or specialty.id = " +
                           QString::number(this->ui->specialtyList->item(i)->data(Qt::UserRole).toInt());
            }
        }
        sql += ")";

        courceListUpdated(NULL);

        QSqlQuery query;
        query.prepare(sql);
        if(query.exec())
        {
            this->ui->subjectBox->clear();
            QStringList list;
            while(query.next())
            {
                list.push_back(query.value(1).toString());
                this->ui->subjectBox->addItem(query.value(1).toString(), query.value(0).toInt());
            }
            QCompleter *completer = new QCompleter(list);
            this->ui->subjectBox->setCompleter(completer);
        }
    }
    else
        QMessageBox::warning(this, "Ошибка!", "Тест должен принадлежать хотяб одной специальности!");
}

QByteArray TestProperties::getgroupsSql()
{
    QByteArray sql(groupSQL.toLocal8Bit());

    return sql;
}

void TestProperties::onAddAnswer()
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setText("Двойной клик для изменения...");
    this->ui->answerList->addItem(item);
}

void TestProperties::onRemoveAnswer()
{
    delete this->ui->answerList->currentItem();
}

QByteArray TestProperties::getAnswerList()
{
    QByteArray ansList;
    QTextStream stream(&ansList);
    stream.setCodec("Windows-1251");

    for(int i = 0; i < this->ui->answerList->count(); i++)
        stream << this->ui->answerList->item(i)->text() << "\n";

    return ansList;
}

void TestProperties::setAnswerList(QByteArray list)
{
    QTextStream stream(&list);
    stream.setCodec("Windows-1251");

    QString line;
    do
    {
        line = stream.readLine();
        if(!line.isNull())
        {
            QListWidgetItem *item = new QListWidgetItem;
            item->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setText(line);
            this->ui->answerList->addItem(item);
        }
    }while(!line.isNull());
}

QByteArray TestProperties::getEditorParam()
{
    QDomDocument doc;

    QDomNode xmlNode = doc.createProcessingInstruction("xml",
                                                       "version=\"1.0\" encoding=\"Windows-1251\"");
    doc.appendChild(xmlNode);
    QDomElement root = doc.createElement("editor");
    doc.appendChild(root);

    //Запись ограничений
    QDomElement restrictions = doc.createElement("restrictions");
    QDomElement cources = doc.createElement("cources");
    //Курс
    for(int i = 0; i < this->ui->courceList->count(); i++)
    {
        if(this->ui->courceList->item(i)->checkState() == Qt::Checked)
        {
            QDomElement value = doc.createElement("value");
            value.appendChild(doc.createTextNode(this->ui->courceList->item(i)->text()));
            cources.appendChild(value);
        }
    }

    //специальности
    QDomElement spec = doc.createElement("spec");
    for(int i = 0; i < this->ui->specialtyList->count(); i++)
    {
        if(this->ui->specialtyList->item(i)->checkState() == Qt::Checked)
        {
            QDomElement value = doc.createElement("value");
            value.appendChild(doc.createTextNode(this->ui->specialtyList->item(i)->data(Qt::UserRole).toString()));
            spec.appendChild(value);
        }
    }

    //Критерии оценки
    QDomElement levels = doc.createElement("levels");
    levels.setAttribute("type", this->ui->markBox->currentIndex());
    for(int i = 0; i < this->ui->tableWidget->rowCount(); i++)
    {
        QDomElement value = doc.createElement("level");
        value.setAttribute("score", this->ui->tableWidget->item(i, 1)->text());
        value.setAttribute("percent", this->ui->tableWidget->item(i, 2)->text());
        value.appendChild(doc.createTextNode(this->ui->tableWidget->item(i, 0)->text()));
        levels.appendChild(value);
    }

    restrictions.appendChild(cources);
    restrictions.appendChild(spec);
    restrictions.appendChild(levels);
    root.appendChild(restrictions);

    QByteArray toSave;
    QTextStream stream(&toSave);
    stream.setCodec("Windows-1251");
    doc.save(stream, 4);

    return toSave;
}

void TestProperties::checkSpec(int data)
{
    for(int i = 0; i < this->ui->specialtyList->count(); i++)
        if(this->ui->specialtyList->item(i)->data(Qt::UserRole).toInt() == data)
        {
            this->ui->specialtyList->item(i)->setCheckState(Qt::Checked);
            break;
        }
}

void TestProperties::checkCource(QString name)
{
    for(int i = 0; i < this->ui->courceList->count(); i++)
        if(this->ui->courceList->item(i)->text() == name)
        {
            this->ui->courceList->item(i)->setCheckState(Qt::Checked);
            break;
        }
}

void TestProperties::setEditorParam(QByteArray param)
{
    QDomDocument doc;
    QString err;
    if(doc.setContent(param, false, &err))
    {
        QDomElement docElement = doc.documentElement();
        QDomElement restrictions = docElement.firstChildElement("restrictions");

        QDomElement cources = restrictions.firstChildElement("cources");

        for(int i = 0; i < this->ui->courceList->count(); i++)
            this->ui->courceList->item(i)->setCheckState(Qt::Unchecked);

        for(QDomNode n = cources.firstChild(); !n.isNull(); n = n.nextSibling())
        {
            QDomElement elem = n.toElement();
            this->checkCource(elem.text());
        }

        this->courceListUpdated(NULL);

        for(int i = 0; i < this->ui->specialtyList->count(); i++)
            this->ui->specialtyList->item(i)->setCheckState(Qt::Unchecked);

        QDomElement spec = restrictions.firstChildElement("spec");
        if(spec.attribute("all", 0).toInt() == 1)
        {
            for(int i = 0; i < this->ui->specialtyList->count(); i++)
                this->ui->specialtyList->item(i)->setCheckState(Qt::Checked);
        }
        else
        {
            for(QDomNode n = spec.firstChild(); !n.isNull(); n = n.nextSibling())
            {
                QDomElement elem = n.toElement();
                qDebug() << "Специальность: " << elem.text().toInt();
                this->checkSpec(elem.text().toInt());
            }
        }

        this->specialListUpdated(NULL);

        QDomElement levels = restrictions.firstChildElement("levels");
        if(!levels.isNull())
        {
            int type = levels.attribute("type", "0").toInt();
            this->ui->markBox->setCurrentIndex(type);
            if(type == 1)
            {
                this->ui->tableWidget->setRowCount(2);
                this->ui->tableWidget->verticalHeaderItem(0)->setText("Зачет");
                this->ui->tableWidget->verticalHeaderItem(1)->setText("Не зачет");
            }
            else
            {
                this->ui->tableWidget->setRowCount(4);
                QStringList list;
                list << "Отлично(5)" << "Хорошо(4)" << "Уд(3)" << "Плохо(2)";
                this->ui->tableWidget->setVerticalHeaderLabels(list);
            }

            int i = 0;
            for(QDomNode n = levels.firstChild(); !n.isNull(); n = n.nextSibling())
            {
                QDomElement elem = n.toElement();
                this->ui->tableWidget->setItem(i, 0, new QTableWidgetItem(elem.text()));
                this->ui->tableWidget->setItem(i, 1, new QTableWidgetItem(elem.attribute("score", "0")));
                this->ui->tableWidget->setItem(i, 2, new QTableWidgetItem(elem.attribute("percent", "0")));

                i++;
            }

            for(int i = 0; i < this->ui->tableWidget->rowCount(); i++)
            {
                for(int j = 0; j < this->ui->tableWidget->columnCount(); j++)
                {
                    this->ui->tableWidget->item(i, j)->setFlags(Qt::ItemIsSelectable|
                                                                Qt::ItemIsEnabled|Qt::ItemIsEditable);
                    this->ui->tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                }
            }
        }
    }
    else
        QMessageBox::critical(this, "Ошибка разбора теста", err);
}

void TestProperties::onLoadButton()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть список ответов...", "\\",
                                                    "Текст (*.txt)");

    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            bool ok;
            QString content = file.readAll();
            QString text = QInputDialog::getText(this, "Параметры загрузки",
                                                      "Введите разделитель между словами:", QLineEdit::Normal,
                                                      "", &ok);
            if(ok && !text.isEmpty())
                content = content.replace(text, "\n");

            this->ui->answerList->clear();
            QTextStream stream(&content);
            QString line;
            do
            {
                line = stream.readLine();
                if(!line.isNull())
                {
                    this->ui->answerList->addItem(line);
                }
            }while(!line.isNull());
        }
        else
            QMessageBox::critical(this, "Ошибка открытия файла!",
                                  "Не удалось открыть файл '" + fileName + "'!");
    }
}

bool TestProperties::isOnTime()
{
    return this->ui->timeGroup->isChecked();
}

int TestProperties::getTime()
{
    return this->ui->timeSpin->value();
}

bool TestProperties::isShowTab()
{
    return this->ui->questTabCheck->isChecked();
}

bool TestProperties::isEndButton()
{
    return this->ui->checkBox_2->isChecked();
}

bool TestProperties::isNavigation()
{
    return this->ui->navigationCheck->isChecked();
}

void TestProperties::setComment(QByteArray comment)
{
    QTextStream stream(comment);
    stream.setCodec("Windows-1251");

    this->ui->commentEdit->setHtml(stream.readAll());
}

QByteArray TestProperties::getComment()
{
    QByteArray toSave;
    QTextStream stream(&toSave);
    stream.setCodec("Windows-1251");
    stream << this->ui->commentEdit->toHtml();

    return toSave;
}

void TestProperties::onEditComment()
{
    editor->setDocument(this->ui->commentEdit->document());

    editor->exec();
}

QByteArray TestProperties::getReportTemplate()
{
    QByteArray toSave;
    QTextStream stream(&toSave);
    stream.setCodec("Windows-1251");

    stream << this->ui->reportEdit->toHtml();

    return toSave;
}

void TestProperties::setReportTemplate(QByteArray temp)
{
    QTextStream stream(temp);
    stream.setCodec("Windows-1251");

    this->ui->reportEdit->setHtml(stream.readAll());
}

void TestProperties::onEditReport()
{
    editor->setDocument(this->ui->reportEdit->document());

    editor->exec();
}

QString TestProperties::getOpenPass()
{
    if(this->ui->openTestPassBox->isChecked())
        return this->ui->openPassEdit->text();
    return "";
}

QString TestProperties::getStartPass()
{
    if(this->ui->startTestPassBox->isChecked())
        return this->ui->startPassEdit->text();
    return "";
}

QString TestProperties::getEndPass()
{
    if(this->ui->endTestPassBox->isChecked())
        return this->ui->endPassEdit->text();
    return "";
}

void TestProperties::setPassWords(QString onOpen, QString onStart, QString onEnd)
{
    this->ui->openTestPassBox->setChecked(!onOpen.isEmpty());
    this->ui->openPassEdit->setText(onOpen);

    this->ui->startTestPassBox->setChecked(!onStart.isEmpty());
    this->ui->startPassEdit->setText(onStart);

    this->ui->endTestPassBox->setChecked(!onEnd.isEmpty());
    this->ui->endPassEdit->setText(onEnd);
}

void TestProperties::onOpenPassGroup(bool on)
{
}

QString TestProperties::getWindowCaption()
{
    return this->ui->captionEdit->text();
}

void TestProperties::setWindowCaption(QString value)
{
    this->ui->captionEdit->setText(value);
}

bool TestProperties::hasMixQuestions()
{
    return this->ui->mixQuestsCheck->isChecked();
}

bool TestProperties::hasMixAnswers()
{
    return this->ui->mixAnswCheck->isChecked();
}

int TestProperties::getMaxTestStarts()
{
    return this->ui->spinBox->value();
}

void TestProperties::setMixParams(bool mQ, bool mA)
{
    this->ui->mixQuestsCheck->setChecked(mQ);
    this->ui->mixAnswCheck->setChecked(mA);
}

void TestProperties::setMaxTestsStarts(int value)
{
    this->ui->spinBox->setValue(value);
}

int TestProperties::getStartPeriod()
{
    return this->ui->spinBox_2->value();
}

void TestProperties::setStartPeriod(int value)
{
    this->ui->spinBox_2->setValue(value);
}

bool TestProperties::hasStartLimiting()
{
    return this->ui->groupBox_8->isChecked();
}

void TestProperties::setStartLimiting(bool value)
{
    this->ui->groupBox_8->setChecked(value);
}

void TestProperties::onChangeMarkType(int index)
{
    if(index == 1)
    {
        this->ui->tableWidget->setRowCount(2);
        this->ui->tableWidget->verticalHeaderItem(0)->setText("Зачет");
        this->ui->tableWidget->verticalHeaderItem(1)->setText("Не зачет");

        this->ui->tableWidget->item(0, 0)->setText("Зачет");
        this->ui->tableWidget->item(0, 1)->setText("5");
        this->ui->tableWidget->item(0, 2)->setText("50");

        this->ui->tableWidget->item(1, 0)->setText("Не Зачет");
        this->ui->tableWidget->item(1, 1)->setText("2");
        this->ui->tableWidget->item(1, 2)->setText("0");
    }
    else
    {
        this->ui->tableWidget->setRowCount(4);
        QStringList list;
        list << "Отлично(5)" << "Хорошо(4)" << "Уд(3)" << "Плохо(2)";
        this->ui->tableWidget->setVerticalHeaderLabels(list);

        this->ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Отлично"));
        this->ui->tableWidget->setItem(0, 1, new QTableWidgetItem("5"));
        this->ui->tableWidget->setItem(0, 2, new QTableWidgetItem("85"));

        this->ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Хорошо"));
        this->ui->tableWidget->setItem(1, 1, new QTableWidgetItem("4"));
        this->ui->tableWidget->setItem(1, 2, new QTableWidgetItem("70"));

        this->ui->tableWidget->setItem(2, 0, new QTableWidgetItem("Удовлетворительно"));
        this->ui->tableWidget->setItem(2, 1, new QTableWidgetItem("3"));
        this->ui->tableWidget->setItem(2, 2, new QTableWidgetItem("50"));

        this->ui->tableWidget->setItem(3, 0, new QTableWidgetItem("Плохо"));
        this->ui->tableWidget->setItem(3, 1, new QTableWidgetItem("2"));
        this->ui->tableWidget->setItem(3, 2, new QTableWidgetItem("0"));
    }

    for(int i = 0; i < this->ui->tableWidget->rowCount(); i++)
    {
        for(int j = 0; j < this->ui->tableWidget->columnCount(); j++)
        {
            this->ui->tableWidget->item(i, j)->setFlags(Qt::ItemIsSelectable|
                                                        Qt::ItemIsEnabled|Qt::ItemIsEditable);
            this->ui->tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}

QList<PTSCORE> TestProperties::getScores()
{
    QList<PTSCORE> list;

    for(int i = 0; i < this->ui->tableWidget->rowCount(); i++)
    {
        PTSCORE buf = new SCORE;
        buf->text = this->ui->tableWidget->item(i, 0)->text();
        buf->value = this->ui->tableWidget->item(i, 1)->text().toInt();
        buf->percent = this->ui->tableWidget->item(i, 2)->text().toInt();

        list.push_back(buf);
    }

    return list;
}

int TestProperties::getSubject()
{
    return this->ui->subjectBox->itemData(this->ui->subjectBox->currentIndex()).toInt();
}

void TestProperties::setSubject(int id)
{
    if(id == -1)
        this->ui->subjectBox->setCurrentIndex(0);
    else
        for(int i = 0; i < this->ui->subjectBox->count(); i++)
            if(this->ui->subjectBox->itemData(i).toInt() == id)
            {
                this->ui->subjectBox->setCurrentIndex(i);
                break;
            }
}

void TestProperties::onClickReportLink(QString link)
{
    this->ui->reportEdit->insertPlainText(link);
}
