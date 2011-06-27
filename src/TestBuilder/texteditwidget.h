#ifndef TEXTEDITWIDGET_H
#define TEXTEDITWIDGET_H

#include <QTextEdit>
#include "../filesystem/filesystem.h"
#include "../questpanel/questpanel.h"

class TextEditWidget : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEditWidget(QuestPanel *handele, QWidget *parent = 0);

signals:

public slots:

protected:
    virtual void insertFromMimeData(const QMimeData * source);
    virtual QVariant loadResource(int type, const QUrl &name);
    QuestPanel *hHandle;

};

#endif // TEXTEDITWIDGET_H
