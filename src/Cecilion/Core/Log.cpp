#include "Log.h"
#include "../Event/MessagingSystem.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Cecilion {
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;

    void Log::Init() {
        spdlog::pattern_formatter("%^[%T] %n: %v%$");
        s_CoreLogger = spdlog::stdout_color_mt("Cecilion");
        s_CoreLogger->set_level(spdlog::level::trace);

        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
    }
}

