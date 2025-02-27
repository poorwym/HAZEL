﻿#pragma once
/**
 * @note:   这里是一个log，分为Core和Client，
 *          Core是引擎的log，Client是客户端的log。
 */
#include "hzpch.h"
#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>


namespace Hazel{
    class HAZEL_API Log{
    private:    
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
    public:
        static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    };
}

// core log macros
#define HAZEL_CORE_TRACE(...) ::Hazel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HAZEL_CORE_INFO(...) ::Hazel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HAZEL_CORE_WARN(...) ::Hazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HAZEL_CORE_ERROR(...) ::Hazel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HAZEL_CORE_CRITICAL(...) ::Hazel::Log::GetCoreLogger()->critical(__VA_ARGS__)

// client log macros
#define HAZEL_TRACE(...) ::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HAZEL_INFO(...) ::Hazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define HAZEL_WARN(...) ::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HAZEL_ERROR(...) ::Hazel::Log::GetClientLogger()->error(__VA_ARGS__)
#define HAZEL_CRITICAL(...) ::Hazel::Log::GetClientLogger()->critical(__VA_ARGS__)