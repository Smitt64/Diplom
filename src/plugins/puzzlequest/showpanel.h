#ifndef SHOWPANEL_H
#define SHOWPANEL_H

#include <QWidget>
#include <QPixmap>
#include "../../showpanelinterface.h"

class PiecesList;
class PuzzleWidget;
class QListWidgetItem;

class ShowPanel : public ShowPanelInterface
{
    Q_OBJECT
public:
    ShowPanel(QWidget *parent = 0);
    virtual void setQuestion(Question *value);
    virtual void setResource(QString name, QByteArray resource);
    virtual void saveQuestionData(Question *value);
    virtual void loadQuestionData(Question *value);

signals:
    void needResource(QString name, ShowPanelInterface *ptr);

private slots:
    void setCompleted();

private:
    QPixmap puzzleImage;
    PiecesList *piecesList;
    PuzzleWidget *puzzleWidget;

};

#endif // SHOWPANEL_H
