#ifndef MAZEVIEW_H
#define MAZEVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "mazegenerator.h"
#include "spritegenerator.h"

const int kBlockWidth  = 32;
const int kBlockHeight = 32;

class CellView;

class MazeView : public QGraphicsView
{
    Q_OBJECT
public:
    MazeView(MazeGenerator *mg, QWidget *parent = nullptr);
    ~MazeView();
private:
    void InitSprites();
    void InvertCellAt(QPointF pos, bool ignore_state = true);

public slots:
    void Reload();
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    MazeGenerator *mg;
    QGraphicsScene *scene;
    SpriteGenerator *sg;

    QPixmap grass, stone;

    bool state;

};

class CellView : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    CellView(QPixmap *pixmap, QGraphicsItem *parent = nullptr);
    void ChangeSprite(QPixmap *pixmap);
    ~CellView();

public slots:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPixmap *pixmap;
};

#endif // MAZEVIEW_H
