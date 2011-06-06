#ifndef PIECESLIST_H
#define PIECESLIST_H

#include <QWidget>
#include <QListWidget>

class PiecesList : public QListWidget
{
    Q_OBJECT
public:
    PiecesList(QWidget *parent = 0);
    void addPiece(QPixmap pixmap, QPoint location);

    QPixmap getPixmap(QPoint location);
    void removeElement(QPoint location);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void startDrag(Qt::DropActions supportedActions);
};

#endif // PIECESLIST_H
