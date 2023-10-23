#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <QObject>
#include "mazegenerator.h"
#include <algorithm>
#include <map>
#include <unordered_map>
#include <QElapsedTimer>

class PathFinder : public QObject
{
    Q_OBJECT
public:
    PathFinder(MazeGenerator *mg);

    void SetStartPoint(QPoint pos);
    void SetEndPoint(QPoint pos);

private:
    virtual void FindPath() = 0;

public slots:
    void MakePath();
    void SetPoint(QPoint pos, Qt::MouseButton button);
    void ResetPoints();

public:
    MazeGenerator *mg;
    Cell* start_cell = nullptr;
    Cell* end_cell = nullptr;
    Cell* nearest  = nullptr;
    bool success = false;

    QList<Cell*> path;

private:
    QElapsedTimer timer;
    qint64 find_path_time;

signals:
    void changed();
    void pathChanged();
    void pathTimeChanged(qint64 time);
};

class LinePather : public PathFinder
{
public:
    LinePather(MazeGenerator *mg) : PathFinder(mg) {}

private:
    void FindPath() override;
};

class Node
{
public:
//    Cell* cell;
    int cost;
    Cell *prev;
};

class AStarFinder : public PathFinder
{
public:
    AStarFinder(MazeGenerator *mg) : PathFinder(mg) {}
    void FindPath() override;

private:
    std::unordered_map<Cell*, Node> nodes;
    QList<Cell*> fringe;
};

#endif // PATHFINDER_H
