#ifndef CECILION_CORE_LOG_H
#define CECILION_CORE_LOG_H
#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"


namespace Cecilion {
    class Log {
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {return s_CoreLogger;}
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {return s_ClientLogger;}
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

}

// Define Core log macros
#define CORE_LOG_TRACE(...) ::Cecilion::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_LOG_INFO(...)  ::Cecilion::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_LOG_WARN(...)  ::Cecilion::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_LOG_ERROR(...) ::Cecilion::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_LOG_CRITICAL(...) ::Cecilion::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Define client log macros
#define LOG_TRACE(...) ::Cecilion::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)  ::Cecilion::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)  ::Cecilion::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Cecilion::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Cecilion::Log::GetClientLogger()->critical(__VA_ARGS__)

#endif


