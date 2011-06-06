#include <QtGui>
#include "puzzlewidget.h"

PuzzleWidget::PuzzleWidget(QWidget *parent) :
    QWidget(parent)
{
    setAcceptDrops(true);
    setMaximumSize(400, 400);
    setMinimumSize(400, 400);
}

void PuzzleWidget::clear()
 {
     pieceLocations.clear();
     piecePixmaps.clear();
     pieceRects.clear();
     highlightedRect = QRect();
     inPlace = 0;
     update();
 }

 void PuzzleWidget::dragEnterEvent(QDragEnterEvent *event)
 {
     if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
         event->accept();
     else
         event->ignore();
 }

 void PuzzleWidget::dragLeaveEvent(QDragLeaveEvent *event)
 {
     QRect updateRect = highlightedRect;
     highlightedRect = QRect();
     update(updateRect);
     event->accept();
 }

 void PuzzleWidget::dragMoveEvent(QDragMoveEvent *event)
 {
     QRect updateRect = highlightedRect.unite(targetSquare(event->pos()));

     if (event->mimeData()->hasFormat("image/x-puzzle-piece")
         && findPiece(targetSquare(event->pos())) == -1) {

         highlightedRect = targetSquare(event->pos());
         event->setDropAction(Qt::MoveAction);
         event->accept();
     } else {
         highlightedRect = QRect();
         event->ignore();
     }

     update(updateRect);
 }

 void PuzzleWidget::dropEvent(QDropEvent *event)
 {
     if (event->mimeData()->hasFormat("image/x-puzzle-piece")
         && findPiece(targetSquare(event->pos())) == -1) {

         QByteArray pieceData = event->mimeData()->data("image/x-puzzle-piece");
         QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
         QRect square = targetSquare(event->pos());
         QPixmap pixmap;
         QPoint location;
         dataStream >> pixmap >> location;

         pieceLocations.append(location);
         piecePixmaps.append(pixmap);
         pieceRects.append(square);

         highlightedRect = QRect();
         update(square);

         event->setDropAction(Qt::MoveAction);
         event->accept();

         if (location == QPoint(square.x()/80, square.y()/80)) {
             inPlace++;
             if (inPlace == 25)
                 emit puzzleCompleted();
         }
     } else {
         highlightedRect = QRect();
         event->ignore();
     }
 }

 int PuzzleWidget::findPiece(const QRect &pieceRect) const
 {
     for (int i = 0; i < pieceRects.size(); ++i) {
         if (pieceRect == pieceRects[i]) {
             return i;
         }
     }
     return -1;
 }

 void PuzzleWidget::mousePressEvent(QMouseEvent *event)
 {
     QRect square = targetSquare(event->pos());
     int found = findPiece(square);

     if (found == -1)
         return;

     QPoint location = pieceLocations[found];
     QPixmap pixmap = piecePixmaps[found];
     pieceLocations.removeAt(found);
     piecePixmaps.removeAt(found);
     pieceRects.removeAt(found);

     if (location == QPoint(square.x()/80, square.y()/80))
         inPlace--;

     update(square);

     QByteArray itemData;
     QDataStream dataStream(&itemData, QIODevice::WriteOnly);

     dataStream << pixmap << location;

     QMimeData *mimeData = new QMimeData;
     mimeData->setData("image/x-puzzle-piece", itemData);

     QDrag *drag = new QDrag(this);
     drag->setMimeData(mimeData);
     drag->setHotSpot(event->pos() - square.topLeft());
     drag->setPixmap(pixmap);

     if (!(drag->exec(Qt::MoveAction) == Qt::MoveAction)) {
         pieceLocations.insert(found, location);
         piecePixmaps.insert(found, pixmap);
         pieceRects.insert(found, square);
         update(targetSquare(event->pos()));

         if (location == QPoint(square.x()/80, square.y()/80))
             inPlace++;
     }
 }

 void PuzzleWidget::paintEvent(QPaintEvent *event)
 {
     QPainter painter;
     painter.begin(this);

     QRect sceneRect = QRect(0, 0, 398, 398);
     QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
     gradient.setColorAt(0, Qt::white);
     gradient.setColorAt(1, Qt::lightGray);
     painter.fillRect(sceneRect.intersect(sceneRect), gradient);
     painter.setBrush(Qt::NoBrush);
     painter.drawRect(sceneRect);

     if (highlightedRect.isValid()) {
         painter.setBrush(QColor("#ffcccc"));
         painter.setPen(Qt::NoPen);
         painter.drawRect(highlightedRect.adjusted(0, 0, -1, -1));
     }

     for (int i = 0; i < pieceRects.size(); ++i) {
         painter.drawPixmap(pieceRects[i], piecePixmaps[i]);
     }
     painter.end();
 }

 const QRect PuzzleWidget::targetSquare(const QPoint &position) const
 {
     return QRect(position.x()/80 * 80, position.y()/80 * 80, 80, 80);
 }
