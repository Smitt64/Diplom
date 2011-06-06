#include "textbrowser.h"
#include <qdebug.h>
#include <qpixmap.h>
#include <qstyleoption.h>
#include <qpainter.h>

TextBrowser::TextBrowser(FSHANDLE *handle, QWidget *parent) :
    QTextBrowser(parent),
    hHandle(handle)
{
}

QVariant TextBrowser::loadResource(int type, const QUrl &name)
{
    if(this->hHandle->file.isOpen())
        this->hHandle->file.close();

    FileSystem::getInst()->fsOpen(this->hHandle);
    if(!FileSystem::getInst()->fsHasFile(name.toString(), this->hHandle))
    {
#ifndef DEBUG_MSG
        qDebug() << "File " << name.toString() << " not found";
#endif
        FileSystem::getInst()->fsClose(this->hHandle);
        return QTextBrowser::loadResource(type, name);;
    }

#ifndef DEBUG_MSG
    qDebug() << "Has file";
#endif

    QByteArray data = FileSystem::getInst()->fsGetFile(name.toString(), this->hHandle);
    FileSystem::getInst()->fsClose(this->hHandle);
    return QVariant::fromValue(data);
}

void TextBrowser::setSource(const QUrl &name)
{
    if(this->hHandle->file.isOpen())
        this->hHandle->file.close();

    FileSystem::getInst()->fsOpen(this->hHandle);
    if(!FileSystem::getInst()->fsHasFile(name.toString(), this->hHandle))
    {
        FileSystem::getInst()->fsClose(this->hHandle);
        return;
    }

    this->setHtml(FileSystem::getInst()->fsGetFile(name.toString(), this->hHandle));
    FileSystem::getInst()->fsClose(this->hHandle);
}
