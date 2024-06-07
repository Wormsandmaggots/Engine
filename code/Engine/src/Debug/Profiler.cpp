#include "Debug/Profiler.h"
#include <tracy/Tracy.hpp>
#include "glad/glad.h"
#include "glm/glm.hpp"

Profiler::Profiler(){}
Profiler &Profiler::get() {
    static Profiler instance;
    return instance;
}
void Profiler::init() {
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    CreateProcess(NULL, (LPWSTR)"res/tracyExe/Tracy.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
}
void Profiler::end() {
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
void Profiler::markFrame() { //for marking frames
    FrameMark;
}

void Profiler::zoneScope() {
    ZoneScoped;
}

