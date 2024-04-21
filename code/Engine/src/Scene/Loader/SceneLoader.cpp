//
// Created by Radek on 24.03.2024.
//

#include "Scene/Loader/SceneLoader.h"
#include "Debug/Logger.h"
#include "Scene/Loader/YamlHelpers.h"

using namespace std;

Scene* SceneLoader::loadScene(std::string path) {
    reader.parseFile(path);
    YAML::Node data = reader.getData();

    Scene* s = new Scene(data["Scene"]["SceneName"].as<string>());

    std::vector<std::pair<std::string, Entity*>> toParent;
    std::vector<Entity*> possibleParents;
    std::vector<Entity*> roots;

    for(const auto& node : data["Scene"]["Entities"])
    {
        Entity* newEntity = new Entity(node.as<Entity>());

        if(node["Parent"])
        {
            toParent.emplace_back(node["Parent"].as<string>(), newEntity);
        }
        else
        {
            roots.push_back(newEntity);
        }

        possibleParents.push_back(newEntity);
    }

    int i = 0;

    while(!toParent.empty())
    {
        std::string key = toParent[i].first;
        Entity* unparentedEntity = toParent[i].second;

        auto found = std::find_if(possibleParents.begin(), possibleParents.end(),
                                  [key](const Entity* parent){ return parent->getName() == key;});

         if (*found != nullptr)
         {
             (*found)->addChild(unparentedEntity);

             toParent.erase(toParent.begin() + i);
             i--;
         }

        i++;

        if(i >= toParent.size())
        {
            i = 0;
        }
    }

    s->addEntities(roots);

    return s;
}
