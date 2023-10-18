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
//    strategy = GenStrategy::BFS;
//    strategy = GenStrategy::RANDOM;
//    strategy = GenStrategy::DFS;

    steps.clear();

    Cell *cell;
    for (int i = 0; i < n_row; ++i) {
        for (int j = 0; j < n_col; ++j) {
            cell = maze[i][j];
            cell->is_wall = true;
            cell->prev = nullptr;
            cell->straight_only = false;

            maze_step[i][j] = true;
        }
    }
    std::deque<Cell*> fringe;
    fringe.push_back(maze[1][1]);

    Cell *prev;

    std::vector<Cell*> next;
    int neigb_count, count = 0;
    bool need_to_be_wall = false;

    static int gen_count = 0;
//    std::random_device rd{};
//    std::mt19937 gen{rd()};
    std::mt19937 gen(gen_count++);
    std::normal_distribution d;
    double rnd;
    decltype (fringe.begin()) it;

    while(fringe.size()) {
        emit step();
        switch (strategy) {
        case GenStrategy::BFS:
            cell = fringe[fringe.size() -1];
            fringe.pop_back();
            break;
        case GenStrategy::RANDOM:
            d = std::normal_distribution(0.0, sqrt(fringe.size()));
            it = fringe.begin();
            do {
                rnd = std::round(d(gen));
            } while (rnd < 0);
            rnd = std::max(std::min(rnd, (double)fringe.size()-1),0.0);
            std::advance(it, (int)rnd);
//            std::advance(it, rand() % fringe.size());
            cell = *it;
            fringe.erase(it);
            break;
        case GenStrategy::DFS:
            cell = fringe[0];
            fringe.pop_front();
            break;
        }

        need_to_be_wall = false;
        // check if we can destroy wall
        for (auto i : cell->neighbs) {
            if (i != cell->prev && !i->is_wall) {
                need_to_be_wall = true;
                break;
            }
        }
        if (need_to_be_wall) continue;

        neigb_count = 0;
        next.clear();
        std::sample(cell->neighbs.begin(), cell->neighbs.end(), std::back_inserter(next), cell->neighbs.size(),
                    std::mt19937{rd()});
        std::shuffle(next.begin(), next.end(),std::mt19937 {rd()});

        auto dir = cell->prev ? cell->Direction(*cell->prev) : QPoint(0,0);
        // add all connected walls to fringe
        for (auto i : next) {
            if (i->is_wall && i->neighbs.size() > 3) {
                if (cell->straight_only == false || i->pos() == cell->pos() + dir) {
                    fringe.push_front(i);
                    i->prev = cell;
                    neigb_count++;
                    if (cell->straight_only == false) {
                        i->straight_only = true;
                    }
                }
            }
        }

        cell->is_wall = false;
        maze_step[cell->row][cell->col] = false;

        QList<Cell*> cur_fringe;
        std::copy(fringe.begin(), fringe.end(), std::back_inserter(cur_fringe));
        steps << GenStep{cur_fringe, cell};
        count++;
    }
    last_step = steps.end() - 1;
//    qDebug() << "generated";
    emit generated();
}

void MazeGenerator::SetRowCount(int value)
{
    if (bindSizes) {
        n_col = n_col + (value - n_row);
    }
    n_row = value;
    Reset();
    emit generated();
}

void MazeGenerator::SetColCount(int value)
{
    if (bindSizes) {
        n_row = n_row + (value - n_col);
    }
    n_col = value;
    Reset();
    emit generated();

}

void MazeGenerator::SetBindSizes(bool bind)
{
    bindSizes = bind;
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
        std::vector<bool> s_row(n_col, 1);
        maze_step.push_back(s_row);
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

    steps.clear();

}

void MazeGenerator::Clear()
{
    for (auto row : maze) {
        for (auto cell : row) {
            delete cell;
        }
    }

    maze.clear();
    maze_step.clear();
}

Cell::Cell(int row, int col)
    : row(row), col(col)
{
    is_wall = false;
    cost = 1;
    neighbs.clear();
    prev = nullptr;
}

QPoint Cell::Direction(Cell &oth)
{
    return QPoint{col - oth.col, row - oth.row};
}

QPoint Cell::pos()
{
    return {col, row};
}
