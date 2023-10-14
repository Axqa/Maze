#ifndef SPRITEGENERATOR_H
#define SPRITEGENERATOR_H

#include <QObject>
#include "mazegenerator.h"
#include <QPixmap>
#include <unordered_map>

const int kSpriteWidth  = 32;
const int kSpriteHeight = 32;

class SpriteGenerator : public QObject
{
    Q_OBJECT
public:
    SpriteGenerator(MazeGenerator *mg);

    QPixmap* GetSpriteForXY(int x, int y);

    QPixmap* GetGrassSprite(int x, int y);
    QPixmap* GetStoneSprite(int x, int y);
    QPixmap* GetStoneSpriteForStep(int x, int y);

private:
    MazeGenerator *mg;

    QPixmap grass;
    QPixmap stone;

    std::unordered_map<int, std::pair<int,int>> env_to_pos;
};

std::pair<int,int> GetSpritePos(bool env[3][3]);

#endif // SPRITEGENERATOR_H
