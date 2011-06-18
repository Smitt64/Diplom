#include "questionwidgt.h"
#include "ui_questionwidgt.h"

QuestionWidgt::QuestionWidgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuestionWidgt)
{
    ui->setupUi(this);
}

QuestionWidgt::~QuestionWidgt()
{
    delete ui;
}

void QuestionWidgt::changeEvent(QEvent *e)
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
