#ifndef PROFILER_H
#define PROFILER_H

#include <string>
#include "Editor/Camera.h"

class Profiler {
public:
    Profiler(const Profiler&) = delete; //deleting copy constructor
    static Profiler& get();
    void init();
    void end();
    void markFrame(); //marking frames
    ///zone represents the lifetime of a special on-stack profiler variable. Typically it would exist for the duration of a whole scope of the
    ///profiled function, but you also can measure time spent in scopes of a for-loop or an if-branch.
    void zoneScope();
    void zoneScope(std::string name);
private:
    Profiler();
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
};

#endif //PROFILER_H