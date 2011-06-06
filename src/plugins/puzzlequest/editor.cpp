#include "editor.h"
#include <QFileDialog>
//#include "../../Questions.h"
#include "ui_editor.h"

Editor::Editor(QWidget *parent) :
    ui(new Ui::Editor),
    res(1)
{
    ui->setupUi(this);

    connect(this->ui->toolButton, SIGNAL(clicked()), this, SLOT(onOpenImage()));
    //connect(this->ui->pushButton, SIGNAL(clicked()), this, SLOT(onUpdateSize()));
}

Editor::~Editor()
{
    delete ui;
}

int Editor::getResCount()
{
    return res;
}

void Editor::getResource(int id, QString *name, QByteArray *data)
{
    if(id < this->getResCount())
    {
        QFileInfo inf(this->ui->lineEdit->text());

        fname = QString("resource\\puzzlequest\\" + inf.fileName());
        *name = fname;
        QBuffer buffer(data);
        buffer.open(QIODevice::WriteOnly);
        this->map.save(&buffer, inf.completeSuffix().toUpper().toLocal8Bit().data());
    }
}

void Editor::onOpenImage()
{
    QString fName = QFileDialog::getOpenFileName(this, "Открыть изображение...", "./",
                                                 "Images (*.png *.xpm *.jpg)");

    if(!fName.isEmpty())
    {
        QFileInfo inf(fName);

        this->ui->lineEdit->setText(fName);

        map = QPixmap(fName);
        this->fname = QString("resource\\puzzlequest\\" + inf.fileName());

        this->ui->view->setPixmap(map);
    }
}

Question *Editor::makeQuestion(int counter)
{
    Question *maked = new Question(QString("puzzle%1").arg(counter), fname, QString(""), QString("Puzzle"));

    QByteArray settings;
    QDataStream stream(&settings, QIODevice::WriteOnly);
    stream << this->ui->spinBox->value() <<  this->ui->spinBox_2->value();

    maked->setSettings(settings);

    return maked;
}
