#include "texteditwidget.h"
#include <qmimedata.h>
#include <qbuffer.h>
#include <qclipboard.h>
#include <qtextcursor.h>

TextEditWidget::TextEditWidget(QuestPanel *handele, QWidget *parent) :
    QTextEdit(parent),
    hHandle(handele)
{
    this->setAcceptDrops(true);
}

void TextEditWidget::insertFromMimeData(const QMimeData *source)
{
    if(source->hasImage())
    {
        QImage image = qvariant_cast<QImage>(source->imageData());

        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
        buffer.close();

        QString name = "resources\\mimeres" + QString::number(this->hHandle->counter) + ".png";
        FileSystem::getInst()->fsOpen(&this->hHandle->questFile);
        FileSystem::getInst()->fsAddTempFile(ba, name,
                                             &this->hHandle->questFile);
        this->hHandle->counter++;
        FileSystem::getInst()->fsClose(&this->hHandle->questFile);

        QTextImageFormat imageFormat;
        imageFormat.setName(name);
        this->textCursor().insertImage(image, name);
    }
    else
    {
        QTextEdit::insertFromMimeData(source);
    }
}
