//
// Created by Radek on 25.03.2024.
//

#ifndef ENGINE_AUDIOMANAGER_H
#define ENGINE_AUDIOMANAGER_H

#include "Debug/Logger.h"
#include "miniaudio.h"
#include "Sound.h"

class AudioManager {
public:
    AudioManager();
    virtual ~AudioManager() = default;

    int init();
    void end();
//    void playSound();
//    void stopSound();
    Sound* loadSound(const std::string&);

private:
    ma_engine engine;
    std::vector<Sound*> loadedSounds;
};


#endif //ENGINE_AUDIOMANAGER_H
