#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
namespace Hazel {
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init(){
        spdlog::set_pattern("%^[%T] %n: %v%$"); // %^: 开始颜色, %$: 结束颜色
        s_CoreLogger = spdlog::stdout_color_mt("HAZEL"); // stdout_color_mt: 输出到控制台
        s_CoreLogger->set_level(spdlog::level::trace); // trace, debug, info, warn, error, critical

        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
    }
} 