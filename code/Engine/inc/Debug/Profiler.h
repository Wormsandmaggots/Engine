#ifndef PROFILER_H
#define PROFILER_H
#include "Editor/Camera.h"

class Profiler {
public:
    Profiler(const Profiler&) = delete; //deleting copy constructor
    static Profiler& get();
    void init();
    void end();
    void mark();
private:
    Profiler();
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
};

#endif //PROFILER_H
