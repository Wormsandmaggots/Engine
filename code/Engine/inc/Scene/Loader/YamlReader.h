//
// Created by Radek on 23.03.2024.
//

#ifndef ENGINE_YAMLREADER_H
#define ENGINE_YAMLREADER_H

#include <string>
#include "yaml-cpp/yaml.h"
#include "Reader.h"

class YamlReader : public Reader{
public:
    YamlReader() = default;
    ~YamlReader() override = default;

    int parseFile(std::string path) override;
    YAML::Node getData() const;

private:
    YAML::Node data;
};


#endif //ENGINE_YAMLREADER_H
