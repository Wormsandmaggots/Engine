//
// Created by Radek on 06.04.2024.
//

#ifndef ENGINE_FILEDIALOG_H
#define ENGINE_FILEDIALOG_H

#include <string>

class FileDialog {
public:

    static std::string openFile(const char*);
    static std::string saveFile(const char*);

private:
    static std::string ResPath;
};


#endif //ENGINE_FILEDIALOG_H
