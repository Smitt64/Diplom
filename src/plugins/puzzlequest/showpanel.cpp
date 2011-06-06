#include "showpanel.h"
#include "pieceslist.h"
#include "puzzlewidget.h"

ShowPanel::ShowPanel(QWidget *parent)
{
    QHBoxLayout *frameLayout = new QHBoxLayout(this);

    piecesList = new PiecesList;
    puzzleWidget = new PuzzleWidget;

    connect(puzzleWidget, SIGNAL(puzzleCompleted()),
            this, SLOT(setCompleted()), Qt::QueuedConnection);

    frameLayout->addWidget(piecesList);
    frameLayout->addWidget(puzzleWidget);
}

void ShowPanel::setCompleted()
{

}

void ShowPanel::setResource(QString name, QByteArray resource)
{
    qDebug() << "ShowPanel::setResource";
    puzzleImage.loadFromData(resource);
}

void ShowPanel::setQuestion(Question *value)
{
    Question *quest = value;

    if(quest->getType() == "Puzzle")
    {
        emit needResource(quest->getSrc(), (ShowPanelInterface*)this);

        int size = qMin(puzzleImage.width(), puzzleImage.height());
        puzzleImage = puzzleImage.copy((puzzleImage.width() - size) / 2,
                                       (puzzleImage.height() - size) / 2, size, size).scaled(400,
                                       400, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        piecesList->clear();

        QDataStream stream(&quest->settings(), QIODevice::ReadOnly);

        int dx = 5, dy = 5;
        stream >> dx >> dy;

        for (int y = 0; y < dy; ++y)
        {
            for(int x = 0; x < dx; ++x)
            {
                QPixmap pieceImage = puzzleImage.copy(x * 80, y * 80, 80, 80);
                piecesList->addPiece(pieceImage, QPoint(x, y));
            }
        }

        qsrand(QCursor::pos().x() ^ QCursor::pos().y());

        for (int i = 0; i < piecesList->count(); ++i)
        {
            if(int(2.0 * qrand() / (RAND_MAX + 1.0)) == 1)
            {
                QListWidgetItem *item = piecesList->takeItem(i);
                piecesList->insertItem(0, item);
            }
        }

        puzzleWidget->clear();
    }
}

void ShowPanel::saveQuestionData(Question *value)
{
    //Question *quest = value;

    qDebug() << "void ShowPanel::saveQuestionData(Question *value)";
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    int count = this->puzzleWidget->pieceLocations.count();
    dataStream << (qint8)count;

    for(int i = 0; i < count; i++)
    {
        dataStream << this->puzzleWidget->pieceLocations.at(i) << this->puzzleWidget->pieceRects.at(i);
    }

    value->setData(itemData);
}

void ShowPanel::loadQuestionData(Question *value)
{
    QByteArray itemData;
    itemData = value->data();
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    qint8 count;

    dataStream >> count;

    for(int i = 0; i < count; i++)
    {
        QPoint point;
        QRect rect;

        dataStream >> point >> rect;

        QPixmap map = this->piecesList->getPixmap(point);

        if(!map.isNull())
        {
            this->piecesList->removeElement(point);

            this->puzzleWidget->pieceLocations.push_back(point);
            this->puzzleWidget->piecePixmaps.push_back(map);
            this->puzzleWidget->pieceRects.push_back(rect);
            this->update();
        }

        /*int index = this->puzzleWidget->pieceLocations.indexOf(point);

        if(index != -1)
        {

        }*/

    }
}
