#ifndef OPENGLGP_JSONREADER_H
#define OPENGLGP_JSONREADER_H
#include "nlohmann/json.hpp"
#include "glm/vec3.hpp"

using namespace nlohmann;
using namespace glm;


class JsonReader {
public:
    explicit JsonReader(const std::string &path);
    virtual ~JsonReader() = default;

    //if parsing was successful returns 0
    int ParseFile(std::string path);
    //looks for a property in json file,
    //should be used after file parsing
    vec3 ParseToVec3(std::string obj, std::string propertyName);
    float ParseToFloat(std::string obj, std::string propertyName, std::string val);
    std::string ParseToString(std::string obj, std::string propertyName);

private:
    std::string path = "";
    json data;
};


#endif //OPENGLGP_JSONREADER_H
