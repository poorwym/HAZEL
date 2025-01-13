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
            ["ALWAYS_SEARCH_USER_PATHS"] = "NO "
        }

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Engine"
    location "Engine"
    kind "SharedLib"
    language "C++"
    
    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/%{prj.name}")
    
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "HAZEL_PLATFORM_WINDOWS",
            "HAZEL_BUILD_DLL"
        }
    
        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. output_dir .. "/Sandbox")
        }

    filter "system:macosx"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "HAZEL_PLATFORM_MACOS"
        }

        postbuildcommands {
            ("{MKDIR} ../bin/" .. output_dir .. "/Sandbox"),
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. output_dir .. "/Sandbox")
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
    
    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "Engine/vendor/spdlog/include",
        "Engine/src"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines{
            HAZEL_PLATFORM_WINDOWS
        }

    filter "system:macosx"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines{
            "HAZEL_PLATFORM_MACOS",
        }

    links{
        "Engine"
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

    