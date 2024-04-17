//
// Created by Radek on 08.04.2024.
//

#include <fstream>
#include "Editor/YamlWriter.h"
#include "Scene/Scene2.h"
#include "yaml-cpp/yaml.h"
#include "Scene/Loader/YamlHelpers.h"

std::string YamlWriter::lastlySavedScene = "";

void YamlWriter::WriteTo(std::string path, YAML::Node toWrite) {
    std::ofstream file(path);
}

void YamlWriter::WriteTo(const Scene2 &sceneToSave) {
    lastlySavedScene = sceneToSave.getName();

    YAML::Emitter emitter;
    emitter << YAML::BeginMap << YAML::Key << "Scene";
    emitter << YAML::BeginMap << YAML::Key << "SceneName" << YAML::Value << sceneToSave.getName() << YAML::EndMap;
    emitter << YAML::BeginSeq << YAML::Key << "Entities";
    for (Entity* e : sceneToSave.getSceneEntities()) {
        emitter << e;
    }
}

void YamlWriter::WriteTo(Scene2* sceneToSave, std::string path) {
    std::ofstream file(path);

    lastlySavedScene = sceneToSave->getName();

    YAML::Emitter emitter;
    emitter << YAML::BeginMap << YAML::Key << "Scene";
    emitter << YAML::BeginMap << YAML::Key << "SceneName" << YAML::Value << sceneToSave->getName();
    emitter  << YAML::Key << "Entities";
    emitter << YAML::BeginSeq;
    for (Entity* e : sceneToSave->getSceneEntities()) {
        e->toYaml(emitter);
    }
    emitter << YAML::EndSeq << YAML::EndMap << YAML::EndMap;

    file << emitter.c_str();
}
