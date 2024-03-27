//
// Created by Radek on 24.03.2024.
//

#ifndef ENGINE_READER_H
#define ENGINE_READER_H

#include <string>

class Reader {
public:
    Reader() = default;
    virtual ~Reader() = default;

    virtual int parseFile(std::string) = 0;
protected:
    std::string filename;
};

#endif //ENGINE_READER_H
