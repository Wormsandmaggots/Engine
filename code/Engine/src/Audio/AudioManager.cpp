#include "Engine/inc/Audio/AudioManager.h"
#include "Core/AssetManager/AssetManager.h"

void AudioManager::playSound(const std::string& filePath, float volume)
{
    auto sound = loadSound(filePath);
    if (!sound) {
        // Handle the case where the sound could not be loaded. For example, log an error.
        LOG_ERROR("Failed to load sound: " + filePath);
        return;
    }
    sound->setVolume(volume);
    sound->play();
}

void AudioManager::playThisSound(const std::string& name, const std::string& filePath, float volume)
{
    auto sound = loadSound(filePath);
    if (!sound) {
        LOG_ERROR("Failed to load sound: " + filePath);
        return;
    }
    sound->setVolume(volume);
    soundMap[name] = sound;
}

void AudioManager::playThisSong(const std::string& name) {
    auto it = soundMap.find(name);
    if (it != soundMap.end()) {
        it->second->play();
    }
}

void AudioManager::pauseThisSong(const std::string& name) {
    auto it = soundMap.find(name);
    if (it != soundMap.end()) {
        it->second->stop();
    }
}




int AudioManager::init() {
    if (ma_engine_init(nullptr, &engine) != MA_SUCCESS)
    {
        LOG_ERROR("Couldn't initialize miniaudio Engine");
    }

    return 0;
}

std::shared_ptr<Sound> AudioManager::loadSound(const std::string& path) {
    auto asset = AssetManager::GetAsset<Sound>(path);
    if (!asset) {
        // Handle the case where the asset is not found. For example, log an error and return a null pointer.
        LOG_ERROR("Asset not found: " + path);
        return nullptr;
    }

    auto newSound = std::make_shared<Sound>(*asset);
    ma_sound_init_from_file(&engine, path.c_str(), MA_SOUND_FLAG_NO_SPATIALIZATION, NULL, NULL, &newSound->getSound());
    loadedSounds.push_back(newSound);

    return newSound;
}


void AudioManager::end() {
    for (auto& s : loadedSounds) {
        ma_sound_uninit(&s->getSound());
    }

    loadedSounds.clear();
    ma_engine_uninit(&engine);
}

