#pragma once

#include "Bubble/Core/Base.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Bubble
{

    class Log
    {
    public:
        static void Init();

        static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };

}

// Core log macros
#define BB_CORE_TRACE(...)    ::Bubble::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BB_CORE_INFO(...)     ::Bubble::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BB_CORE_WARN(...)     ::Bubble::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BB_CORE_ERROR(...)    ::Bubble::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BB_CORE_CRITICAL(...) ::Bubble::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define BB_TRACE(...)         ::Bubble::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BB_INFO(...)          ::Bubble::Log::GetClientLogger()->info(__VA_ARGS__)
#define BB_WARN(...)          ::Bubble::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BB_ERROR(...)         ::Bubble::Log::GetClientLogger()->error(__VA_ARGS__)
#define BB_CRITICAL(...)      ::Bubble::Log::GetClientLogger()->critical(__VA_ARGS__)