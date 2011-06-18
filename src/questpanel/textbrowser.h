#ifndef TEXTBROWSER_H
#define TEXTBROWSER_H

#include <QTextBrowser>
#include "../filesystem/filesystem.h"

class TextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    TextBrowser(FSHANDLE *handle, QWidget *parent = 0);

    virtual QVariant loadResource(int type, const QUrl &name);

signals:

public slots:
    virtual void setSource(const QUrl &name);

protected:
    FSHANDLE *hHandle;

    //virtual void paintEvent(QPaintEvent *event);

};

#endif // TEXTBROWSER_H
