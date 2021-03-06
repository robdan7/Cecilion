#pragma once
#include <memory>
#include "spdlog/spdlog.h"
namespace Cecilion {
    /**
     * Simple logging class.
     * TODO This should be extended so the application can throw exceptions and such.
     */
    class Log {
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger>& get_core_logger() {return s_core_logger;}
        inline static std::shared_ptr<spdlog::logger>& get_client_logger() {return s_client_logger;}
    private:
        static std::shared_ptr<spdlog::logger> s_core_logger;
        static std::shared_ptr<spdlog::logger> s_client_logger;
    };
}

// Define Core log macros
#define CORE_LOG_TRACE(...) ::Cecilion::Log::get_core_logger()->trace(__VA_ARGS__)
#define CORE_LOG_INFO(...)  ::Cecilion::Log::get_core_logger()->info(__VA_ARGS__)
#define CORE_LOG_WARN(...)  ::Cecilion::Log::get_core_logger()->warn(__VA_ARGS__)
#define CORE_LOG_ERROR(...) ::Cecilion::Log::get_core_logger()->error(__VA_ARGS__)
#define CORE_LOG_CRITICAL(...) ::Cecilion::Log::get_core_logger()->critical(__VA_ARGS__)

// Define client log macros
#define LOG_TRACE(...) ::Cecilion::Log::get_client_logger()->trace(__VA_ARGS__)
#define LOG_INFO(...)  ::Cecilion::Log::get_client_logger()->info(__VA_ARGS__)
#define LOG_WARN(...)  ::Cecilion::Log::get_client_logger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Cecilion::Log::get_client_logger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Cecilion::Log::get_client_logger()->critical(__VA_ARGS__)

#ifdef CECILION_ENABLE_ASSERTS
    #define ASSERT(x, ...) {if(!x) {LOG_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
    #define CORE_ASSERT(x, ...) {if(!x) {CORE_LOG_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
#define ASSERT(x,...)
#define CORE_ASSERT(x,...)
#endif