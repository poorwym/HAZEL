workspace "Hazel"
    architecture "x64"
    startproject "Sandbox"

    configurations { 
        "Debug", 
        "Release",
        "Dist"
    }

    startproject "Sandbox"

    filter "system:macosx"
        xcodebuildsettings {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["ALWAYS_SEARCH_USER_PATHS"] = "NO"
        }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Engine/vendor/GLFW"

IncludeDir = {}
IncludeDir["GLFW"] = "Engine/vendor/GLFW/include"

project "Engine"
    location "Engine"
    kind "SharedLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "hzpch.h"
    pchsource "Engine/src/hzpch.cpp"
    
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}"
    }

    links {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        
        buildoptions { "/utf-8" }
        
        defines {
            "HAZEL_PLATFORM_WINDOWS",
            "HAZEL_BUILD_DLL"
        }
    
        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "system:macosx"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "HAZEL_PLATFORM_MACOS"
        }

        postbuildcommands {
            ("{MKDIR} ../bin/" .. outputdir .. "/Sandbox"),
            ("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug" 
        defines "HAZEL_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "HAZEL_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "HAZEL_DIST"
        runtime "Release"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "Engine/vendor/spdlog/include",
        "Engine/src",
        "%{IncludeDir.GLFW}"
    }

    links{
        "Engine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        
        buildoptions { "/utf-8" }

        defines{
            "HAZEL_PLATFORM_WINDOWS"
        }

    filter "system:macosx"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines{
            "HAZEL_PLATFORM_MACOS",
        }

    filter "configurations:Debug" 
        defines "HAZEL_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "HAZEL_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "HAZEL_DIST"
        runtime "Release"
        optimize "On"

    