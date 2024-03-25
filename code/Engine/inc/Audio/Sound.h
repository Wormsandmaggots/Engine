//
// Created by Radek on 25.03.2024.
//

#ifndef ENGINE_SOUND_H
#define ENGINE_SOUND_H

#include <string>
#include "miniaudio.h"

class Sound {
public:
    explicit Sound(const std::string&);
    virtual ~Sound() = default;

    void play();
    void stop();

    void setPath(const std::string&);
    void setVolume(float);

    [[nodiscard]] ma_sound& getSound();
    [[nodiscard]] std::string getPath() const;

private:
    std::string path;
    std::string name;
    float volume = 1;
    ma_sound sound;
};


#endif //ENGINE_SOUND_H
