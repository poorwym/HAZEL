#pragma once
// 超级煞笔的平台检测，bydIDE看不到预定义宏只好手动再定义，其实注释掉下面这些一点关系也没有
#if defined(_WIN32) || defined(_WIN64)
    #ifndef HAZEL_PLATFORM_WINDOWS
        #define HAZEL_PLATFORM_WINDOWS
    #endif
#elif defined(__APPLE__)
    #ifndef HAZEL_PLATFORM_MACOS
        #define HAZEL_PLATFORM_MACOS
    #endif
#elif defined(__linux__)
    #ifndef HAZEL_PLATFORM_LINUX
        #define HAZEL_PLATFORM_LINUX
    #endif
#else
    #error HAZEL only supports Windows, Linux and Mac!
#endif

// 定义宏，用于导入导出API
#ifdef HAZEL_PLATFORM_WINDOWS
    #ifdef HAZEL_BUILD_DLL
        #define HAZEL_API __declspec(dllexport) // 导出API
    #else
        #define HAZEL_API __declspec(dllimport) // 导入API
    #endif
#elif defined(HAZEL_PLATFORM_MACOS) || defined(HAZEL_PLATFORM_LINUX)
    #define HAZEL_API __attribute__((visibility("default"))) // 导出API
#else
    #error HAZEL only supports Windows, Linux and Mac!
#endif

#define BIT(x) (1 << x)