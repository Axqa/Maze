#include "mazegenerator.h"

MazeGenerator::MazeGenerator()
{
    Reset();
    MakeBounds();
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
}

Cell::Cell(int row, int col)
    : row(row), col(col)
{
    is_wall = false;
    cost = 1;
    neighbs.clear();
}
