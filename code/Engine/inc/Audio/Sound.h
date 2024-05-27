//
// Created by Radek on 25.03.2024.
//

#ifndef ENGINE_SOUND_H
#define ENGINE_SOUND_H

#include <string>
#include "miniaudio.h"
#include "Core/AssetManager/Asset.h"
#include "Debug/Logger.h"

class Sound : public Asset
{
public:
    explicit Sound(const std::string &path) : Asset(path){};
    virtual ~Sound() = default;

    void play();
    void stop();
    void applyLowPassfilter();
    void load() override {};

    void setVolume(float);

    [[nodiscard]] ma_sound &getSound();

private:
    float volume = 1;
    ma_sound sound;
};

#endif // ENGINE_SOUND_H
