//
// Created by Radek on 23.03.2024.
//

#ifndef ENGINE_ISERIALIZABLE_H
#define ENGINE_ISERIALIZABLE_H

#include <string>

class ISerializable{
public:
    virtual std::string serialize() = 0;
};
#endif //ENGINE_ISERIALIZABLE_H
