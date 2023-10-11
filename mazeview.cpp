#include "mazeview.h"
#include <QDebug>
#include <QMouseEvent>
#include "soundmanager.h"
#include <QRectF>

MazeView::MazeView(MazeGenerator *mg, QWidget *parent)
    : QGraphicsView(parent)
    , mg(mg)
{
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(QColor("#72751b")));
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    InitSprites();

    setScene(scene);

    Reload();

    connect(mg, &MazeGenerator::generated, this, &MazeView::Reload);
}

MazeView::~MazeView()
{
    delete sg;
}

void MazeView::InitSprites()
{
    sg = new SpriteGenerator(mg);
    QPixmap all_grass = QPixmap(":/sprites/grass.png");
    QPixmap all_stone = QPixmap(":/sprites/stone.png");

    grass = all_grass.copy(0,0, kBlockWidth, kBlockHeight);
    stone = all_stone.copy(16, 16, kBlockWidth, kBlockHeight);
}

void MazeView::InvertCellAt(QPointF pos, bool ignore_state)
{
    int x = pos.x() / kBlockWidth;
    int y = pos.y() / kBlockHeight;

    if (x < 0 || x >= mg->n_row || y < 0 || y >= mg->n_col) {
        return;
    }

    if (!ignore_state) {
        if (mg->maze[y][x]->is_wall == state) return;
    }

    state = !mg->maze[y][x]->is_wall;

    mg->maze[y][x]->is_wall = state;

    if (state) {
        SoundManager::Instance().WallSet();
    } else {
        SoundManager::Instance().WallClear();
    }

    Reload();

//    QGraphicsItem * gi = scene->itemAt(pos, QTransform());
//    CellView *cv;
//    if ((cv = dynamic_cast<CellView*>(gi))) {
//        cv->ChangeSprite(sg->GetSpriteForXY(x,y));


//    }

    this->update();
}

void MazeView::Reload()
{
    scene->clear();

    for (int i = 0; i < mg->n_row; ++i) {
        for (int j = 0; j < mg->n_col; ++j) {
//            auto cell = mg->maze[i][j];
            QGraphicsItem *block = new CellView(sg->GetSpriteForXY(j,i));

            scene->addItem(block);
            block->setPos(j*kBlockWidth, i*kBlockHeight);
//            auto r = scene->addRect(block->boundingRect(), QPen(), QBrush(QColor(Qt::red)));
//            r->setPos(block->pos());
        }
    }

//    for (int i = 0; i < mg->n_row; ++i) {
//        for (int j = 0; j < mg->n_col; ++j) {
//            auto cell = mg->maze[i][j];
//            QGraphicsItem *block;
//            if (cell->is_wall) {
//                block = new QGraphicsPixmapItem(stone);
//            } else {
//                block = new QGraphicsPixmapItem(grass);
//            }
//            scene->addItem(block);
//            block->setPos(j*kBlockWidth, i*kBlockHeight);
//        }
//    }
    scene->setSceneRect(QRectF());
    this->fitInView(0,0, mg->n_col*kBlockWidth, mg->n_row*kBlockHeight/*, Qt::KeepAspectRatio*/);
}


void MazeView::resizeEvent(QResizeEvent *event)
{
    this->fitInView(0,0, mg->n_col*kBlockWidth, mg->n_row*kBlockHeight/*, Qt::KeepAspectRatio*/);
    //    qDebug() << "aa" ;
}

void MazeView::mousePressEvent(QMouseEvent *event)
{
    auto pos = mapToScene(event->pos());

    InvertCellAt(pos);
//    Reload();
    //    event->ignore();
}

void MazeView::mouseMoveEvent(QMouseEvent *event)
{
    auto pos = mapToScene(event->pos());
    InvertCellAt(pos, false);
}

CellView::CellView(QPixmap *pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(*pixmap, parent)
    , pixmap(pixmap)
{
    setAcceptedMouseButtons(Qt::MouseButton::RightButton);

}

void CellView::ChangeSprite(QPixmap *pixmap)
{
    delete this->pixmap;
    this->pixmap = pixmap;
    this->setPixmap(*pixmap);
}

CellView::~CellView()
{
    delete pixmap;
}

void CellView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}
