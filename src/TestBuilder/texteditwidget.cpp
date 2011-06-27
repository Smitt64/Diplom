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

QVariant TextEditWidget::loadResource(int type, const QUrl &name)
{
    if(type == QTextDocument::ImageResource)
    {
        FileSystem::getInst()->fsOpen(&this->hHandle->questFile);
        if(FileSystem::getInst()->fsHasFile(name.path(), &this->hHandle->questFile))
        {
            QPixmap data;
            data.loadFromData(FileSystem::getInst()->fsGetFile(name.path(), &this->hHandle->questFile));
            FileSystem::getInst()->fsClose(&this->hHandle->questFile);
            return data;
        }
        else
            return QTextEdit::loadResource(type, name);
        FileSystem::getInst()->fsClose(&this->hHandle->questFile);
    }
    return QTextEdit::loadResource(type, name);
}
