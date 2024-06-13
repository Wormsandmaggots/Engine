#include "Engine/inc/Audio/AudioManager.h"
#include "Core/AssetManager/AssetManager.h"

void AudioManager::playSound(const std::string& filePath, float volume)
{
	auto sound = loadSound(filePath);
    sound->setVolume(volume);
    sound->play();
}

int AudioManager::init() {
    if (ma_engine_init(nullptr, &engine) != MA_SUCCESS)
    {
        LOG_ERROR("Couldn't initialize miniaudio Engine");
    }

    return 0;
}

std::shared_ptr<Sound> AudioManager::loadSound(const std::string& path) {
    auto newSound = std::make_shared<Sound>(*AssetManager::GetAsset<Sound>(path));
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
