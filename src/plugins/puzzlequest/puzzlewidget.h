#ifndef PUZZLEWIDGET_H
#define PUZZLEWIDGET_H

#include <QWidget>
#include <QList>
#include <QPoint>
#include <QPixmap>
#include <QWidget>

class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;

class PuzzleWidget : public QWidget
{
    Q_OBJECT

    friend class ShowPanel;
public:
    PuzzleWidget(QWidget *parent = 0);
    void clear();

signals:
     void puzzleCompleted();

 protected:
     void dragEnterEvent(QDragEnterEvent *event);
     void dragLeaveEvent(QDragLeaveEvent *event);
     void dragMoveEvent(QDragMoveEvent *event);
     void dropEvent(QDropEvent *event);
     void mousePressEvent(QMouseEvent *event);
     void paintEvent(QPaintEvent *event);

 protected:
     int findPiece(const QRect &pieceRect) const;
     const QRect targetSquare(const QPoint &position) const;

     QList<QPixmap> piecePixmaps;
     QList<QRect> pieceRects;
     QList<QPoint> pieceLocations;
     QRect highlightedRect;
     int inPlace;

};

#endif // PUZZLEWIDGET_H
