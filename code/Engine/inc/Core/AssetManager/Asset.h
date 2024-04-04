//
// Created by Radek on 04.04.2024.
//

#ifndef ENGINE_ASSET_H
#define ENGINE_ASSET_H

#include "string"

class Asset {
public:
    explicit Asset(const std::string &path) : path(path){
        //references++;
    };

    virtual ~Asset() = default;

    virtual void load() {};

    [[nodiscard]] std::string getPath() const { return path; }
    //[[nodiscard]] int getReferenceCount() const { return references; }

    //void operator++() { references++; }
    //void operator--() { references--; }

protected:
    std::string path;
    //int references = 0;
};


#endif //ENGINE_ASSET_H
