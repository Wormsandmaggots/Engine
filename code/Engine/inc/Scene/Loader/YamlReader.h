//
// Created by Radek on 23.03.2024.
//

#ifndef ENGINE_YAMLREADER_H
#define ENGINE_YAMLREADER_H

#include <string>
#include "yaml-cpp/yaml.h"
#include "Reader.h"
#include "Debug/Logger.h"
#include <optional>


class YamlReader : public Reader {
public:
    YamlReader() = default;
    ~YamlReader() override = default;

    int parseFile(std::string path) override;
    YAML::Node getData() const;

    template<class T>
    std::optional<T> getByPath(const YAML::Node& start, std::initializer_list<std::string> pathToValue) const;

private:
    YAML::Node data;
};


#endif //ENGINE_YAMLREADER_H
