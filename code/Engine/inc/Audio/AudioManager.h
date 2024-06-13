#ifndef ENGINE_AUDIOMANAGER_H
#define ENGINE_AUDIOMANAGER_H

#include "miniaudio.h"
#include <string>
#include <vector>
#include <memory>
#include "Audio/Sound.h"
#include "Core/AssetManager/AssetManager.h"

class AudioManager : public AssetManager {
public:
    static AudioManager& getInstance() {
        static AudioManager instance; // Guaranteed to be destroyed and instantiated on first use.
        return instance;
    }

    AudioManager(const AudioManager&) = delete; // Delete copy constructor
    void operator=(const AudioManager&) = delete; // Delete copy assignment operator

    void playSound(const std::string& filePath,float);

    int init();
    void end();
    std::shared_ptr<Sound> loadSound(const std::string& filePath);

private:
    AudioManager() : engine() {} // Private constructor with engine initialization
    virtual ~AudioManager() {} // Private destructor

    ma_engine engine;
    std::vector<std::shared_ptr<Sound>> loadedSounds; // Corrected vector declaration
};

#endif // ENGINE_AUDIOMANAGER_H
