#include "answerwidget.h"
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qpainter.h>
#include <qurl.h>
#include <qdebug.h>
#include "ui_answerwidget.h"

AnswerWidget::AnswerWidget(FSHANDLE *handle, QString source, Type t, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::AnswerWidget),
        type(t),
        hHandle(handle)
    #ifdef REDACTOR
        ,showRight(false),
        rightAnswer(false)
    #endif
{
    ui->setupUi(this);

    if(this->type == RADIOBOX)
        this->check = new QRadioButton(this);
    else if(this->type == CHECKBOX)
        this->check = new QCheckBox(this);

    this->textBrowser = new TextBrowser(handle, this);
    this->textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
    this->textBrowser->setGeometry(QRect(10, 10, 381, 121));
    this->textBrowser->setFrameShape(QFrame::NoFrame);
    this->textBrowser->setFrameShadow(QFrame::Plain);
    this->textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->textBrowser->setAutoFormatting(QTextEdit::AutoAll);
    this->textBrowser->setAcceptRichText(false);
    this->textBrowser->setTextInteractionFlags(Qt::NoTextInteraction);
    this->textBrowser->setOpenLinks(false);

    this->textBrowser->setSource(source);

    this->textBrowser->viewport()->setAutoFillBackground(false);

    this->setVisible(true);
}

AnswerWidget::~AnswerWidget()
{
    delete ui;
}

void AnswerWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AnswerWidget::setupGeometry()
{
    this->check->setGeometry(QRect(10, 0, 16, 16));

    QSize sz(this->parentWidget()->geometry().width(),
             this->parentWidget()->geometry().height());

    this->textBrowser->setMaximumSize(sz);
    this->textBrowser->setMinimumSize(sz);

    this->textBrowser->document()->adjustSize();
    sz = this->textBrowser->document()->size().toSize() + QSize(8, 8);
    this->textBrowser->document()->setPageSize(QSizeF(sz));

    if(sz.width() < this->parentWidget()->geometry().width() - 28)
        sz.setWidth(this->parentWidget()->geometry().width() - 28);


    this->textBrowser->setMaximumSize(sz);
    this->textBrowser->setMinimumSize(sz);

    this->textBrowser->setProperty("pos", QPoint(28, -4));


    this->adjustSize();
}


void AnswerWidget::setControlWidth(int iWidth)
{
    this->cWidth = iWidth;
}

bool AnswerWidget::isChecked()
{
    return this->check->isChecked();
}

QAbstractButton *AnswerWidget::checkWidget()
{
    return this->check;
}

void AnswerWidget::setChecked(bool state)
{
    this->check->setChecked(state);
}

void AnswerWidget::paintEvent(QPaintEvent */*event*/)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);

    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

#ifdef REDACTOR
    if(showRight && rightAnswer)
    {
        painter.setPen(QPen(Qt::darkRed, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawRoundedRect(QRect(5, 0, this->width() - 25, this->height() - 5), 5, 5);
    }
#endif
}

#ifdef REDACTOR
void AnswerWidget::setRightAnswer(bool value)
{
    rightAnswer = value;
}
void AnswerWidget::setShowRight(bool state)
{
    showRight = state;
}

#endif
