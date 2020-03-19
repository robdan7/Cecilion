#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Cecilion {
    std::shared_ptr<spdlog::logger> Log::s_client_logger;
    std::shared_ptr<spdlog::logger> Log::s_core_logger;

    void Log::Init() {
        spdlog::pattern_formatter("%^[%T] %n: %v%$");
        s_core_logger = spdlog::stdout_color_mt("Cecilion");
        s_core_logger->set_level(spdlog::level::trace);

        s_client_logger = spdlog::stdout_color_mt("APP");
        s_client_logger->set_level(spdlog::level::trace);
    }
}

