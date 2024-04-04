//
// Created by Radek on 04.04.2024.
//

#ifndef ENGINE_ASSETMANAGER_H
#define ENGINE_ASSETMANAGER_H

#include <unordered_map>
#include <string>
#include <filesystem>
#include <typeinfo>
#include "Asset.h"
#include "Debug/Logger.h"
#include "Core/Utils/StringUtils.h"

class AssetManager {
public:
    template<typename T>
    static T* GetAsset(const std::string& path);

    //!IT SHOULD BE CALLED AFTER OTHER MODULES HAS BEEN DEALLOCATED
    static void end();

private:
    static std::unordered_map<int, Asset*> Assets;
};



template<typename T>
T *AssetManager::GetAsset(const std::string &path) {

    if(!std::is_base_of<Asset, T>())
    {
        LOG_ERROR("CLASS '" + std::string(typeid(T).name()) + "' MUST DERIVE ASSET CLASS");
        return nullptr;
    }

    try{
        Asset* asset = Assets.at(utils::stringToHash(path));
        return dynamic_cast<T*>(asset);
    }
    catch (const std::out_of_range& e)
    {
        if(std::filesystem::exists(path))
        {
            Asset* newAsset = new T(path);
            newAsset->load();
            Assets[utils::stringToHash(path)] = newAsset;
            return dynamic_cast<T*>(newAsset);
        }
        else
        {
            LOG_WARNING("COULDN'T FIND A FILE AT PATH: '" + path + "'");
        }
    }

    return nullptr;
}


#endif //ENGINE_ASSETMANAGER_H
