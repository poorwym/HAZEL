#pragma once

// 定义宏，用于导入导出API
#ifdef HAZEL_PLATFORM_WINDOWS
    #ifdef HAZEL_BUILD_DLL
        #define HAZEL_API __declspec(dllexport) // 导出API
    #else
        #define HAZEL_API __declspec(dllimport) // 导入API
    #endif
#elif HAZEL_PLATFORM_LINUX || HAZEL_PLATFORM_MACOS
    #define HAZEL_API __attribute__((visibility("default"))) // 导出API
#else
    #error HAZEL only supports Windows and Mac!
#endif

