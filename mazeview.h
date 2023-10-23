#ifndef MAZEVIEW_H
#define MAZEVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "mazegenerator.h"
#include "spritegenerator.h"
#include "pathfinder.h"

const int kBlockWidth  = 32;
const int kBlockHeight = 32;

const int kObjWidthOffset = 4;
const int kObjWidth = kBlockWidth - kObjWidthOffset*2;
const int kObjHeightOffset = 4;
const int kObjHeight = kBlockHeight - kObjHeightOffset*2;

const QColor kPathStartColor = QColor(Qt::GlobalColor::cyan);
const QColor kPathEndColor = QColor(Qt::GlobalColor::yellow);

enum class Mode {
    GENERATE,
    PATH_FINDER
};

class CellView;

class MazeView : public QGraphicsView
{
    Q_OBJECT
public:
    MazeView(MazeGenerator *mg, PathFinder *pf, QWidget *parent = nullptr);
    ~MazeView();

    void setCurStep(int step);
    void setMaxStep(int step);
private:
    void InitSprites();
    void InvertCellAt(QPointF pos, bool ignore_state = true);

public slots:
    void Reload();
    void IncStep(bool ignore_update = false);
    void DecStep(bool ignore_update = false);
    void ToStep(int step);
    void UpdateStep();
    void UpdatePathMode();
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void setMode(Mode mode);

signals:
    void cellClicked(QPoint pos, Qt::MouseButton button);
    void pathCellClicked(QPoint pos, Qt::MouseButton button);
    void sceneTimeChanged(qint64 time);

public:
    qint64 scene_time;
private:
    MazeGenerator *mg;
    QGraphicsScene *scene;
    SpriteGenerator *sg;
    PathFinder *pf;

    QPixmap grass, stone;

    bool state;
    int cur_step;
    int max_step;

    QList<Cell*> cur_fringe;

    Mode mode = Mode::GENERATE;
    QElapsedTimer timer;
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

class ConnectedLines : public QGraphicsItem
{
public:
    ConnectedLines(QList<QPointF> points);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QList<QPointF> points;
    QRectF b_rect;
};

#endif // MAZEVIEW_H
