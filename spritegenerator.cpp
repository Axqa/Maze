#include "spritegenerator.h"
#include <QDebug>

SpriteGenerator::SpriteGenerator(MazeGenerator *mg)
    : mg(mg)
{
    grass = QPixmap(":/sprites/grass.png");
    stone = QPixmap(":/sprites/stone.png");

    bool env[3][3];
    for (int i = 0; i < 1<<9; ++i) {
        int dig = 0;
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                env[r][c] = i & (1<<dig++);
            }
        }
        env_to_pos[i] = GetSpritePos(env);
    }
}

std::pair<int, int> GetSpritePos(bool env[3][3])
{
    bool *e = env[0];

    // 0 1 2
    // 3 4 5
    // 6 7 8

    // assume [1][1] is 1 == wall

    if (env[1][1] == 0)
        return {0,0};


    // 0 connected walls
    if ((e[1] | e[3] | e[5] | e[7]) == false)
        return {3,3};

    // 4 connected walls
    if ((e[1] & e[3] & e[5] & e[7]) == true)
        return {6, 1};

    // 2 connected walls
    // horizontally
    if ((e[3] | e[5]) == 0) {
        if (e[1] & e[7])
            return {3,1};
        if (e[7])
            return {3,0};
        return {3,2};
    }
    // vertically
    if ((e[1] | e[7]) == 0) {
        if (e[3] & e[5])
            return {1,3};
        if (e[3])
            return {2,3};
        return {0,3};
    }

    // 2 connected walls
    // in corner
    if ((e[1] + e[3] + e[5] + e[7]) == 2) {
        if (e[1] & e[5])
            return {5,2};
        if (e[5] & e[7])
            return {5,0};
        if (e[7] & e[3])
            return {7,0};
        return {7,2};
    }

    // 3 connected walls
    if ((e[1] + e[3] + e[5] + e[7]) == 3) {
        if (e[1] == 0)
            return {6,0};
        if (e[3] == 0)
            return {5,1};
        if (e[5] == 0)
            return {7,1};
        return {6,2};
    }

    // if somehow stay unhandled
    return {6,4};
}

QPixmap* SpriteGenerator::GetSpriteForXY(int x, int y)
{
    Cell *cell = mg->maze[y][x];
    if (cell->is_wall == false) {
        int row = (((x *23) * (y * 17)) ^ x) % 8;
        int col = (((y *23) * (x * 17)) ^ y) % 8;
//        int row = rand() % 8;
//        int col = rand() % 8;

        return new QPixmap(grass.copy(col * kSpriteWidth, row * kSpriteHeight, kSpriteWidth, kSpriteHeight));
    }


    bool env[3][3];
    int pos = 0, dig = 0;
    bool cur;
    for (int r = y-1; r < y+2; r++) {
        for (int c = x-1; c < x+2; ++c) {
            cur = (r>=0 && r < mg->n_row && c>=0 && c<mg->n_col)
                    && mg->maze[r][c]->is_wall;
            env[r - y + 1][c - x + 1] = cur;
            pos |= cur << dig++;
        }
    }

    auto x_y = env_to_pos[pos];

    return new QPixmap(stone.copy(x_y.first*kSpriteWidth,x_y.second*kSpriteHeight,kSpriteWidth,kSpriteHeight));
}



