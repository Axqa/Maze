#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include <QVector>
#include <QList>
#include <QObject>
#include <random>

class Cell
{
public:
    int row, col;
    bool is_wall;
    int cost;
    QList<Cell*> neighbs;
    Cell *prev;

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

private:
    std::random_device rd;

public:
    MazeGenerator();

signals:
    void generated();

public slots:
    void Generate();
    void SetRowCount(int value);
    void SetColCount(int value);
private:
    void MakeBounds();
    void Reset();
    void Clear();
};

#endif // MAZEGENERATOR_H
