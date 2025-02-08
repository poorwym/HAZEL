#pragma once
#include <memory>
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
#ifdef HAZEL_DYNAMIC_LINK
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
#else
    #define HAZEL_API
#endif

#ifdef HAZEL_ENABLE_ASSERTS
    #ifdef HAZEL_PLATFORM_WINDOWS
        #define HAZEL_ASSERT(x, ...)\
        {\
            if(!(x)){\
                    HAZEL_ERROR("Assertion Failed: {0}", __VA_ARGS__);\
                    __debugbreak();\
                }
        }\
        #define HAZEL_CORE_ASSERT(x, ...)\
        {\
            if(!(x)){\
                    HAZEL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);\
                    __debugbreak();\
                }
        }
    #elif defined(HAZEL_PLATFORM_MACOS) 
        #define HAZEL_ASSERT(x, ...)\
        {\
            if(!(x)){\
                    HAZEL_ERROR("Assertion Failed: {0}", __VA_ARGS__);\
                    __builtin_trap();\
                }
        }\
        #define HAZEL_CORE_ASSERT(x, ...)\
        {\
            if(!(x)){\
                    HAZEL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);\
                    __builtin_trap();\
                }
        }
    #endif
#else
    #define HAZEL_ASSERT(x, ...)
    #define HAZEL_CORE_ASSERT(x, ...)
#endif
// 宏，用于计算二进制位
#define BIT(x) (1 << x)
// 宏，用于绑定事件
#define HAZEL_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Hazel {

    /**
    * @note:这里并不是一个单纯的重命名，实际上并不是所有的shared_ptr被替换成了Ref（比如log中）,
    * 而是Hazel中使用的明确被定义为asset的东西，
    * 比如VertexArray，VertexBuffer，里面的shared_ptr才会使用Ref。
    * 下面的using也可以随时替换为模板类。
    */
    template<typename T>
	using Scope = std::unique_ptr<T>;

    template<typename T>
	using Ref = std::shared_ptr<T>;

}