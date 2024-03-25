//
// Created by Radek on 25.03.2024.
//

#include "Audio/Sound.h"
#include "Math.h"

ma_sound& Sound::getSound() {
    return sound;
}

std::string Sound::getPath() const{
    return path;
}

void Sound::play() {
    ma_sound_start(&sound);
}

void Sound::stop() {
    ma_sound_stop(&sound);
}

void Sound::setPath(const std::string &path) {
    this->path = path;
}

Sound::Sound(const std::string &name) {
    this->name = name;
}

void Sound::setVolume(float volume) {
    this->volume = volume;

    clamp<float>(this->volume, 0, 1);

    ma_sound_set_volume(&sound, volume);
}
