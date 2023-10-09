#include "mazegenerator.h"
#include <deque>
#include <algorithm>
#include <random>
#include <iterator>
#include <QDebug>

MazeGenerator::MazeGenerator()
{
    Reset();
//    MakeBounds();
    Generate();
}

void MazeGenerator::Generate()
{
    Cell *cell;
    for (int i = 0; i < n_row; ++i) {
        for (int j = 0; j < n_col; ++j) {
            cell = maze[i][j];
            cell->is_wall = true;
            cell->prev = nullptr;
        }
    }
    std::deque<Cell*> fringe;
    fringe.push_back(maze[0][0]);

    Cell *prev;

    std::vector<Cell*> next;
    int neigb_count, count = 0;
    bool need_to_be_wall = false;
    while(fringe.size()) {
        cell = fringe[0];
        fringe.pop_front();
        neigb_count = 0;
        need_to_be_wall = false;
        // check if we can destroy wall
        for (auto i : cell->neighbs) {
            if (i != cell->prev && !i->is_wall) {
                need_to_be_wall = true;
                break;
            }
        }
        if (need_to_be_wall) continue;

        next.clear();
        std::sample(cell->neighbs.begin(), cell->neighbs.end(), std::back_inserter(next), cell->neighbs.size(),
                    std::mt19937{rd()});
        std::shuffle(next.begin(), next.end(),std::mt19937 {rd()});
        // add all connected walls to fringe
        for (auto i : next) {
            if (i->is_wall) {
                fringe.push_front(i);
                i->prev = cell;
            }
        }

        cell->is_wall = false;
        count++;
    }
//    qDebug() << "generated";
    emit generated();
}

void MazeGenerator::SetRowCount(int value)
{
    n_row = value;
    Reset();
    emit generated();
}

void MazeGenerator::SetColCount(int value)
{
    n_col = value;
    Reset();
    emit generated();

}

void MazeGenerator::MakeBounds()
{
    for (int i = 0; i < n_row; ++i) {
        for (int j = 0; j < n_col; ++j) {
            if (i == 0 || i == n_row-1 || j == 0 || j == n_col-1)
                maze[i][j]->is_wall = true;
        }
    }

}

void MazeGenerator::Reset()
{
    Clear();

    for (int i = 0; i < n_row; ++i) {
        QVector<Cell*> row;
        for (int j = 0; j < n_col; ++j) {
            row.push_back(new Cell(i, j));
        }
        maze.push_back(row);
    }

    for (int i = 0; i < n_row; ++i) {
        for (int j = 0; j < n_col; ++j) {
            auto cell = maze[i][j];
            if (i < n_row - 1)  cell->neighbs.append(maze[i+1][j]);
            if (i > 0)          cell->neighbs.append(maze[i-1][j]);
            if (j > 0)          cell->neighbs.append(maze[i][j-1]);
            if (j < n_col - 1)  cell->neighbs.append(maze[i][j+1]);
        }
    }

}

void MazeGenerator::Clear()
{
    for (auto row : maze) {
        for (auto cell : row) {
            delete cell;
        }
    }

    maze.clear();
}

Cell::Cell(int row, int col)
    : row(row), col(col)
{
    is_wall = false;
    cost = 1;
    neighbs.clear();
    prev = nullptr;
}
