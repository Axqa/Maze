#include "soundmanager.h"

void SoundManager::WallSet()
{
    wall_set.play();
}

void SoundManager::WallClear()
{
    wall_clear.play();
}

SoundManager::SoundManager()
//    : wall_set(":/assets/sounds/MI_SFX 24.wav")
//    , wall_clear(":/assets/sounds/MI_SFX 30.wav")
{
    wall_set.setSource(QUrl::fromLocalFile(":/assets/sounds/MI_SFX 24.wav"));
    wall_clear.setSource(QUrl::fromLocalFile(":/assets/sounds/MI_SFX 30.wav"));

    wall_set.setVolume(0.5f);
    wall_clear.setVolume(0.5f);
}
