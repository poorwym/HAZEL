#pragma once


#ifdef HAZEL_PLATFORM_WINDOWS
    #ifdef HAZEL_BUILD_DLL
        #define HAZEL_API __declspec(dllexport)
    #else
        #define HAZEL_API __declspec(dllimport)
    #endif
#elif HAZEL_PLATFORM_LINUX || HAZEL_PLATFORM_MACOS
    #define HAZEL_API __attribute__((visibility("default")))
#else
    #error HAZEL only supports Windows and Mac!
#endif

