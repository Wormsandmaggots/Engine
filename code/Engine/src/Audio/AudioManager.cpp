//
// Created by Radek on 25.03.2024.
//

#include "Engine/inc/Audio/AudioManager.h"
#include "Core/AssetManager/AssetManager.h"

int AudioManager::init() {
    if(ma_engine_init(nullptr, &engine) != MA_SUCCESS)
    {
        LOG_ERROR("Couldn't initialize miniaudio Engine");
    }

    return 0;
}

Sound* AudioManager::loadSound(const std::string &path) {
    Sound* newSound = AssetManager::GetAsset<Sound>(path);
    ma_sound_init_from_file(&engine, path.c_str(), 0, NULL, NULL, &newSound->getSound());
    loadedSounds.push_back(newSound);

    return newSound;
}

void AudioManager::end() {

    for (Sound* s : loadedSounds) {
        ma_sound_uninit(&s->getSound());
    }

    loadedSounds.clear();

    ma_engine_uninit(&engine);
}
