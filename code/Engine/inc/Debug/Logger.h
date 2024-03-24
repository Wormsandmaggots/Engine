#ifndef OPENGL_LOGGER_H
#define OPENGL_LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using msgType = spdlog::level::level_enum;

//!for basic needs use those
//!-----------------------------------
#define LOG_ERROR(msg) Logger::log(msg, msgType::err)
#define LOG_WARNING(msg) Logger::log(msg, msgType::warn)
#define LOG_INFO(msg) Logger::log(msg, msgType::info)
//!-----------------------------------

class  Logger {

    public:
        static void log(const std::string& message, const char* color, msgType level);
        static void log(const std::string& message, const char* color, const char* level);
        static void log(const std::string& message, msgType level);

    private:

        static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink;
        static spdlog::logger logger;
};


#endif //OPENGL_LOGGER_H
