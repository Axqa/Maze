#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include <QVector>
#include <QList>
#include <QObject>
#include <random>
#include <QPoint>

class GenStep;

enum class GenStrategy {
    BFS,
    RANDOM,
    DFS
};

class Cell
{
public:
    int row, col;
    bool is_wall;
    int cost;
    QList<Cell*> neighbs;
    Cell *prev;

    bool straight_only;

public:
    Cell(int row, int col);
    QPoint Direction(Cell &oth);
    QPoint pos();
    int ManhattanDist(Cell &oth);
    int SquaredDist(Cell &oth);
};


class MazeGenerator : public QObject
{
    Q_OBJECT

public:
    int n_row = 21;
    int n_col = 21;
    QVector<QVector<Cell*>> maze;
    std::vector<std::vector<bool>> maze_step;

    GenStrategy strategy = GenStrategy::DFS;

    QList<GenStep> steps;
    decltype (steps.begin()) last_step;
private:
    std::random_device rd;
    bool bindSizes = false;
public:
    MazeGenerator();

signals:
    void generated();
    void step();
public slots:
    void Generate();
    void SetRowCount(int value);
    void SetColCount(int value);
    void SetBindSizes(bool bind);
private:
    void MakeBounds();
    void Reset();
    void Clear();
};

struct GenStep
{
    QList<Cell*> fringe;
    Cell* next;
};

#endif // MAZEGENERATOR_H
