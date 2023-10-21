#include "pathfinder.h"
#include <QDebug>

PathFinder::PathFinder(MazeGenerator *mg)
    : mg(mg)
{

}

void PathFinder::SetStartPoint(QPoint pos)
{
    Cell* cell = mg->maze[pos.y()][pos.x()];
    if (cell->is_wall == true) {
        return;
    }

    if (cell == start_cell) {
        start_cell = nullptr;
    }

    if (cell == end_cell) {
        end_cell = nullptr;
    }

    start_cell = cell;

    MakePath();

    emit changed();
}

void PathFinder::SetEndPoint(QPoint pos)
{
    Cell* cell = mg->maze[pos.y()][pos.x()];
    if (cell->is_wall == true) {
        return;
    }

    if (cell == start_cell) {
        start_cell = nullptr;
    }

    if (cell == end_cell) {
        end_cell = nullptr;
    }

    end_cell = cell;

    MakePath();

    emit changed();

}

void PathFinder::MakePath()
{
    path.clear();
    success = false;

    if (start_cell == nullptr || end_cell == nullptr) {
        return;
    }

    FindPath();
}

void PathFinder::SetPoint(QPoint pos, Qt::MouseButton button)
{
    if (button == Qt::MouseButton::LeftButton) {
        SetStartPoint(pos);
    }

    if (button == Qt::MouseButton::RightButton) {
        SetEndPoint(pos);
    }
}

void LinePather::FindPath()
{
    Cell *cell = start_cell, *next;
    int min_dist, dist;
    while (cell != end_cell) {
        path << cell;

        min_dist = INFINITY;
        for (auto i : cell->neighbs) {
            if (min_dist > (dist = i->ManhattanDist(*end_cell))) {
                min_dist = dist;
                next = i;
            }
        }

        cell = next;
    }
    path << end_cell;
    success = true;
    emit pathChanged();
}

void AStarFinder::FindPath()
{
    fringe.clear();
    nodes.clear();

    Cell* cell = nullptr;
    int cost, new_cost, min_dist = INFINITY, dist;
//    fringe[start_cell] 0 + start_cell->SquaredDist(*end_cell);
    fringe << start_cell;
    nodes[start_cell] = {start_cell->SquaredDist(*end_cell), nullptr};
    while (fringe.size()) {
        cell = fringe.first();
        fringe.pop_front();

//        cell = node->first;
//        cost = node->second;
        cost = nodes[cell].cost;

        if ((dist = cell->SquaredDist(*end_cell)) < min_dist) {
            min_dist = dist;
            nearest = cell;
        }
        if (cell == end_cell) break;

        for (auto n: cell->neighbs) {
            if (n->is_wall) continue;
            new_cost = cost + n->cost + n->SquaredDist(*end_cell);
            // not in nodes (not processed)
            if (nodes.find(n) == nodes.end()) {
                fringe.insert(std::lower_bound(fringe.begin(), fringe.end(), new_cost,
                                               [&](Cell* el, int value) {return nodes[el].cost < value;}), n);
//                fringe[n] = new_cost;
                nodes[n].prev = cell;
                nodes[n].cost = new_cost;
            } else {
                // new way somehow shorter
                if (nodes[n].cost > new_cost) {
                    nodes[n].cost = new_cost;
                    nodes[n].prev = cell;
                }
            }
        }
    }
    if (cell != end_cell) {
        cell = nearest;
        while (cell != start_cell) {
            path.push_front(cell);
            cell = nodes[cell].prev;
        }
        return;
    }

    success = true;

    while (cell != start_cell) {
        path.push_front(cell);
        cell = nodes[cell].prev;
    }
    path.push_front(cell); // start_cell

    emit pathChanged();

}
