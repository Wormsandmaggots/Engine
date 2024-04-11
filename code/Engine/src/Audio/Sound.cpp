//
// Created by Radek on 25.03.2024.
//

#include "Audio/Sound.h"
#include "Core/Utils/MathUtils.h"

ma_sound& Sound::getSound() {
    return sound;
}

void Sound::play() {
    ma_sound_start(&sound);
}

void Sound::stop() {
    ma_sound_stop(&sound);
}

void Sound::setVolume(float volume) {
    this->volume = volume;

    Math::Clamp<float>(this->volume, 0, 1);

    ma_sound_set_volume(&sound, volume);
}
