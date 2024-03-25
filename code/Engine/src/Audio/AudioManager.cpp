//
// Created by Radek on 25.03.2024.
//

#include "Engine/inc/Audio/AudioManager.h"

AudioManager::AudioManager() {

}

int AudioManager::init() {
    if(ma_engine_init(nullptr, &engine) != MA_SUCCESS)
    {
        LOG_ERROR("Couldn't initialize miniaudio Engine");
    }

    return 0;
}

Sound* AudioManager::loadSound(const std::string &path, const std::string& name) {
    Sound* newSound = new Sound(name);
    newSound->setPath(path);
    ma_sound_init_from_file(&engine, path.c_str(), 0, NULL, NULL, &newSound->getSound());
    loadedSounds.push_back(newSound);

    return newSound;
}
