#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include <QVector>
#include <QList>
#include <QObject>

class Cell
{
public:
    int row, col;
    bool is_wall;
    int cost;
    QList<Cell*> neighbs;

public:
    Cell(int row, int col);
};


class MazeGenerator : public QObject
{
    Q_OBJECT

public:
    int n_row = 20;
    int n_col = 20;
    QVector<QVector<Cell*>> maze;

public:
    MazeGenerator();

private:
    void MakeBounds();
    void Reset();
    void Clear();
};

#endif // MAZEGENERATOR_H
