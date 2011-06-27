#include "texteditor.h"
#include "mainwindow.h"
#include <qtoolbar.h>
#include <qcolordialog.h>
#include <qfiledialog.h>
#include <qinputdialog.h>
#include "../filesystem/filesystem.h"
#include "../questpanel/questpanel.h"
#include <QTextDocumentWriter>
#include <qtextcodec.h>
#include "texteditwidget.h"
#include <qbuffer.h>
#include <qurl.h>
#include <qlabel.h>
#include "ui_texteditor.h"

TextEditView::TextEditView(QuestPanel *value, QWidget *parent) :
    QMainWindow(parent),
    panel(value)
{
    textEdit = new TextEditWidget(value);

    fontSizeBox = new QComboBox;
    fontSizeBox->addItems(QStringList()
                          << "9"
                          << "10"
                          << "11"
                          << "12"
                          << "14"
                          << "16"
                          << "18"
                          << "20"
                          << "22"
                          << "24"
                          << "26"
                          << "28"
                          << "36"
                          << "48"
                          << "72");
    fontBox = new QFontComboBox;

    setCentralWidget(textEdit);
    formating = addToolBar(tr("Форматирование"));
    formating->addWidget(fontSizeBox);
    formating->addWidget(fontBox);
    formating->addSeparator();

    actionBold = formating->addAction(QIcon(MainWindow::pixFile("interface\\icons\\boldhs.png")), tr("Жырный"));
    actionItalic = formating->addAction(QIcon(MainWindow::pixFile("interface\\icons\\ItalicHS.png")), tr("Курсив"));
    formating->addSeparator();
    actionFontColor = formating->addAction(tr("Цвет текста"));
}

void TextEditView::updateColor()
{

}

////////////////////////////////////////////////////////////////

TextEditor::TextEditor(QuestPanel *pan, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextEditor),
    panel(pan)
{
    ui->setupUi(this);

    this->setLayout(this->ui->mainlLayout);
    textEdit = new TextEditWidget(pan);
    textEdit->setAcceptRichText(true);
    this->ui->mainlLayout->insertWidget(2, new TextEditView(pan));

    /*this->ui->boldlButton->setIcon(QIcon(MainWindow::pixFile("interface\\icons\\boldhs.png")));
    this->ui->italiclButton->setIcon(QIcon(MainWindow::pixFile("interface\\icons\\ItalicHS.png")));
    this->ui->colorButton->setIcon(QIcon(MainWindow::pixFile("interface\\icons\\Color_fontHS.png")));

    this->ui->leftButton->setIcon(QIcon(MainWindow::pixFile("interface\\icons\\AlignTableCellMiddleLeftJustHS.PNG")));
    this->ui->centerButton->setIcon(QIcon(MainWindow::pixFile("interface\\icons\\AlignTableCellMiddleCenterHS.png")));
    this->ui->rightButton->setIcon(QIcon(MainWindow::pixFile("interface\\icons\\AlignTableCellMiddleRightHS.png")));

    this->ui->imageButton->setIcon(QIcon(MainWindow::pixFile("interface\\icons\\InsertPictureHS.png")));

    fontChanged(this->textEdit->font());
    alignmentChanged(this->textEdit->alignment());

    this->textEdit->setFocus();

    this->ui->comboStyle->addItem("Standard");
    this->ui->comboStyle->addItem("Bullet List (Disc)");
    this->ui->comboStyle->addItem("Bullet List (Circle)");
    this->ui->comboStyle->addItem("Bullet List (Square)");
    this->ui->comboStyle->addItem("Ordered List (Decimal)");
    this->ui->comboStyle->addItem("Ordered List (Alpha lower)");
    this->ui->comboStyle->addItem("Ordered List (Alpha upper)");
    this->ui->comboStyle->addItem("Ordered List (Roman lower)");
    this->ui->comboStyle->addItem("Ordered List (Roman upper)");

    connect(this->ui->boldlButton, SIGNAL(clicked()), this, SLOT(textBold()));
    connect(this->ui->italiclButton, SIGNAL(clicked()), this, SLOT(textItalic()));
    connect(this->ui->fontComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(textFamily(QString)));
    connect(this->ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(textSize(QString)));
    connect(this->ui->colorButton, SIGNAL(clicked()), this, SLOT(textColor()));
    connect(this->ui->leftButton, SIGNAL(clicked()), this, SLOT(textLeft()));
    connect(this->ui->centerButton, SIGNAL(clicked()), this, SLOT(textCenter()));
    connect(this->ui->rightButton, SIGNAL(clicked()), this, SLOT(textRight()));
    connect(this->ui->imageButton, SIGNAL(clicked()), this, SLOT(insertImage()));

    connect(this->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
    connect(this->textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChanged(QTextCharFormat)));

    connect(this->ui->comboStyle, SIGNAL(activated(int)), this, SLOT(textStyle(int)));*/
}

TextEditor::~TextEditor()
{
    delete ui;
}

void TextEditor::changeEvent(QEvent *e)
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

/*void TextEditor::loadResources()
{
    for(int i = 0; i < this->panel->resources.count(); i++)
        this->textEdit->document()->addResource(QTextDocument::ImageResource,
                                                    QUrl(this->panel->resources[i]),
                                                    this->panel->getPixResource(this->panel->resources[i]));
}*/

QString TextEditor::getPlainText()
{
    return this->textEdit->toPlainText();
}

void TextEditor::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(this->ui->boldlButton->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void TextEditor::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(this->ui->italiclButton->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void TextEditor::textFamily(QString f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void TextEditor::textSize(QString p)
{
    float pointSize = p.toFloat();
    if (p.toFloat() > 0)
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void TextEditor::textColor()
{
    QColor col = QColorDialog::getColor(this->textEdit->textColor(), this);
    if(!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
}

void TextEditor::mergeFormatOnWordOrSelection(QTextCharFormat format)
{
    QTextCursor cursor = this->textEdit->textCursor();
    if(!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    this->textEdit->mergeCurrentCharFormat(format);
}

void TextEditor::cursorPositionChanged()
{
    alignmentChanged(this->textEdit->alignment());
}

void TextEditor::alignmentChanged(Qt::Alignment a)
{
    this->ui->leftButton->setChecked(false);
    this->ui->centerButton->setChecked(false);

    if(a & Qt::AlignLeft)
        this->ui->leftButton->setChecked(true);
    else if(a & Qt::AlignHCenter)
        this->ui->centerButton->setChecked(true);
    else if (a & Qt::AlignRight)
        this->ui->rightButton->setChecked(true);
    /*else if (a & Qt::AlignJustify)
        actionAlignJustify->setChecked(true);*/
 }

void TextEditor::textLeft()
{
    this->textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    alignmentChanged(this->textEdit->alignment());
}

void TextEditor::textCenter()
{
    this->textEdit->setAlignment(Qt::AlignHCenter);
    alignmentChanged(this->textEdit->alignment());
}

void TextEditor::textRight()
{
    this->textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    alignmentChanged(this->textEdit->alignment());
}

void TextEditor::insertImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть изображение..."));

    if(!fileName.isEmpty())
    {
        QPixmap map(fileName);
        QFile file(fileName);

        QString f = FileSystem::getInst()->fsGetFName(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            if(!this->panel->hasResource("resources\\" + f))
            {
                QByteArray d = file.read(file.size());
                this->panel->addResource("resources\\" + f, d);
                this->textEdit->document()->addResource(QTextDocument::ImageResource,
                                                            QUrl("resources\\" + f),
                                                            QPixmap(fileName));
                QTextCursor cursor = this->textEdit->textCursor();
                QTextImageFormat imageFormat;
                imageFormat.setName("resources\\" + f);
                cursor.insertImage(imageFormat);
            }
            else
            {
                QMessageBox msg(this);
                QPushButton *insert = msg.addButton(tr("Вставить существующий"), QMessageBox::ActionRole);
                QPushButton *rename = msg.addButton(tr("Переименовать"), QMessageBox::ActionRole);
                QPushButton *cancel = msg.addButton(QMessageBox::Cancel);

                msg.setWindowTitle(tr("Совпадение ресурсов"));
                msg.setText(tr("Файл '%1' уже существует в тесте.").arg(f));
                msg.setInformativeText(tr("Выберите действие:"));

                msg.exec();

                if((QPushButton*)msg.clickedButton() == insert)
                {
                    QTextCursor cursor = this->textEdit->textCursor();
                    QTextImageFormat imageFormat;
                    imageFormat.setName("resources\\" + f);
                    cursor.insertImage(imageFormat);
                }
                else if((QPushButton*)msg.clickedButton() == rename)
                {
                    bool ok;
                    f = "";
                    f = QInputDialog::getText(this, tr("Добавление ресурса."),
                                              tr("Введите новое имя:"), QLineEdit::Normal,
                                              "", &ok);

                    if(ok && !f.isEmpty())
                    {
                        QTextCursor cursor = this->textEdit->textCursor();
                        QTextImageFormat imageFormat;
                        imageFormat.setName("resources\\" + f);
                        cursor.insertImage(imageFormat);
                    }
                }
            }
        }
    }
}

void TextEditor::fontChanged(QFont f)
{
    this->ui->fontComboBox->setCurrentIndex(this->ui->fontComboBox->findText(QFontInfo(f).family()));
    this->ui->comboBox->setCurrentIndex(this->ui->comboBox->findText(QString::number(f.pointSize())));
    this->ui->boldlButton->setChecked(f.bold());
    this->ui->italiclButton->setChecked(f.italic());

    //actionTextUnderline->setChecked(f.underline());
}

void TextEditor::currentCharFormatChanged(QTextCharFormat format)
{
    fontChanged(format.font());
}

QByteArray TextEditor::getHTMLData()
{
    QByteArray data;
    QBuffer buf(&data);

    QTextDocumentWriter writer(&buf, "HTML");
    writer.setCodec(QTextCodec::codecForName("UTF-8"));
    writer.write(this->textEdit->document());

    return data;
}

QString TextEditor::getHTML()
{
    return this->textEdit->toHtml();
}

void TextEditor::setHtml(QString html)
{
    this->textEdit->setHtml(html);
}

void TextEditor::setDocument(QTextDocument *document)
{
    this->textEdit->setDocument(document);
}

void TextEditor::textStyle(int styleIndex)
{
    QTextCursor cursor = this->textEdit->textCursor();

    if(styleIndex != 0)
    {
        QTextListFormat::Style style = QTextListFormat::ListDisc;

        switch(styleIndex)
        {
        default:
        case 1:
            style = QTextListFormat::ListDisc;
            break;
        case 2:
            style = QTextListFormat::ListCircle;
            break;
        case 3:
            style = QTextListFormat::ListSquare;
            break;
        case 4:
            style = QTextListFormat::ListDecimal;
            break;
        case 5:
            style = QTextListFormat::ListLowerAlpha;
            break;
        case 6:
            style = QTextListFormat::ListUpperAlpha;
            break;
        case 7:
            style = QTextListFormat::ListLowerRoman;
            break;
        case 8:
            style = QTextListFormat::ListUpperRoman;
            break;
        }

        cursor.beginEditBlock();

        QTextBlockFormat blockFmt = cursor.blockFormat();

        QTextListFormat listFmt;

        if(cursor.currentList())
            listFmt = cursor.currentList()->format();
        else
        {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }

        listFmt.setStyle(style);
        cursor.createList(listFmt);
        cursor.endEditBlock();
    }
    else
    {
        QTextBlockFormat bfmt = cursor.blockFormat();
        if(cursor.currentList())
        {
            cursor.currentList()->remove(cursor.block());
        }
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }
}
