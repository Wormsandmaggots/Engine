#include <fstream>
#include "../inc/JsonReader.h"

JsonReader::JsonReader(const std::string &path) : path(path) {
    ParseFile(this->path);
}

int JsonReader::ParseFile(std::string path) {
    this->path = path;
    std::ifstream f(this->path);
    data = json::parse(f);
    return 0;
}

vec3 JsonReader::ParseToVec3(std::string obj, std::string propertyName) {
    float x = (float)data[obj][propertyName]["x"];
    float y = (float)data[obj][propertyName]["y"];
    float z = (float)data[obj][propertyName]["z"];

    return {x,y,z};
}

float JsonReader::ParseToFloat(std::string obj, std::string propertyName, std::string val) {
    return data[obj][propertyName][val];
}

std::string JsonReader::ParseToString(std::string obj, std::string propertyName) {
    return data[obj][propertyName];
}
