#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QSound>
#include <QSoundEffect>

class SoundManager : public QObject
{
    Q_OBJECT
public:
    static SoundManager& Instance() {
        static SoundManager sm;
        return sm;
    }

    void WallSet();
    void WallClear();
private:
    SoundManager();

private:
    QSoundEffect wall_set;
    QSoundEffect wall_clear;
//    QSound wall_set;
//    QSound wall_clear;
};

#endif // SOUNDMANAGER_H
