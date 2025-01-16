workspace "Hazel"
    -- 指定编译架构为 x64
    architecture "x64"
    -- 指定启动项目为 Sandbox
    startproject "Sandbox"

    -- 配置列表，包含 Debug、Release、Dist 三种配置
    configurations { 
        "Debug", 
        "Release",
        "Dist"
    }

    -- 再次指定启动项目为 Sandbox（部分 Premake 版本可能需要重复指定）
    startproject "Sandbox"

    -- 添加全局 UTF-8 编码设置
    flags { "MultiProcessorCompile", "NoMinimalRebuild" }
    characterset "Unicode"         -- 设置字符集为 Unicode

    -- 针对 macOS 系统的特定过滤器
    filter "system:macosx"
        -- Xcode 的构建设置
        xcodebuildsettings {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["ALWAYS_SEARCH_USER_PATHS"] = "NO"
        }

-- 输出目录，根据配置、系统和架构动态生成
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- 包含 GLFW 第三方库的 Premake 脚本
include "Engine/vendor/GLFW"

-- 定义包含目录的表
IncludeDir = {}
IncludeDir["GLFW"] = "Engine/vendor/GLFW/include"

-- 定义名为 "Engine" 的项目
project "Engine"
    -- 指定项目在解决方案中的位置
    location "Engine"
    -- 生成的目标类型为共享库（dll）
    kind "SharedLib"
    -- 指定使用 C++ 语言
    language "C++"
    
    -- 设置最终生成的二进制文件目录
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    -- 设置中间文件（obj 等）的输出目录
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    -- 指定预编译头（pch）头文件
    pchheader "hzpch.h"
    -- 对应的预编译头源文件
    pchsource "Engine/src/hzpch.cpp"
    
    -- 包含当前项目的所有 .h 和 .cpp 文件
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    -- 包含目录列表
    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}"
    }

    -- 确保 GLFW 在 Engine 之前构建
    dependson {
        "GLFW"
    }
    
    -- 链接的库文件，这里链接 GLFW 和 opengl32.lib
    links {
        "GLFW",
        "opengl32.lib",
        "ucrtd.lib",      -- Debug 模式下的 C 运行时
        "msvcrtd.lib"     -- Debug 模式下的 C++ 运行时
    }

    -- 针对 Windows 平台的过滤器
    filter "system:windows"
        -- 使用 C++17 标准
        cppdialect "C++17"
        -- 静态运行时
        staticruntime "Off"
        -- 使用最新可用的 Windows SDK
        systemversion "latest"
        
        -- 添加 UTF-8 编译选项
        buildoptions { 
            "/utf-8",            -- 源文件的编码
            "/validate-charset"  -- 验证源文件字符集
        }
        
        -- 定义宏，标识当前平台和构建 DLL
        defines {
            "HAZEL_PLATFORM_WINDOWS",
            "HAZEL_BUILD_DLL"
        }
    
        -- 后置构建命令，将生成的 DLL 复制到 Sandbox 项目的可执行文件目录
        postbuildcommands {
            ("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

        -- 链接的库文件，这里链接 GLFW 和 opengl32.lib
        links {
            "GLFW",
            "opengl32.lib",
            "ucrtd.lib",      -- Debug 模式下的 C 运行时
            "msvcrtd.lib"     -- Debug 模式下的 C++ 运行时
        }

    -- 针对 macOS 平台的过滤器
    filter "system:macosx"
        -- 使用 C++17 标准
        cppdialect "C++17"
        -- 静态运行时
        staticruntime "On"
        -- 使用最新可用的 macOS SDK
        systemversion "latest"

        -- 定义宏，标识当前平台
        defines {
            "HAZEL_PLATFORM_MACOS"
        }

        -- 后置构建命令，创建 Sandbox 目录并复制生成的文件
        postbuildcommands {
            ("{MKDIR} ../bin/" .. outputdir .. "/Sandbox"),
            ("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    -- 针对 Debug 配置的过滤器
    filter "configurations:Debug" 
        -- 定义 Debug 模式宏
        defines "HAZEL_DEBUG"
        -- 使用调试版本的运行时库
        runtime "Debug"
        -- 开启调试符号
        symbols "On"
        -- 指定使用多线程调试版本
        buildoptions {
            "/MDd"
        }

    -- 针对 Release 配置的过滤器
    filter "configurations:Release"
        -- 定义 Release 模式宏
        defines "HAZEL_RELEASE"
        -- 使用发布版本的运行时库
        runtime "Release"
        -- 开启编译优化
        optimize "On"
        -- 指定使用多线程版本
        buildoptions {
            "/MD"
        }

    -- 针对 Dist 配置的过滤器
    filter "configurations:Dist"
        -- 定义 Dist 模式宏
        defines "HAZEL_DIST"
        -- 使用发布版本的运行时库
        runtime "Release"
        -- 开启编译优化
        optimize "On"
        -- 指定使用多线程版本
        buildoptions {
            "/MD"
        }


-- 定义名为 "Sandbox" 的项目
project "Sandbox"
    -- 指定项目在解决方案中的位置
    location "Sandbox"
    -- 生成类型为控制台应用
    kind "ConsoleApp"
    -- 指定使用 C++ 语言
    language "C++"
    
    -- 设置最终生成的二进制文件目录
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    -- 设置中间文件的输出目录
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    -- 包含当前项目的所有 .h 和 .cpp 文件
    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    -- 包含目录列表
    includedirs{
        "Engine/vendor/spdlog/include",
        "Engine/src",
        "%{IncludeDir.GLFW}"
    }

    -- 确保 Engine 在 Sandbox 之前构建
    dependson {
        "Engine"
    }
    
    -- 链接 Engine 项目生成的动态库或静态库
    links{
        "Engine"
    }

    -- 针对 Windows 平台的过滤器
    filter "system:windows"
        -- 使用 C++17 标准
        cppdialect "C++17"
        -- 静态运行时
        staticruntime "Off"
        -- 使用最新可用的 Windows SDK
        systemversion "latest"
        
        -- 添加 UTF-8 编译选项
        buildoptions { 
            "/utf-8",            -- 源文件的编码
            "/validate-charset"  -- 验证源文件字符集
        }
        
        -- 定义宏，标识当前平台
        defines{
            "HAZEL_PLATFORM_WINDOWS"
        }

    -- 针对 macOS 平台的过滤器
    filter "system:macosx"
        -- 使用 C++17 标准
        cppdialect "C++17"
        -- 静态运行时
        staticruntime "On"
        -- 使用最新可用的 macOS SDK
        systemversion "latest"

        -- 定义宏，标识当前平台
        defines{
            "HAZEL_PLATFORM_MACOS",
        }

    -- 针对 Debug 配置的过滤器
    filter "configurations:Debug" 
        -- 定义 Debug 模式宏
        defines "HAZEL_DEBUG"
        -- 使用调试版本的运行时库
        runtime "Debug"
        -- 开启调试符号
        symbols "On"
        -- 指定使用多线程调试版本
        buildoptions {
            "/MDd"
        }

    -- 针对 Release 配置的过滤器
    filter "configurations:Release"
        -- 定义 Release 模式宏
        defines "HAZEL_RELEASE"
        -- 使用发布版本的运行时库
        runtime "Release"
        -- 开启编译优化
        optimize "On"
        -- 指定使用多线程版本
        buildoptions {
            "/MD"
        }


    -- 针对 Dist 配置的过滤器
    filter "configurations:Dist"
        -- 定义 Dist 模式宏
        defines "HAZEL_DIST"
        -- 使用发布版本的运行时库
        runtime "Release"
        -- 开启编译优化
        optimize "On"
        -- 指定使用多线程版本
        buildoptions {
            "/MD"
        }