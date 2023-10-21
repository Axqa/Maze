#include "mazeview.h"
#include <QDebug>
#include <QMouseEvent>
#include "soundmanager.h"
#include <QRectF>

MazeView::MazeView(MazeGenerator *mg, PathFinder *pf, QWidget *parent)
    : QGraphicsView(parent)
    , mg(mg)
    , pf(pf)
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

void MazeView::setCurStep(int step)
{
    cur_step = step;
}

void MazeView::setMaxStep(int step)
{
    max_step = step;
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
    if (cur_step != max_step) return;

    int x = pos.x() / kBlockWidth;
    int y = pos.y() / kBlockHeight;

    if (x < 0 || y >= mg->n_row || y < 0 || x >= mg->n_col) {
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
            QGraphicsItem *block = new CellView(sg->GetSpriteForXY(j,i));

            scene->addItem(block);
            block->setPos(j*kBlockWidth, i*kBlockHeight);
        }
    }

    scene->setSceneRect(QRectF());
    this->fitInView(0,0, mg->n_col*kBlockWidth, mg->n_row*kBlockHeight/*, Qt::KeepAspectRatio*/);
}

void MazeView::IncStep(bool ignore_update)
{
    if (cur_step >= max_step) return;
    auto next_el = mg->last_step = mg->last_step + 1;
    mg->maze_step[next_el->next->row][next_el->next->col] = false;
    cur_fringe = next_el->fringe;
    cur_step = cur_step + 1;

    if (ignore_update == false) {
        UpdateStep();
    }
}

void MazeView::DecStep(bool ignore_update)
{
    if (cur_step <= 0) return;
    if (cur_step == 1) {
    int r = 0;
    Q_UNUSED(r);
    //        cur_fringe = QList<Cell*>();

    }

    auto next_el = mg->last_step;
    mg->maze_step[next_el->next->row][next_el->next->col] = true;
    cur_fringe = next_el->fringe;
    cur_step = cur_step - 1;
    mg->last_step = next_el - 1;

    if (ignore_update == false) {
        UpdateStep();
    }
}

void MazeView::ToStep(int step)
{
    while (cur_step != step) {
        if (cur_step < step) {
            IncStep(true);
        } else {
            DecStep(true);
        }
    }

    UpdateStep();
}

void MazeView::UpdateStep()
{
    if (cur_step == max_step) {
        Reload();
        return;
    }

    scene->clear();

    for (int i = 0; i < mg->n_row; ++i) {
        for (int j = 0; j < mg->n_col; ++j) {
            QGraphicsItem *block = new CellView(mg->maze_step[i][j] ? sg->GetStoneSpriteForStep(j,i) : sg->GetGrassSprite(j,i));

            scene->addItem(block);
            block->setPos(j*kBlockWidth, i*kBlockHeight);
        }
    }


    if (cur_step != 0) {
        for (auto i = mg->last_step->fringe.begin(); i < mg->last_step->fringe.end(); i++) {
            auto pos = (*i)->pos();
                scene->addRect(pos.x() * kBlockWidth + 2, pos.y() * kBlockHeight + 2, kBlockWidth-4, kBlockHeight-4, QPen(), QBrush(QColor(Qt::yellow)));
        }
        auto pos = mg->last_step->next->pos();
        scene->addRect(pos.x() * kBlockWidth + 2, pos.y() * kBlockHeight + 2, kBlockWidth-4, kBlockHeight-4, QPen(), QBrush(QColor(Qt::blue)));

    }

    this->fitInView(0,0, mg->n_col*kBlockWidth, mg->n_row*kBlockHeight/*, Qt::KeepAspectRatio*/);
}

void MazeView::UpdatePathMode()
{
    Reload();


    auto start = pf->start_cell;
    auto end = pf->end_cell;

    if (true) {
        QList<QPointF> points;
        for (auto cell : pf->path) {
            points << cell->pos() * kBlockWidth;
        }

        auto item = new ConnectedLines(points);
        scene->addItem(item);
        item->setPos({kBlockWidth/2, kBlockHeight/2});

    }

    if (start) {
        scene->addEllipse(start->col*kBlockWidth + kObjWidthOffset,
                          start->row*kBlockHeight + kObjHeightOffset,
                          kObjWidth, kObjHeight,
                          QPen(),
                          QBrush(kPathStartColor));
    }

    if (end) {
        scene->addEllipse(end->col*kBlockWidth + kObjWidthOffset,
                          end->row*kBlockHeight + kObjHeightOffset,
                          kObjWidth, kObjHeight,
                          QPen(),
                          QBrush(kPathEndColor));

    }
    update();
}


void MazeView::resizeEvent(QResizeEvent *event)
{
    this->fitInView(0,0, mg->n_col*kBlockWidth, mg->n_row*kBlockHeight/*, Qt::KeepAspectRatio*/);
    //    qDebug() << "aa" ;
}

void MazeView::mousePressEvent(QMouseEvent *event)
{
    auto pos = mapToScene(event->pos());
    int x = pos.x() / kBlockWidth;
    int y = pos.y() / kBlockHeight;
    emit cellClicked({x,y}, event->button());

    switch (mode) {
    case Mode::GENERATE:

        InvertCellAt(pos);
        QGraphicsView::mousePressEvent(event);
        break;

    case Mode::PATH_FINDER:
        emit pathCellClicked({x,y}, event->button());
        break;

    default:
        break;
    }

}

void MazeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    mousePressEvent(event);
}

void MazeView::mouseMoveEvent(QMouseEvent *event)
{
    auto pos = mapToScene(event->pos());
    switch (mode) {
    case Mode::GENERATE:
        InvertCellAt(pos, false);
        break;

    case Mode::PATH_FINDER:
        break;

    default:
        break;
    }
}

void MazeView::setMode(Mode mode)
{
    this->mode = mode;

    switch (mode) {
    case Mode::GENERATE:
        Reload();
        break;

    case Mode::PATH_FINDER:
        pf->MakePath();
        UpdatePathMode();
        break;

    default:
        break;

    }
}

CellView::CellView(QPixmap *pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(*pixmap, parent)
    , pixmap(pixmap)
{
    setAcceptedMouseButtons(Qt::MouseButton::RightButton);
    setFlags(QGraphicsItem::ItemIsSelectable|
                      QGraphicsItem::ItemIsMovable);
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
//    qDebug() << "mouse press" << event;
}

ConnectedLines::ConnectedLines(QList<QPointF> points)
    : points(std::move(points))
{
    if (!points.isEmpty()) {
            qreal minX = this->points.first().x();
            qreal minY = this->points.first().y();
            qreal maxX = this->points.first().x();
            qreal maxY = this->points.first().y();

            for (const QPointF& point : this->points) {
                minX = qMin(minX, point.x());
                minY = qMin(minY, point.y());
                maxX = qMax(maxX, point.x());
                maxY = qMax(maxY, point.y());
            }

            b_rect = QRectF(QPointF(minX, minY), QPointF(maxX, maxY));
        }
}

QRectF ConnectedLines::boundingRect() const
{
    return b_rect;
}

void ConnectedLines::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (points.size() == 0) return;

    QPen pen;
    QColor col = Qt::red;
    col.setAlphaF(0.7);
    pen.setColor(col);
    pen.setWidth(8);
//    pen.setStyle(Qt::DashLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);

    QPainterPath path;
    path.moveTo(points[0]);

    for (auto i : points) {
        path.lineTo(i);
    }

    painter->drawPath(path);

}
