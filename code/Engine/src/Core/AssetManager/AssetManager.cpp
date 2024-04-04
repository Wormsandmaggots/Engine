//
// Created by Radek on 04.04.2024.
//

#include "Core/AssetManager/AssetManager.h"

std::unordered_map<int, Asset*> AssetManager::Assets;

void AssetManager::end() {
    for (auto pair : Assets) {
        delete pair.second;
    }

    Assets.clear();
}
