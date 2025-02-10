# CMake 项目文件结构
生成时间: 2025-02-10 23:21:49

Engine/
  CMakeLists.txt
  Engine.vcxproj
  Engine.vcxproj.filters
  Engine.vcxproj.user
  .vscode/
    c_cpp_properties.json
  src/
    Hazel.h
    hzpch.cpp
    hzpch.h
    Hazel/
      Application.cpp
      Application.h
      Core.h
      EntryPoint.h
      Input.h
      KeyCodes.h
      Layer.cpp
      Layer.h
      LayerStack.cpp
      LayerStack.h
      Log.cpp
      Log.h
      MouseButtonCodes.h
      OrthographicCameraController.cpp
      OrthographicCameraController.h
      Window.h
      Core/
        Timestep.h
      Events/
        ApplicationEvent.h
        Event.h
        KeyEvent.h
        MouseEvent.h
      ImGui/
        ImGuiBuild.cpp
        ImGuiLayer.cpp
        ImGuiLayer.h
      Renderer/
        Buffer.cpp
        Buffer.h
        GraphicsContext.h
        OrthographicCamera.cpp
        OrthographicCamera.h
        RenderCommand.cpp
        RenderCommand.h
        Renderer.cpp
        Renderer.h
        RendererAPI.cpp
        RendererAPI.h
        Shader.cpp
        Shader.h
        Texture.cpp
        Texture.h
        VertexArray.cpp
        VertexArray.h
    Platform/
      OpenGL/
        OpenGLBuffer.cpp
        OpenGLBuffer.h
        OpenGLContext.cpp
        OpenGLContext.h
        OpenGLRendererAPI.cpp
        OpenGLRendererAPI.h
        OpenGLShader.cpp
        OpenGLShader.h
        OpenGLTexture.cpp
        OpenGLTexture.h
        OpenGLVertexArray.cpp
        OpenGLVertexArray.h
      Windows/
        WindowsInput.cpp
        WindowsInput.h
        WindowsWindow.cpp
        WindowsWindow.h


# CMake 文件内容

## CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.20)
project(Engine)

# 设置源文件目录
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 自动获取所有源文件
file(GLOB_RECURSE HAZEL_SOURCES 
    "${SOURCE_DIR}/Hazel/*.cpp"
    "${SOURCE_DIR}/Hazel/*.h"
    "${SOURCE_DIR}/Platform/*.cpp"
    "${SOURCE_DIR}/Platform/*.h"
    "${SOURCE_DIR}/Hazel.h"
    "${SOURCE_DIR}/Hazel/Event/*.h"
    "${SOURCE_DIR}/Hazel/Event/*.cpp"
    "${SOURCE_DIR}/Hazel/Renderer/*.h"
    "${SOURCE_DIR}/Hazel/Renderer/*.cpp"
    "${SOURCE_DIR}/Hazel/Core/*.h"
    "${SOURCE_DIR}/Hazel/Core/*.cpp"
    "${VENDOR_DIR}/stb_image/*.h"
    "${VENDOR_DIR}/stb_image/*.cpp"
)



# 输出相对路径的源文件列表（可选，用于调试）
foreach(SOURCE ${HAZEL_SOURCES})
    file(RELATIVE_PATH REL_SOURCE ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCE})
    message(STATUS "Found source file: ${REL_SOURCE}")
endforeach()
# message(STATUS "Found source files: ${HAZEL_SOURCES}")

add_library(Engine STATIC
    ${HAZEL_SOURCES}
)

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HAZEL
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform
    ${INCLUDE_DIR}
    ${VENDOR_DIR}
    ${VENDOR_DIR}/GLFW/include
    ${VENDOR_DIR}/spdlog/include
    ${VENDOR_DIR}/glm
    ${VENDOR_DIR}/imgui
    ${VENDOR_DIR}/stb_image
)



target_precompile_headers(Engine 
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hzpch.h"
)

target_compile_options(Engine PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/Yu"hzpch.h">
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-include hzpch.h>
)


# 设置架构相关的编译选项
set_architecture_options()

# 设置平台相关的宏定义
set_platform_definitions(Engine)

# 为 Windows 添加额外的定义
if(WIN32)
    add_definitions(-DENGINE_EXPORTS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_compile_definitions(Engine PRIVATE HAZEL_BUILD_DLL)
    target_compile_definitions(Engine PRIVATE GLFW_INCLUDE_NONE)
endif()

if(APPLE)
    target_compile_definitions(Engine PRIVATE HAZEL_BUILD_DLL)
    target_compile_definitions(Engine PRIVATE GLFW_INCLUDE_NONE)
endif()

# 设置输出目录
set_project_output_directories(Engine)

# 打印项目信息
print_project_info(Engine)

# 下面这两句不加上就不能使用opengl的函数,但是可以使用glfw的函数
# 查找OpenGL库
find_package(OpenGL REQUIRED)
# 链接OpenGL库
target_link_libraries(Engine PRIVATE OpenGL::GL)

target_link_libraries(Engine 
    PRIVATE 
    imgui
    glfw
    glad
    glm
)




```

## vendor\Glad\CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.20)
project(glad C)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 创建静态库
add_library(glad STATIC
    src/glad.c
)

# 设置包含目录
target_include_directories(glad
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include 
)

# 关闭警告
if(MSVC)
    target_compile_options(glad PRIVATE /W0)
else()
    target_compile_options(glad PRIVATE -w)
endif()

# 设置C运行时库
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# Debug配置
set_target_properties(glad PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib/Debug"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib/Debug"
)

# Release配置
set_target_properties(glad PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/Release"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/Release"
)

# 优化设置
target_compile_options(glad PRIVATE
    $<$<CONFIG:Debug>:-O0 -g>
    $<$<CONFIG:Release>:-O3>
)

```

## vendor\GLFW\CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.4...3.20 FATAL_ERROR)

project(GLFW VERSION 3.4.0 LANGUAGES C)

set(CMAKE_LEGACY_CYGWIN_WIN32 OFF)

if (POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

if (POLICY CMP0069)
    cmake_policy(SET CMP0069 NEW)
endif()

if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(GLFW_STANDALONE TRUE)
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" ${GLFW_STANDALONE})
option(GLFW_BUILD_TESTS "Build the GLFW test programs" ${GLFW_STANDALONE})
option(GLFW_BUILD_DOCS "Build the GLFW documentation" ON)
option(GLFW_INSTALL "Generate installation target" ON)

include(GNUInstallDirs)
include(CMakeDependentOption)

if (GLFW_USE_OSMESA)
    message(FATAL_ERROR "GLFW_USE_OSMESA has been removed; set the GLFW_PLATFORM init hint")
endif()

cmake_dependent_option(GLFW_BUILD_WIN32 "Build support for Win32" ON "WIN32" OFF)
cmake_dependent_option(GLFW_BUILD_COCOA "Build support for Cocoa" ON "APPLE" OFF)
cmake_dependent_option(GLFW_BUILD_X11 "Build support for X11" ON "UNIX;NOT APPLE" OFF)
cmake_dependent_option(GLFW_BUILD_WAYLAND "Build support for Wayland"
    "${GLFW_USE_WAYLAND}" "UNIX;NOT APPLE" OFF)

cmake_dependent_option(GLFW_USE_HYBRID_HPG "Force use of high-performance GPU on hybrid systems" OFF
                       "WIN32" OFF)
cmake_dependent_option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" ON
                       "MSVC" OFF)

set(GLFW_LIBRARY_TYPE "${GLFW_LIBRARY_TYPE}" CACHE STRING
    "Library type override for GLFW (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")

if (GLFW_LIBRARY_TYPE)
    if (GLFW_LIBRARY_TYPE STREQUAL "SHARED")
        set(GLFW_BUILD_SHARED_LIBRARY TRUE)
    else()
        set(GLFW_BUILD_SHARED_LIBRARY FALSE)
    endif()
else()
    set(GLFW_BUILD_SHARED_LIBRARY ${BUILD_SHARED_LIBS})
endif()

list(APPEND CMAKE_MODULE_PATH "${GLFW_SOURCE_DIR}/CMake/modules")

find_package(Threads REQUIRED)

if (GLFW_BUILD_DOCS)
    set(DOXYGEN_SKIP_DOT TRUE)
    find_package(Doxygen)
endif()

#--------------------------------------------------------------------
# Report backend selection
#--------------------------------------------------------------------
if (GLFW_BUILD_WIN32)
    message(STATUS "Including Win32 support")
endif()
if (GLFW_BUILD_COCOA)
    message(STATUS "Including Cocoa support")
endif()
if (GLFW_BUILD_WAYLAND)
    message(STATUS "Including Wayland support")
endif()
if (GLFW_BUILD_X11)
    message(STATUS "Including X11 support")
endif()

#--------------------------------------------------------------------
# Apply Microsoft C runtime library option
# This is here because it also applies to tests and examples
#--------------------------------------------------------------------
if (MSVC AND NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
    if (CMAKE_VERSION VERSION_LESS 3.15)
        foreach (flag CMAKE_C_FLAGS
                      CMAKE_C_FLAGS_DEBUG
                      CMAKE_C_FLAGS_RELEASE
                      CMAKE_C_FLAGS_MINSIZEREL
                      CMAKE_C_FLAGS_RELWITHDEBINFO)

            if (flag MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
            endif()
            if (flag MATCHES "/MDd")
                string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
            endif()

        endforeach()
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()

#--------------------------------------------------------------------
# Create generated files
#--------------------------------------------------------------------
include(CMakePackageConfigHelpers)

set(GLFW_CONFIG_PATH "${CMAKE_INSTALL_LIBDIR}/cmake/glfw3")

configure_package_config_file(CMake/glfw3Config.cmake.in
                              src/glfw3Config.cmake
                              INSTALL_DESTINATION "${GLFW_CONFIG_PATH}"
                              NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(src/glfw3ConfigVersion.cmake
                                 VERSION ${GLFW_VERSION}
                                 COMPATIBILITY SameMajorVersion)

#--------------------------------------------------------------------
# Add subdirectories
#--------------------------------------------------------------------
add_subdirectory(src)

if (GLFW_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (GLFW_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if (DOXYGEN_FOUND AND GLFW_BUILD_DOCS)
    add_subdirectory(docs)
endif()

#--------------------------------------------------------------------
# Install files other than the library
# The library is installed by src/CMakeLists.txt
#--------------------------------------------------------------------
if (GLFW_INSTALL)
    install(DIRECTORY include/GLFW DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN glfw3.h PATTERN glfw3native.h)

    install(FILES "${GLFW_BINARY_DIR}/src/glfw3Config.cmake"
                  "${GLFW_BINARY_DIR}/src/glfw3ConfigVersion.cmake"
            DESTINATION "${GLFW_CONFIG_PATH}")

    install(EXPORT glfwTargets FILE glfw3Targets.cmake
            EXPORT_LINK_INTERFACE_LIBRARIES
            DESTINATION "${GLFW_CONFIG_PATH}")
    install(FILES "${GLFW_BINARY_DIR}/src/glfw3.pc"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

    if (DOXYGEN_FOUND AND GLFW_BUILD_DOCS)
        install(DIRECTORY "${GLFW_BINARY_DIR}/docs/html"
                DESTINATION "${CMAKE_INSTALL_DOCDIR}")
    endif()

    # Only generate this target if no higher-level project already has
    if (NOT TARGET uninstall)
        configure_file(CMake/cmake_uninstall.cmake.in
                       cmake_uninstall.cmake IMMEDIATE @ONLY)

        add_custom_target(uninstall
                          "${CMAKE_COMMAND}" -P
                          "${GLFW_BINARY_DIR}/cmake_uninstall.cmake")
        set_target_properties(uninstall PROPERTIES FOLDER "GLFW3")
    endif()
endif()


```

## vendor\GLFW\CMake\GenerateMappings.cmake
```cmake
# Usage:
# cmake -P GenerateMappings.cmake <path/to/mappings.h.in> <path/to/mappings.h>

set(source_url "https://raw.githubusercontent.com/gabomdq/SDL_GameControllerDB/master/gamecontrollerdb.txt")
set(source_path "${CMAKE_CURRENT_BINARY_DIR}/gamecontrollerdb.txt")
set(template_path "${CMAKE_ARGV3}")
set(target_path "${CMAKE_ARGV4}")

if (NOT EXISTS "${template_path}")
    message(FATAL_ERROR "Failed to find template file ${template_path}")
endif()

file(DOWNLOAD "${source_url}" "${source_path}"
     STATUS download_status
     TLS_VERIFY on)

list(GET download_status 0 status_code)
list(GET download_status 1 status_message)

if (status_code)
    message(FATAL_ERROR "Failed to download ${source_url}: ${status_message}")
endif()

file(STRINGS "${source_path}" lines)
foreach(line ${lines})
    if (line MATCHES "^[0-9a-fA-F]")
        if (line MATCHES "platform:Windows")
            if (GLFW_WIN32_MAPPINGS)
                string(APPEND GLFW_WIN32_MAPPINGS "\n")
            endif()
            string(APPEND GLFW_WIN32_MAPPINGS "\"${line}\",")
        elseif (line MATCHES "platform:Mac OS X")
            if (GLFW_COCOA_MAPPINGS)
                string(APPEND GLFW_COCOA_MAPPINGS "\n")
            endif()
            string(APPEND GLFW_COCOA_MAPPINGS "\"${line}\",")
        elseif (line MATCHES "platform:Linux")
            if (GLFW_LINUX_MAPPINGS)
                string(APPEND GLFW_LINUX_MAPPINGS "\n")
            endif()
            string(APPEND GLFW_LINUX_MAPPINGS "\"${line}\",")
        endif()
    endif()
endforeach()

configure_file("${template_path}" "${target_path}" @ONLY NEWLINE_STYLE UNIX)
file(REMOVE "${source_path}")


```

## vendor\GLFW\CMake\i686-w64-mingw32-clang.cmake
```cmake
# Define the environment for cross-compiling with 32-bit MinGW-w64 Clang
SET(CMAKE_SYSTEM_NAME    Windows) # Target system name
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_C_COMPILER     "i686-w64-mingw32-clang")
SET(CMAKE_CXX_COMPILER   "i686-w64-mingw32-clang++")
SET(CMAKE_RC_COMPILER    "i686-w64-mingw32-windres")
SET(CMAKE_RANLIB         "i686-w64-mingw32-ranlib")

# Configure the behaviour of the find commands
SET(CMAKE_FIND_ROOT_PATH "/usr/i686-w64-mingw32")
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

```

## vendor\GLFW\CMake\i686-w64-mingw32.cmake
```cmake
# Define the environment for cross-compiling with 32-bit MinGW-w64 GCC
SET(CMAKE_SYSTEM_NAME    Windows) # Target system name
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_C_COMPILER     "i686-w64-mingw32-gcc")
SET(CMAKE_CXX_COMPILER   "i686-w64-mingw32-g++")
SET(CMAKE_RC_COMPILER    "i686-w64-mingw32-windres")
SET(CMAKE_RANLIB         "i686-w64-mingw32-ranlib")

# Configure the behaviour of the find commands
SET(CMAKE_FIND_ROOT_PATH "/usr/i686-w64-mingw32")
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

```

## vendor\GLFW\CMake\x86_64-w64-mingw32-clang.cmake
```cmake
# Define the environment for cross-compiling with 64-bit MinGW-w64 Clang
SET(CMAKE_SYSTEM_NAME    Windows) # Target system name
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_C_COMPILER     "x86_64-w64-mingw32-clang")
SET(CMAKE_CXX_COMPILER   "x86_64-w64-mingw32-clang++")
SET(CMAKE_RC_COMPILER    "x86_64-w64-mingw32-windres")
SET(CMAKE_RANLIB         "x86_64-w64-mingw32-ranlib")

# Configure the behaviour of the find commands
SET(CMAKE_FIND_ROOT_PATH "/usr/x86_64-w64-mingw32")
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

```

## vendor\GLFW\CMake\x86_64-w64-mingw32.cmake
```cmake
# Define the environment for cross-compiling with 64-bit MinGW-w64 GCC
SET(CMAKE_SYSTEM_NAME    Windows) # Target system name
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_C_COMPILER     "x86_64-w64-mingw32-gcc")
SET(CMAKE_CXX_COMPILER   "x86_64-w64-mingw32-g++")
SET(CMAKE_RC_COMPILER    "x86_64-w64-mingw32-windres")
SET(CMAKE_RANLIB         "x86_64-w64-mingw32-ranlib")

# Configure the behaviour of the find commands
SET(CMAKE_FIND_ROOT_PATH "/usr/x86_64-w64-mingw32")
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

```

## vendor\GLFW\CMake\modules\FindEpollShim.cmake
```cmake
# Find EpollShim
# Once done, this will define
#
#   EPOLLSHIM_FOUND - System has EpollShim
#   EPOLLSHIM_INCLUDE_DIRS - The EpollShim include directories
#   EPOLLSHIM_LIBRARIES - The libraries needed to use EpollShim

find_path(EPOLLSHIM_INCLUDE_DIRS NAMES sys/epoll.h sys/timerfd.h HINTS /usr/local/include/libepoll-shim)
find_library(EPOLLSHIM_LIBRARIES NAMES epoll-shim libepoll-shim HINTS /usr/local/lib)

if (EPOLLSHIM_INCLUDE_DIRS AND EPOLLSHIM_LIBRARIES)
	set(EPOLLSHIM_FOUND TRUE)
endif (EPOLLSHIM_INCLUDE_DIRS AND EPOLLSHIM_LIBRARIES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(EpollShim DEFAULT_MSG EPOLLSHIM_LIBRARIES EPOLLSHIM_INCLUDE_DIRS)
mark_as_advanced(EPOLLSHIM_INCLUDE_DIRS EPOLLSHIM_LIBRARIES)

```

## vendor\GLFW\CMake\modules\FindOSMesa.cmake
```cmake
# Try to find OSMesa on a Unix system
#
# This will define:
#
#   OSMESA_LIBRARIES   - Link these to use OSMesa
#   OSMESA_INCLUDE_DIR - Include directory for OSMesa
#
# Copyright (c) 2014 Brandon Schaefer <brandon.schaefer@canonical.com>

if (NOT WIN32)

  find_package (PkgConfig)
  pkg_check_modules (PKG_OSMESA QUIET osmesa)

  set (OSMESA_INCLUDE_DIR ${PKG_OSMESA_INCLUDE_DIRS})
  set (OSMESA_LIBRARIES   ${PKG_OSMESA_LIBRARIES})

endif ()

```

## vendor\GLFW\docs\CMakeLists.txt
```cmake

# NOTE: The order of this list determines the order of items in the Guides
#       (i.e. Pages) list in the generated documentation
set(source_files
    main.dox
    news.dox
    quick.dox
    moving.dox
    compile.dox
    build.dox
    intro.dox
    context.dox
    monitor.dox
    window.dox
    input.dox
    vulkan.dox
    compat.dox
    internal.dox)

set(extra_files DoxygenLayout.xml header.html footer.html extra.css spaces.svg)

set(header_paths
    "${GLFW_SOURCE_DIR}/include/GLFW/glfw3.h"
    "${GLFW_SOURCE_DIR}/include/GLFW/glfw3native.h")

# Format the source list into a Doxyfile INPUT value that Doxygen can parse
foreach(path IN LISTS header_paths)
    string(APPEND GLFW_DOXYGEN_INPUT " \\\n\"${path}\"")
endforeach()
foreach(file IN LISTS source_files)
    string(APPEND GLFW_DOXYGEN_INPUT " \\\n\"${CMAKE_CURRENT_SOURCE_DIR}/${file}\"")
endforeach()

configure_file(Doxyfile.in Doxyfile @ONLY)

add_custom_command(OUTPUT "html/index.html"
                   COMMAND "${DOXYGEN_EXECUTABLE}"
                   WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
                   MAIN_DEPENDENCY Doxyfile
                   DEPENDS ${header_paths} ${source_files} ${extra_files}
                   COMMENT "Generating HTML documentation"
                   VERBATIM)

add_custom_target(docs ALL SOURCES "html/index.html")
set_target_properties(docs PROPERTIES FOLDER "GLFW3")


```

## vendor\GLFW\examples\CMakeLists.txt
```cmake

link_libraries(glfw)

include_directories("${GLFW_SOURCE_DIR}/deps")

if (MATH_LIBRARY)
    link_libraries("${MATH_LIBRARY}")
endif()

# Workaround for the MS CRT deprecating parts of the standard library
if (MSVC OR CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if (WIN32)
    set(ICON glfw.rc)
elseif (APPLE)
    set(ICON glfw.icns)
endif()

set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h")
set(GLAD_GLES2 "${GLFW_SOURCE_DIR}/deps/glad/gles2.h")
set(GETOPT "${GLFW_SOURCE_DIR}/deps/getopt.h"
           "${GLFW_SOURCE_DIR}/deps/getopt.c")
set(TINYCTHREAD "${GLFW_SOURCE_DIR}/deps/tinycthread.h"
                "${GLFW_SOURCE_DIR}/deps/tinycthread.c")

add_executable(boing WIN32 MACOSX_BUNDLE boing.c ${ICON} ${GLAD_GL})
add_executable(gears WIN32 MACOSX_BUNDLE gears.c ${ICON} ${GLAD_GL})
add_executable(heightmap WIN32 MACOSX_BUNDLE heightmap.c ${ICON} ${GLAD_GL})
add_executable(offscreen offscreen.c ${ICON} ${GLAD_GL})
add_executable(particles WIN32 MACOSX_BUNDLE particles.c ${ICON} ${TINYCTHREAD} ${GETOPT} ${GLAD_GL})
add_executable(sharing WIN32 MACOSX_BUNDLE sharing.c ${ICON} ${GLAD_GL})
add_executable(splitview WIN32 MACOSX_BUNDLE splitview.c ${ICON} ${GLAD_GL})
add_executable(triangle-opengl WIN32 MACOSX_BUNDLE triangle-opengl.c ${ICON} ${GLAD_GL})
add_executable(triangle-opengles WIN32 MACOSX_BUNDLE triangle-opengles.c ${ICON} ${GLAD_GLES2})
add_executable(wave WIN32 MACOSX_BUNDLE wave.c ${ICON} ${GLAD_GL})
add_executable(windows WIN32 MACOSX_BUNDLE windows.c ${ICON} ${GLAD_GL})

target_link_libraries(particles Threads::Threads)
if (RT_LIBRARY)
    target_link_libraries(particles "${RT_LIBRARY}")
endif()

set(GUI_ONLY_BINARIES boing gears heightmap particles sharing splitview
    triangle-opengl triangle-opengles wave windows)
set(CONSOLE_BINARIES offscreen)

set_target_properties(${GUI_ONLY_BINARIES} ${CONSOLE_BINARIES} PROPERTIES
                      C_STANDARD 99
                      FOLDER "GLFW3/Examples")

if (GLFW_USE_OSMESA)
    find_package(OSMesa REQUIRED)
    target_compile_definitions(offscreen PRIVATE USE_NATIVE_OSMESA)
endif()

if (MSVC)
    # Tell MSVC to use main instead of WinMain
    set_target_properties(${GUI_ONLY_BINARIES} PROPERTIES
                          LINK_FLAGS "/ENTRY:mainCRTStartup")
elseif (CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
    # Tell Clang using MS CRT to use main instead of WinMain
    set_target_properties(${GUI_ONLY_BINARIES} PROPERTIES
                          LINK_FLAGS "-Wl,/entry:mainCRTStartup")
endif()

if (APPLE)
    set_target_properties(boing PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Boing")
    set_target_properties(gears PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Gears")
    set_target_properties(heightmap PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Heightmap")
    set_target_properties(particles PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Particles")
    set_target_properties(sharing PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Sharing")
    set_target_properties(triangle-opengl PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "OpenGL Triangle")
    set_target_properties(triangle-opengles PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "OpenGL ES Triangle")
    set_target_properties(splitview PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "SplitView")
    set_target_properties(wave PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Wave")
    set_target_properties(windows PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Windows")

    set_source_files_properties(glfw.icns PROPERTIES
                                MACOSX_PACKAGE_LOCATION "Resources")
    set_target_properties(${GUI_ONLY_BINARIES} PROPERTIES
                          MACOSX_BUNDLE_SHORT_VERSION_STRING ${GLFW_VERSION}
                          MACOSX_BUNDLE_LONG_VERSION_STRING ${GLFW_VERSION}
                          MACOSX_BUNDLE_ICON_FILE glfw.icns
                          MACOSX_BUNDLE_INFO_PLIST "${GLFW_SOURCE_DIR}/CMake/Info.plist.in")
endif()


```

## vendor\GLFW\src\CMakeLists.txt
```cmake

add_library(glfw ${GLFW_LIBRARY_TYPE}
                 "${GLFW_SOURCE_DIR}/include/GLFW/glfw3.h"
                 "${GLFW_SOURCE_DIR}/include/GLFW/glfw3native.h"
                 internal.h platform.h mappings.h
                 context.c init.c input.c monitor.c platform.c vulkan.c window.c
                 egl_context.c osmesa_context.c null_platform.h null_joystick.h
                 null_init.c null_monitor.c null_window.c null_joystick.c)

# The time, thread and module code is shared between all backends on a given OS,
# including the null backend, which still needs those bits to be functional
if (APPLE)
    target_sources(glfw PRIVATE cocoa_time.h cocoa_time.c posix_thread.h
                                posix_module.c posix_thread.c)
elseif (WIN32)
    target_sources(glfw PRIVATE win32_time.h win32_thread.h win32_module.c
                                win32_time.c win32_thread.c)
else()
    target_sources(glfw PRIVATE posix_time.h posix_thread.h posix_module.c
                                posix_time.c posix_thread.c)
endif()

add_custom_target(update_mappings
    COMMAND "${CMAKE_COMMAND}" -P "${GLFW_SOURCE_DIR}/CMake/GenerateMappings.cmake" mappings.h.in mappings.h
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Updating gamepad mappings from upstream repository"
    SOURCES mappings.h.in "${GLFW_SOURCE_DIR}/CMake/GenerateMappings.cmake"
    VERBATIM)

set_target_properties(update_mappings PROPERTIES FOLDER "GLFW3")

if (GLFW_BUILD_COCOA)
    target_compile_definitions(glfw PRIVATE _GLFW_COCOA)
    target_sources(glfw PRIVATE cocoa_platform.h cocoa_joystick.h cocoa_init.m
                                cocoa_joystick.m cocoa_monitor.m cocoa_window.m
                                nsgl_context.m)
endif()

if (GLFW_BUILD_WIN32)
    target_compile_definitions(glfw PRIVATE _GLFW_WIN32)
    target_sources(glfw PRIVATE win32_platform.h win32_joystick.h win32_init.c
                                win32_joystick.c win32_monitor.c win32_window.c
                                wgl_context.c)
endif()

if (GLFW_BUILD_X11)
    target_compile_definitions(glfw PRIVATE _GLFW_X11)
    target_sources(glfw PRIVATE x11_platform.h xkb_unicode.h x11_init.c
                                x11_monitor.c x11_window.c xkb_unicode.c
                                glx_context.c)
endif()

if (GLFW_BUILD_WAYLAND)
    target_compile_definitions(glfw PRIVATE _GLFW_WAYLAND)
    target_sources(glfw PRIVATE wl_platform.h xkb_unicode.h wl_init.c
                                wl_monitor.c wl_window.c xkb_unicode.c)
endif()

if (GLFW_BUILD_X11 OR GLFW_BUILD_WAYLAND)
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_sources(glfw PRIVATE linux_joystick.h linux_joystick.c)
    endif()
endif()

if (GLFW_BUILD_WAYLAND)
    include(CheckIncludeFiles)
    include(CheckFunctionExists)
    check_function_exists(memfd_create HAVE_MEMFD_CREATE)
    if (HAVE_MEMFD_CREATE)
        target_compile_definitions(glfw PRIVATE HAVE_MEMFD_CREATE)
    endif()

    find_program(WAYLAND_SCANNER_EXECUTABLE NAMES wayland-scanner)

    include(FindPkgConfig)
    pkg_check_modules(WAYLAND_PROTOCOLS REQUIRED wayland-protocols>=1.15)
    pkg_get_variable(WAYLAND_PROTOCOLS_BASE wayland-protocols pkgdatadir)
    pkg_get_variable(WAYLAND_CLIENT_PKGDATADIR wayland-client pkgdatadir)

    macro(wayland_generate protocol_file output_file)
        add_custom_command(OUTPUT "${output_file}.h"
            COMMAND "${WAYLAND_SCANNER_EXECUTABLE}" client-header "${protocol_file}" "${output_file}.h"
            DEPENDS "${protocol_file}"
            VERBATIM)

        add_custom_command(OUTPUT "${output_file}-code.h"
            COMMAND "${WAYLAND_SCANNER_EXECUTABLE}" private-code "${protocol_file}" "${output_file}-code.h"
            DEPENDS "${protocol_file}"
            VERBATIM)

        target_sources(glfw PRIVATE "${output_file}.h" "${output_file}-code.h")
    endmacro()

    wayland_generate(
        "${WAYLAND_CLIENT_PKGDATADIR}/wayland.xml"
        "${GLFW_BINARY_DIR}/src/wayland-client-protocol")
    wayland_generate(
        "${WAYLAND_PROTOCOLS_BASE}/stable/xdg-shell/xdg-shell.xml"
        "${GLFW_BINARY_DIR}/src/wayland-xdg-shell-client-protocol")
    wayland_generate(
        "${WAYLAND_PROTOCOLS_BASE}/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml"
        "${GLFW_BINARY_DIR}/src/wayland-xdg-decoration-client-protocol")
    wayland_generate(
        "${WAYLAND_PROTOCOLS_BASE}/stable/viewporter/viewporter.xml"
        "${GLFW_BINARY_DIR}/src/wayland-viewporter-client-protocol")
    wayland_generate(
        "${WAYLAND_PROTOCOLS_BASE}/unstable/relative-pointer/relative-pointer-unstable-v1.xml"
        "${GLFW_BINARY_DIR}/src/wayland-relative-pointer-unstable-v1-client-protocol")
    wayland_generate(
        "${WAYLAND_PROTOCOLS_BASE}/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml"
        "${GLFW_BINARY_DIR}/src/wayland-pointer-constraints-unstable-v1-client-protocol")
    wayland_generate(
        "${WAYLAND_PROTOCOLS_BASE}/unstable/idle-inhibit/idle-inhibit-unstable-v1.xml"
        "${GLFW_BINARY_DIR}/src/wayland-idle-inhibit-unstable-v1-client-protocol")
endif()

if (WIN32 AND GLFW_BUILD_SHARED_LIBRARY)
    configure_file(glfw.rc.in glfw.rc @ONLY)
    target_sources(glfw PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/glfw.rc")
endif()

if (UNIX AND GLFW_BUILD_SHARED_LIBRARY)
    # On Unix-like systems, shared libraries can use the soname system.
    set(GLFW_LIB_NAME glfw)
else()
    set(GLFW_LIB_NAME glfw3)
endif()

set_target_properties(glfw PROPERTIES
                      OUTPUT_NAME ${GLFW_LIB_NAME}
                      VERSION ${GLFW_VERSION_MAJOR}.${GLFW_VERSION_MINOR}
                      SOVERSION ${GLFW_VERSION_MAJOR}
                      POSITION_INDEPENDENT_CODE ON
                      C_STANDARD 99
                      C_EXTENSIONS OFF
                      DEFINE_SYMBOL _GLFW_BUILD_DLL
                      FOLDER "GLFW3")

target_include_directories(glfw PUBLIC
                           "$<BUILD_INTERFACE:${GLFW_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_include_directories(glfw PRIVATE
                           "${GLFW_SOURCE_DIR}/src"
                           "${GLFW_BINARY_DIR}/src")
target_link_libraries(glfw PRIVATE Threads::Threads)

# Workaround for CMake not knowing about .m files before version 3.16
if (CMAKE_VERSION VERSION_LESS "3.16" AND APPLE)
    set_source_files_properties(cocoa_init.m cocoa_joystick.m cocoa_monitor.m
                                cocoa_window.m nsgl_context.m PROPERTIES
                                LANGUAGE C)
endif()

if (GLFW_BUILD_WIN32)
    list(APPEND glfw_PKG_LIBS "-lgdi32")
endif()

if (GLFW_BUILD_COCOA)
    target_link_libraries(glfw PRIVATE "-framework Cocoa"
                                       "-framework IOKit"
                                       "-framework CoreFoundation")

    set(glfw_PKG_DEPS "")
    set(glfw_PKG_LIBS "-framework Cocoa -framework IOKit -framework CoreFoundation")
endif()

if (GLFW_BUILD_WAYLAND)
    pkg_check_modules(Wayland REQUIRED
        wayland-client>=0.2.7
        wayland-cursor>=0.2.7
        wayland-egl>=0.2.7
        xkbcommon>=0.5.0)

    target_include_directories(glfw PRIVATE ${Wayland_INCLUDE_DIRS})

    if (NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
        find_package(EpollShim)
        if (EPOLLSHIM_FOUND)
            target_include_directories(glfw PRIVATE ${EPOLLSHIM_INCLUDE_DIRS})
            target_link_libraries(glfw PRIVATE ${EPOLLSHIM_LIBRARIES})
        endif()
    endif()
endif()

if (GLFW_BUILD_X11)
    find_package(X11 REQUIRED)
    target_include_directories(glfw PRIVATE "${X11_X11_INCLUDE_PATH}")

    # Check for XRandR (modern resolution switching and gamma control)
    if (NOT X11_Xrandr_INCLUDE_PATH)
        message(FATAL_ERROR "RandR headers not found; install libxrandr development package")
    endif()
    target_include_directories(glfw PRIVATE "${X11_Xrandr_INCLUDE_PATH}")

    # Check for Xinerama (legacy multi-monitor support)
    if (NOT X11_Xinerama_INCLUDE_PATH)
        message(FATAL_ERROR "Xinerama headers not found; install libxinerama development package")
    endif()
    target_include_directories(glfw PRIVATE "${X11_Xinerama_INCLUDE_PATH}")

    # Check for Xkb (X keyboard extension)
    if (NOT X11_Xkb_INCLUDE_PATH)
        message(FATAL_ERROR "XKB headers not found; install X11 development package")
    endif()
    target_include_directories(glfw PRIVATE "${X11_Xkb_INCLUDE_PATH}")

    # Check for Xcursor (cursor creation from RGBA images)
    if (NOT X11_Xcursor_INCLUDE_PATH)
        message(FATAL_ERROR "Xcursor headers not found; install libxcursor development package")
    endif()
    target_include_directories(glfw PRIVATE "${X11_Xcursor_INCLUDE_PATH}")

    # Check for XInput (modern HID input)
    if (NOT X11_Xi_INCLUDE_PATH)
        message(FATAL_ERROR "XInput headers not found; install libxi development package")
    endif()
    target_include_directories(glfw PRIVATE "${X11_Xi_INCLUDE_PATH}")

    # Check for X Shape (custom window input shape)
    if (NOT X11_Xshape_INCLUDE_PATH)
        message(FATAL_ERROR "X Shape headers not found; install libxext development package")
    endif()
    target_include_directories(glfw PRIVATE "${X11_Xshape_INCLUDE_PATH}")
endif()

if (UNIX AND NOT APPLE)
    find_library(RT_LIBRARY rt)
    mark_as_advanced(RT_LIBRARY)
    if (RT_LIBRARY)
        target_link_libraries(glfw PRIVATE "${RT_LIBRARY}")
        list(APPEND glfw_PKG_LIBS "-lrt")
    endif()

    find_library(MATH_LIBRARY m)
    mark_as_advanced(MATH_LIBRARY)
    if (MATH_LIBRARY)
        target_link_libraries(glfw PRIVATE "${MATH_LIBRARY}")
        list(APPEND glfw_PKG_LIBS "-lm")
    endif()

    if (CMAKE_DL_LIBS)
        target_link_libraries(glfw PRIVATE "${CMAKE_DL_LIBS}")
        list(APPEND glfw_PKG_LIBS "-l${CMAKE_DL_LIBS}")
    endif()
endif()

# Make GCC warn about declarations that VS 2010 and 2012 won't accept for all
# source files that VS will build (Clang ignores this because we set -std=c99)
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set_source_files_properties(context.c init.c input.c monitor.c platform.c vulkan.c
                                window.c null_init.c null_joystick.c null_monitor.c
                                null_window.c win32_init.c win32_joystick.c win32_module.c
                                win32_monitor.c win32_time.c win32_thread.c win32_window.c
                                wgl_context.c egl_context.c osmesa_context.c PROPERTIES
                                COMPILE_FLAGS -Wdeclaration-after-statement)
endif()

if (WIN32)
    if (GLFW_USE_HYBRID_HPG)
        target_compile_definitions(glfw PRIVATE _GLFW_USE_HYBRID_HPG)
    endif()
endif()

# Enable a reasonable set of warnings
# NOTE: The order matters here, Clang-CL matches both MSVC and Clang
if (MSVC)
    target_compile_options(glfw PRIVATE "/W3")
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
        CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
        CMAKE_C_COMPILER_ID STREQUAL "AppleClang")

    target_compile_options(glfw PRIVATE "-Wall")
endif()

if (GLFW_BUILD_WIN32)
    target_compile_definitions(glfw PRIVATE UNICODE _UNICODE)
endif()

# HACK: When building on MinGW, WINVER and UNICODE need to be defined before
# the inclusion of stddef.h (by glfw3.h), which is itself included before
# win32_platform.h.  We define them here until a saner solution can be found
# NOTE: MinGW-w64 and Visual C++ do /not/ need this hack.
if (MINGW)
    target_compile_definitions(glfw PRIVATE WINVER=0x0501)
endif()

# Workaround for legacy MinGW not providing XInput and DirectInput
if (MINGW)
    include(CheckIncludeFile)
    check_include_file(dinput.h DINPUT_H_FOUND)
    check_include_file(xinput.h XINPUT_H_FOUND)
    if (NOT DINPUT_H_FOUND OR NOT XINPUT_H_FOUND)
        target_include_directories(glfw PRIVATE "${GLFW_SOURCE_DIR}/deps/mingw")
    endif()
endif()

# Workaround for the MS CRT deprecating parts of the standard library
if (MSVC OR CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
    target_compile_definitions(glfw PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Workaround for VS 2008 not shipping with stdint.h
if (MSVC90)
    target_include_directories(glfw PUBLIC "${GLFW_SOURCE_DIR}/deps/vs2008")
endif()

# Check for the DirectX 9 SDK as it is not included with VS 2008
if (MSVC90)
    include(CheckIncludeFile)
    check_include_file(dinput.h DINPUT_H_FOUND)
    if (NOT DINPUT_H_FOUND)
        message(FATAL_ERROR "DirectX 9 headers not found; install DirectX 9 SDK")
    endif()
endif()

# Workaround for -std=c99 on Linux disabling _DEFAULT_SOURCE (POSIX 2008 and more)
if (GLFW_BUILD_X11 OR GLFW_BUILD_WAYLAND)
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_compile_definitions(glfw PRIVATE _DEFAULT_SOURCE)
    endif()
endif()

if (GLFW_BUILD_SHARED_LIBRARY)
    if (WIN32)
        if (MINGW)
            # Remove the dependency on the shared version of libgcc
            # NOTE: MinGW-w64 has the correct default but MinGW needs this
            target_link_libraries(glfw PRIVATE "-static-libgcc")

            # Remove the lib prefix on the DLL (but not the import library)
            set_target_properties(glfw PROPERTIES PREFIX "")

            # Add a suffix to the import library to avoid naming conflicts
            set_target_properties(glfw PROPERTIES IMPORT_SUFFIX "dll.a")
        else()
            # Add a suffix to the import library to avoid naming conflicts
            set_target_properties(glfw PROPERTIES IMPORT_SUFFIX "dll.lib")
        endif()

        target_compile_definitions(glfw INTERFACE GLFW_DLL)
    endif()

    if (MINGW)
        # Enable link-time exploit mitigation features enabled by default on MSVC
        include(CheckCCompilerFlag)

        # Compatibility with data execution prevention (DEP)
        set(CMAKE_REQUIRED_FLAGS "-Wl,--nxcompat")
        check_c_compiler_flag("" _GLFW_HAS_DEP)
        if (_GLFW_HAS_DEP)
            target_link_libraries(glfw PRIVATE "-Wl,--nxcompat")
        endif()

        # Compatibility with address space layout randomization (ASLR)
        set(CMAKE_REQUIRED_FLAGS "-Wl,--dynamicbase")
        check_c_compiler_flag("" _GLFW_HAS_ASLR)
        if (_GLFW_HAS_ASLR)
            target_link_libraries(glfw PRIVATE "-Wl,--dynamicbase")
        endif()

        # Compatibility with 64-bit address space layout randomization (ASLR)
        set(CMAKE_REQUIRED_FLAGS "-Wl,--high-entropy-va")
        check_c_compiler_flag("" _GLFW_HAS_64ASLR)
        if (_GLFW_HAS_64ASLR)
            target_link_libraries(glfw PRIVATE "-Wl,--high-entropy-va")
        endif()

        # Clear flags again to avoid breaking later tests
        set(CMAKE_REQUIRED_FLAGS)
    endif()

    if (UNIX)
        # Hide symbols not explicitly tagged for export from the shared library
        target_compile_options(glfw PRIVATE "-fvisibility=hidden")
    endif()
endif()

foreach(arg ${glfw_PKG_DEPS})
    string(APPEND deps " ${arg}")
endforeach()
foreach(arg ${glfw_PKG_LIBS})
    string(APPEND libs " ${arg}")
endforeach()

set(GLFW_PKG_CONFIG_REQUIRES_PRIVATE "${deps}" CACHE INTERNAL
    "GLFW pkg-config Requires.private")
set(GLFW_PKG_CONFIG_LIBS_PRIVATE "${libs}" CACHE INTERNAL
    "GLFW pkg-config Libs.private")

configure_file("${GLFW_SOURCE_DIR}/CMake/glfw3.pc.in" glfw3.pc @ONLY)

if (GLFW_INSTALL)
    install(TARGETS glfw
            EXPORT glfwTargets
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()


```

## vendor\GLFW\tests\CMakeLists.txt
```cmake

link_libraries(glfw)

include_directories("${GLFW_SOURCE_DIR}/deps")

if (MATH_LIBRARY)
    link_libraries("${MATH_LIBRARY}")
endif()

# Workaround for the MS CRT deprecating parts of the standard library
if (MSVC OR CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h")
set(GLAD_VULKAN "${GLFW_SOURCE_DIR}/deps/glad/vulkan.h")
set(GETOPT "${GLFW_SOURCE_DIR}/deps/getopt.h"
           "${GLFW_SOURCE_DIR}/deps/getopt.c")
set(TINYCTHREAD "${GLFW_SOURCE_DIR}/deps/tinycthread.h"
                "${GLFW_SOURCE_DIR}/deps/tinycthread.c")

add_executable(allocator allocator.c ${GLAD_GL})
add_executable(clipboard clipboard.c ${GETOPT} ${GLAD_GL})
add_executable(events events.c ${GETOPT} ${GLAD_GL})
add_executable(msaa msaa.c ${GETOPT} ${GLAD_GL})
add_executable(glfwinfo glfwinfo.c ${GETOPT} ${GLAD_GL} ${GLAD_VULKAN})
add_executable(iconify iconify.c ${GETOPT} ${GLAD_GL})
add_executable(monitors monitors.c ${GETOPT} ${GLAD_GL})
add_executable(reopen reopen.c ${GLAD_GL})
add_executable(cursor cursor.c ${GLAD_GL})

add_executable(empty WIN32 MACOSX_BUNDLE empty.c ${TINYCTHREAD} ${GLAD_GL})
add_executable(gamma WIN32 MACOSX_BUNDLE gamma.c ${GLAD_GL})
add_executable(icon WIN32 MACOSX_BUNDLE icon.c ${GLAD_GL})
add_executable(inputlag WIN32 MACOSX_BUNDLE inputlag.c ${GETOPT} ${GLAD_GL})
add_executable(joysticks WIN32 MACOSX_BUNDLE joysticks.c ${GLAD_GL})
add_executable(tearing WIN32 MACOSX_BUNDLE tearing.c ${GLAD_GL})
add_executable(threads WIN32 MACOSX_BUNDLE threads.c ${TINYCTHREAD} ${GLAD_GL})
add_executable(timeout WIN32 MACOSX_BUNDLE timeout.c ${GLAD_GL})
add_executable(title WIN32 MACOSX_BUNDLE title.c ${GLAD_GL})
add_executable(triangle-vulkan WIN32 triangle-vulkan.c ${GLAD_VULKAN})
add_executable(window WIN32 MACOSX_BUNDLE window.c ${GLAD_GL})

target_link_libraries(empty Threads::Threads)
target_link_libraries(threads Threads::Threads)
if (RT_LIBRARY)
    target_link_libraries(empty "${RT_LIBRARY}")
    target_link_libraries(threads "${RT_LIBRARY}")
endif()

set(GUI_ONLY_BINARIES empty gamma icon inputlag joysticks tearing threads
    timeout title triangle-vulkan window)
set(CONSOLE_BINARIES allocator clipboard events msaa glfwinfo iconify monitors
    reopen cursor)

set_target_properties(${GUI_ONLY_BINARIES} ${CONSOLE_BINARIES} PROPERTIES
                      C_STANDARD 99
                      FOLDER "GLFW3/Tests")

if (MSVC)
    # Tell MSVC to use main instead of WinMain
    set_target_properties(${GUI_ONLY_BINARIES} PROPERTIES
                          LINK_FLAGS "/ENTRY:mainCRTStartup")
elseif (CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
    # Tell Clang using MS CRT to use main instead of WinMain
    set_target_properties(${GUI_ONLY_BINARIES} PROPERTIES
                          LINK_FLAGS "-Wl,/entry:mainCRTStartup")
endif()

if (APPLE)
    set_target_properties(empty PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Empty Event")
    set_target_properties(gamma PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Gamma")
    set_target_properties(inputlag PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Input Lag")
    set_target_properties(joysticks PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Joysticks")
    set_target_properties(tearing PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Tearing")
    set_target_properties(threads PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Threads")
    set_target_properties(timeout PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Timeout")
    set_target_properties(title PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Title")
    set_target_properties(window PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Window")

    set_target_properties(${GUI_ONLY_BINARIES} PROPERTIES
                          MACOSX_BUNDLE_SHORT_VERSION_STRING ${GLFW_VERSION}
                          MACOSX_BUNDLE_LONG_VERSION_STRING ${GLFW_VERSION}
                          MACOSX_BUNDLE_INFO_PLIST "${GLFW_SOURCE_DIR}/CMake/Info.plist.in")
endif()


```

## vendor\glm\CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.6 FATAL_ERROR)
cmake_policy(VERSION 3.6)

file(READ "glm/detail/setup.hpp" GLM_SETUP_FILE)
string(REGEX MATCH "#define[ ]+GLM_VERSION_MAJOR[ ]+([0-9]+)" _ ${GLM_SETUP_FILE})
set(GLM_VERSION_MAJOR "${CMAKE_MATCH_1}")
string(REGEX MATCH "#define[ ]+GLM_VERSION_MINOR[ ]+([0-9]+)" _ ${GLM_SETUP_FILE})
set(GLM_VERSION_MINOR "${CMAKE_MATCH_1}")
string(REGEX MATCH "#define[ ]+GLM_VERSION_PATCH[ ]+([0-9]+)" _ ${GLM_SETUP_FILE})
set(GLM_VERSION_PATCH "${CMAKE_MATCH_1}")
string(REGEX MATCH "#define[ ]+GLM_VERSION_REVISION[ ]+([0-9]+)" _ ${GLM_SETUP_FILE})
set(GLM_VERSION_REVISION "${CMAKE_MATCH_1}")

set(GLM_VERSION ${GLM_VERSION_MAJOR}.${GLM_VERSION_MINOR}.${GLM_VERSION_PATCH})
project(glm VERSION ${GLM_VERSION} LANGUAGES CXX)
message(STATUS "GLM: Version " ${GLM_VERSION})

set(GLM_IS_MASTER_PROJECT OFF)
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	set(GLM_IS_MASTER_PROJECT ON)
endif()

option(GLM_BUILD_LIBRARY "Build dynamic/static library" ON)
option(GLM_BUILD_TESTS "Build the test programs" ${GLM_IS_MASTER_PROJECT})
option(GLM_BUILD_INSTALL "Generate the install target" ${GLM_IS_MASTER_PROJECT})

include(GNUInstallDirs)

option(GLM_ENABLE_CXX_98 "Enable C++ 98" OFF)
option(GLM_ENABLE_CXX_11 "Enable C++ 11" OFF)
option(GLM_ENABLE_CXX_14 "Enable C++ 14" OFF)
option(GLM_ENABLE_CXX_17 "Enable C++ 17" OFF)
option(GLM_ENABLE_CXX_20 "Enable C++ 20" OFF)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(GLM_ENABLE_CXX_20)
	set(CMAKE_CXX_STANDARD 20)
	add_definitions(-DGLM_FORCE_CXX20)
	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		message(STATUS "GLM: Disable -Wc++98-compat warnings")
		add_compile_options(-Wno-c++98-compat)
		add_compile_options(-Wno-c++98-compat-pedantic)
	endif()
	if(NOT GLM_QUIET)
		message(STATUS "GLM: Build with C++20 features")
	endif()

elseif(GLM_ENABLE_CXX_17)
	set(CMAKE_CXX_STANDARD 17)
	add_definitions(-DGLM_FORCE_CXX17)
	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		message(STATUS "GLM: Disable -Wc++98-compat warnings")
		add_compile_options(-Wno-c++98-compat)
		add_compile_options(-Wno-c++98-compat-pedantic)
	endif()
	if(NOT GLM_QUIET)
		message(STATUS "GLM: Build with C++17 features")
	endif()

elseif(GLM_ENABLE_CXX_14)
	set(CMAKE_CXX_STANDARD 14)
	add_definitions(-DGLM_FORCE_CXX14)
	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		message(STATUS "GLM: Disable -Wc++98-compat warnings")
		add_compile_options(-Wno-c++98-compat)
		add_compile_options(-Wno-c++98-compat-pedantic)
	endif()
	if(NOT GLM_QUIET)
		message(STATUS "GLM: Build with C++14 features")
	endif()

elseif(GLM_ENABLE_CXX_11)
	set(CMAKE_CXX_STANDARD 11)
	add_definitions(-DGLM_FORCE_CXX11)
	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		message(STATUS "GLM: Disable -Wc++98-compat warnings")
		add_compile_options(-Wno-c++98-compat)
		add_compile_options(-Wno-c++98-compat-pedantic)
	endif()
	if(NOT GLM_QUIET)
		message(STATUS "GLM: Build with C++11 features")
	endif()

elseif(GLM_ENABLE_CXX_98)
	set(CMAKE_CXX_STANDARD 98)
	add_definitions(-DGLM_FORCE_CXX98)
	if(NOT GLM_QUIET)
		message(STATUS "GLM: Build with C++98 features")
	endif()

else()
	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		message(STATUS "GLM: Disable -Wc++98-compat warnings")
		add_compile_options(-Wno-c++98-compat)
		add_compile_options(-Wno-c++98-compat-pedantic)
	endif()
	if(NOT GLM_QUIET)
		message(STATUS "GLM: Build with C++ features auto detection")
	endif()

endif()

option(GLM_ENABLE_LANG_EXTENSIONS "Enable language extensions" OFF)
option(GLM_DISABLE_AUTO_DETECTION "Disable platform, compiler, arch and C++ language detection" OFF)

if(GLM_DISABLE_AUTO_DETECTION)
	add_definitions(-DGLM_FORCE_PLATFORM_UNKNOWN -DGLM_FORCE_COMPILER_UNKNOWN -DGLM_FORCE_ARCH_UNKNOWN -DGLM_FORCE_CXX_UNKNOWN)
endif()

if(GLM_ENABLE_LANG_EXTENSIONS)
	set(CMAKE_CXX_EXTENSIONS ON)
	if((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR (CMAKE_CXX_COMPILER_ID MATCHES "GNU"))
		add_compile_options(-fms-extensions)
	endif()
	message(STATUS "GLM: Build with C++ language extensions")
else()
	set(CMAKE_CXX_EXTENSIONS OFF)
	if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		add_compile_options(/Za)
		if(MSVC15)
			add_compile_options(/permissive-)
		endif()
	endif()
endif()

option(GLM_ENABLE_FAST_MATH "Enable fast math optimizations" OFF)
if(GLM_ENABLE_FAST_MATH)
	if(NOT GLM_QUIET)
		message(STATUS "GLM: Build with fast math optimizations")
	endif()

	if((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR (CMAKE_CXX_COMPILER_ID MATCHES "GNU"))
		add_compile_options(-ffast-math)

	elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		add_compile_options(/fp:fast)
	endif()
else()
	if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		add_compile_options(/fp:precise)
	endif()
endif()

option(GLM_ENABLE_SIMD_SSE2 "Enable SSE2 optimizations" OFF)
option(GLM_ENABLE_SIMD_SSE3 "Enable SSE3 optimizations" OFF)
option(GLM_ENABLE_SIMD_SSSE3 "Enable SSSE3 optimizations" OFF)
option(GLM_ENABLE_SIMD_SSE4_1 "Enable SSE 4.1 optimizations" OFF)
option(GLM_ENABLE_SIMD_SSE4_2 "Enable SSE 4.2 optimizations" OFF)
option(GLM_ENABLE_SIMD_AVX "Enable AVX optimizations" OFF)
option(GLM_ENABLE_SIMD_AVX2 "Enable AVX2 optimizations" OFF)
option(GLM_FORCE_PURE "Force 'pure' instructions" OFF)

if(GLM_FORCE_PURE)
	add_definitions(-DGLM_FORCE_PURE)

	if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
		add_compile_options(-mfpmath=387)
	endif()
	message(STATUS "GLM: No SIMD instruction set")

elseif(GLM_ENABLE_SIMD_AVX2)
	add_definitions(-DGLM_FORCE_INTRINSICS)

	if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
		add_compile_options(-mavx2)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
		add_compile_options(/QxAVX2)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		add_compile_options(/arch:AVX2)
	endif()
	message(STATUS "GLM: AVX2 instruction set")

elseif(GLM_ENABLE_SIMD_AVX)
	add_definitions(-DGLM_FORCE_INTRINSICS)

	if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
		add_compile_options(-mavx)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
		add_compile_options(/QxAVX)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		add_compile_options(/arch:AVX)
	endif()
	message(STATUS "GLM: AVX instruction set")

elseif(GLM_ENABLE_SIMD_SSE4_2)
	add_definitions(-DGLM_FORCE_INTRINSICS)

	if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
		add_compile_options(-msse4.2)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
		add_compile_options(/QxSSE4.2)
	elseif((CMAKE_CXX_COMPILER_ID MATCHES "MSVC") AND NOT CMAKE_CL_64)
		add_compile_options(/arch:SSE2) # VC doesn't support SSE4.2
	endif()
	message(STATUS "GLM: SSE4.2 instruction set")

elseif(GLM_ENABLE_SIMD_SSE4_1)
	add_definitions(-DGLM_FORCE_INTRINSICS)

	if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
		add_compile_options(-msse4.1)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
		add_compile_options(/QxSSE4.1)
	elseif((CMAKE_CXX_COMPILER_ID MATCHES "MSVC") AND NOT CMAKE_CL_64)
		add_compile_options(/arch:SSE2) # VC doesn't support SSE4.1
	endif()
	message(STATUS "GLM: SSE4.1 instruction set")

elseif(GLM_ENABLE_SIMD_SSSE3)
	add_definitions(-DGLM_FORCE_INTRINSICS)

	if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
		add_compile_options(-mssse3)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
		add_compile_options(/QxSSSE3)
	elseif((CMAKE_CXX_COMPILER_ID MATCHES "MSVC") AND NOT CMAKE_CL_64)
		add_compile_options(/arch:SSE2) # VC doesn't support SSSE3
	endif()
	message(STATUS "GLM: SSSE3 instruction set")

elseif(GLM_ENABLE_SIMD_SSE3)
	add_definitions(-DGLM_FORCE_INTRINSICS)

	if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
		add_compile_options(-msse3)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
		add_compile_options(/QxSSE3)
	elseif((CMAKE_CXX_COMPILER_ID MATCHES "MSVC") AND NOT CMAKE_CL_64)
		add_compile_options(/arch:SSE2) # VC doesn't support SSE3
	endif()
	message(STATUS "GLM: SSE3 instruction set")

elseif(GLM_ENABLE_SIMD_SSE2)
	add_definitions(-DGLM_FORCE_INTRINSICS)

	if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
		add_compile_options(-msse2)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
		add_compile_options(/QxSSE2)
	elseif((CMAKE_CXX_COMPILER_ID MATCHES "MSVC") AND NOT CMAKE_CL_64)
		add_compile_options(/arch:SSE2)
	endif()
	message(STATUS "GLM: SSE2 instruction set")
endif()

add_subdirectory(glm)

if (GLM_BUILD_TESTS)
	include(CTest)
	add_subdirectory(test)
endif()

if (GLM_BUILD_INSTALL)
	include(CPack)

	install(TARGETS glm-header-only glm EXPORT glm)
	install(
		DIRECTORY glm
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
		PATTERN "CMakeLists.txt" EXCLUDE
	)
	install(
		EXPORT glm
		NAMESPACE glm::
		DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/glm"
		FILE glmConfig.cmake
	)
	include(CMakePackageConfigHelpers)
	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/glmConfigVersion.cmake"
		COMPATIBILITY AnyNewerVersion
	)
	install(
		FILES "${CMAKE_CURRENT_BINARY_DIR}/glmConfigVersion.cmake"
		DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/glm"
	)

	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
		"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
		IMMEDIATE @ONLY
	)

	add_custom_target(
		uninstall
		"${CMAKE_COMMAND}" -P
		"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	)
endif()

```

## vendor\glm\glm\CMakeLists.txt
```cmake
file(GLOB ROOT_SOURCE *.cpp)
file(GLOB ROOT_INLINE *.inl)
file(GLOB ROOT_HEADER *.hpp)
file(GLOB ROOT_TEXT ../*.txt)
file(GLOB ROOT_MD ../*.md)
file(GLOB ROOT_NAT ../util/glm.natvis)

file(GLOB_RECURSE CORE_SOURCE ./detail/*.cpp)
file(GLOB_RECURSE CORE_INLINE ./detail/*.inl)
file(GLOB_RECURSE CORE_HEADER ./detail/*.hpp)

file(GLOB_RECURSE EXT_SOURCE ./ext/*.cpp)
file(GLOB_RECURSE EXT_INLINE ./ext/*.inl)
file(GLOB_RECURSE EXT_HEADER ./ext/*.hpp)

file(GLOB_RECURSE GTC_SOURCE ./gtc/*.cpp)
file(GLOB_RECURSE GTC_INLINE ./gtc/*.inl)
file(GLOB_RECURSE GTC_HEADER ./gtc/*.hpp)

file(GLOB_RECURSE GTX_SOURCE ./gtx/*.cpp)
file(GLOB_RECURSE GTX_INLINE ./gtx/*.inl)
file(GLOB_RECURSE GTX_HEADER ./gtx/*.hpp)

file(GLOB_RECURSE SIMD_SOURCE ./simd/*.cpp)
file(GLOB_RECURSE SIMD_INLINE ./simd/*.inl)
file(GLOB_RECURSE SIMD_HEADER ./simd/*.h)

source_group("Text Files" FILES ${ROOT_TEXT} ${ROOT_MD})
source_group("Core Files" FILES ${CORE_SOURCE})
source_group("Core Files" FILES ${CORE_INLINE})
source_group("Core Files" FILES ${CORE_HEADER})
source_group("EXT Files" FILES ${EXT_SOURCE})
source_group("EXT Files" FILES ${EXT_INLINE})
source_group("EXT Files" FILES ${EXT_HEADER})
source_group("GTC Files" FILES ${GTC_SOURCE})
source_group("GTC Files" FILES ${GTC_INLINE})
source_group("GTC Files" FILES ${GTC_HEADER})
source_group("GTX Files" FILES ${GTX_SOURCE})
source_group("GTX Files" FILES ${GTX_INLINE})
source_group("GTX Files" FILES ${GTX_HEADER})
source_group("SIMD Files" FILES ${SIMD_SOURCE})
source_group("SIMD Files" FILES ${SIMD_INLINE})
source_group("SIMD Files" FILES ${SIMD_HEADER})

add_library(glm-header-only INTERFACE)
add_library(glm::glm-header-only ALIAS glm-header-only)

target_include_directories(glm-header-only INTERFACE
	"$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

if (GLM_BUILD_LIBRARY)
	add_library(glm
		${ROOT_TEXT}      ${ROOT_MD}        ${ROOT_NAT}
		${ROOT_SOURCE}    ${ROOT_INLINE}    ${ROOT_HEADER}
		${CORE_SOURCE}    ${CORE_INLINE}    ${CORE_HEADER}
		${EXT_SOURCE}     ${EXT_INLINE}     ${EXT_HEADER}
		${GTC_SOURCE}     ${GTC_INLINE}     ${GTC_HEADER}
		${GTX_SOURCE}     ${GTX_INLINE}     ${GTX_HEADER}
		${SIMD_SOURCE}    ${SIMD_INLINE}    ${SIMD_HEADER}
	)
	add_library(glm::glm ALIAS glm)
	target_link_libraries(glm PUBLIC glm-header-only)
else()
	add_library(glm INTERFACE)
	add_library(glm::glm ALIAS glm)
	target_link_libraries(glm INTERFACE glm-header-only)
endif()

```

## vendor\glm\test\CMakeLists.txt
```cmake
option(GLM_QUIET "No CMake Message" OFF)

option(GLM_TEST_ENABLE "Build unit tests" ON)
option(GLM_PERF_TEST_ENABLE "Build perf tests" OFF)

# Compiler and default options

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	if(NOT GLM_QUIET)
		message("GLM: Clang - ${CMAKE_CXX_COMPILER_ID} compiler")
	endif()

	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	if(NOT GLM_DISABLE_AUTO_DETECTION)
		add_compile_options(-Werror -Weverything)
	endif()

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	if(NOT GLM_QUIET)
		message("GLM: GCC - ${CMAKE_CXX_COMPILER_ID} compiler")
	endif()

	if(NOT GLM_DISABLE_AUTO_DETECTION)
		add_compile_options(-Werror)
#		add_compile_options(-Wpedantic)
#		add_compile_options(-Wall)
#		add_compile_options(-Wextra)
	endif()
	add_compile_options(-O2)
	#add_compile_options(-Wno-long-long)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
	if(NOT GLM_QUIET)
		message("GLM: Intel - ${CMAKE_CXX_COMPILER_ID} compiler")
	endif()

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	if(NOT GLM_QUIET)
		message("GLM: Visual C++ - ${CMAKE_CXX_COMPILER_ID} compiler")
	endif()

	if(NOT GLM_DISABLE_AUTO_DETECTION)
		add_compile_options(/Wall /WX)
		add_compile_options(/wd4464) # warning C4464: relative include path contains '..'
		add_compile_options(/wd4514) # warning C4514: unreferenced inline function has been removed 
		add_compile_options(/wd4365) # warning C4365: signed/unsigned mismatch 
		add_compile_options(/wd5045) # warning C5045: Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
		add_compile_options(/wd5029) # warning C5029: nonstandard extension used: alignment attributes in C++ apply to variables, data members and tag types only
		add_compile_options(/wd4820) # warning C4820: 'test_decl::S1': '3' bytes padding added after data member 'test_decl::S1::A' 
		add_compile_options(/wd4710) # warning C4710: 'std::string glm::detail::format(const char *,...)': function not inlined
		add_compile_options(/wd4626) # warning C4626: 'glm::io::format_punct<CTy>': assignment operator was implicitly defined as deleted
		add_compile_options(/wd4711) # warning C4711: function 'int __cdecl test_vec1(void)' selected for automatic inline expansion 
		add_compile_options(/wd4571) # warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
		add_compile_options(/wd4625) # warning C4625: 'std::codecvt_base': copy constructor was implicitly defined as deleted
		add_compile_options(/wd5026) # warning C5026: 'std::_Generic_error_category': move constructor was implicitly defined as deleted
		add_compile_options(/wd5027) # warning C5027: 'std::_Generic_error_category': move assignment operator was implicitly defined as deleted
		add_compile_options(/wd4774) # warning C4774: 'sprintf_s' : format string expected in argument 3 is not a string literal
	endif()
#	add_compile_options(/wd4309 /wd4324 /wd4389 /wd4127 /wd4267 /wd4146 /wd4201 /wd4464 /wd4514 /wd4701 /wd4820 /wd4365)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

function(glmCreateTestGTC NAME)
	set(SAMPLE_NAME test-${NAME})
	add_executable(${SAMPLE_NAME} ${NAME}.cpp)

	add_test(
		NAME ${SAMPLE_NAME}
		COMMAND $<TARGET_FILE:${SAMPLE_NAME}> )
	target_link_libraries(${SAMPLE_NAME} PRIVATE glm::glm)
endfunction()

if(GLM_TEST_ENABLE)
	add_subdirectory(bug)
	add_subdirectory(core)
	add_subdirectory(ext)
	add_subdirectory(gtc)
	add_subdirectory(gtx)
endif()
if(GLM_PERF_TEST_ENABLE)
	add_subdirectory(perf)
endif()


```

## vendor\glm\test\bug\CMakeLists.txt
```cmake
glmCreateTestGTC(bug_ms_vec_static)

```

## vendor\glm\test\cmake\CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.6 FATAL_ERROR)
cmake_policy(VERSION 3.6)
project(test_find_glm)

find_package(glm REQUIRED)

add_executable(test_find_glm test_find_glm.cpp)
target_link_libraries(test_find_glm glm::glm)


```

## vendor\glm\test\core\CMakeLists.txt
```cmake
glmCreateTestGTC(core_cpp_constexpr)
glmCreateTestGTC(core_cpp_defaulted_ctor)
glmCreateTestGTC(core_force_aligned_gentypes)
glmCreateTestGTC(core_force_ctor_init)
glmCreateTestGTC(core_force_arch_unknown)
glmCreateTestGTC(core_force_compiler_unknown)
glmCreateTestGTC(core_force_explicit_ctor)
glmCreateTestGTC(core_force_inline)
glmCreateTestGTC(core_force_platform_unknown)
glmCreateTestGTC(core_force_pure)
glmCreateTestGTC(core_force_unrestricted_gentype)
glmCreateTestGTC(core_force_xyzw_only)
glmCreateTestGTC(core_force_quat_wxyz)
glmCreateTestGTC(core_type_aligned)
glmCreateTestGTC(core_type_cast)
glmCreateTestGTC(core_type_ctor)
glmCreateTestGTC(core_type_int)
glmCreateTestGTC(core_type_length)
glmCreateTestGTC(core_type_mat2x2)
glmCreateTestGTC(core_type_mat2x3)
glmCreateTestGTC(core_type_mat2x4)
glmCreateTestGTC(core_type_mat3x2)
glmCreateTestGTC(core_type_mat3x3)
glmCreateTestGTC(core_type_mat3x4)
glmCreateTestGTC(core_type_mat4x2)
glmCreateTestGTC(core_type_mat4x3)
glmCreateTestGTC(core_type_mat4x4)
glmCreateTestGTC(core_type_vec1)
glmCreateTestGTC(core_type_vec2)
glmCreateTestGTC(core_type_vec3)
glmCreateTestGTC(core_type_vec4)
glmCreateTestGTC(core_func_common)
glmCreateTestGTC(core_func_exponential)
glmCreateTestGTC(core_func_geometric)
glmCreateTestGTC(core_func_integer)
glmCreateTestGTC(core_func_integer_bit_count)
glmCreateTestGTC(core_func_integer_find_lsb)
glmCreateTestGTC(core_func_integer_find_msb)
glmCreateTestGTC(core_func_matrix)
glmCreateTestGTC(core_func_noise)
glmCreateTestGTC(core_func_packing)
glmCreateTestGTC(core_func_trigonometric)
glmCreateTestGTC(core_func_vector_relational)
glmCreateTestGTC(core_func_swizzle)
glmCreateTestGTC(core_setup_force_cxx_unknown)
glmCreateTestGTC(core_setup_force_cxx98)
glmCreateTestGTC(core_setup_force_cxx03)
glmCreateTestGTC(core_setup_force_size_t_length)
glmCreateTestGTC(core_setup_message)
glmCreateTestGTC(core_setup_platform_unknown)
glmCreateTestGTC(core_setup_precision)

```

## vendor\glm\test\ext\CMakeLists.txt
```cmake
glmCreateTestGTC(ext_matrix_relational)
glmCreateTestGTC(ext_matrix_transform)
glmCreateTestGTC(ext_matrix_common)
glmCreateTestGTC(ext_matrix_integer)
glmCreateTestGTC(ext_matrix_int2x2_sized)
glmCreateTestGTC(ext_matrix_int2x3_sized)
glmCreateTestGTC(ext_matrix_int2x4_sized)
glmCreateTestGTC(ext_matrix_int3x2_sized)
glmCreateTestGTC(ext_matrix_int3x3_sized)
glmCreateTestGTC(ext_matrix_int3x4_sized)
glmCreateTestGTC(ext_matrix_int4x2_sized)
glmCreateTestGTC(ext_matrix_int4x3_sized)
glmCreateTestGTC(ext_matrix_int4x4_sized)
glmCreateTestGTC(ext_matrix_uint2x2_sized)
glmCreateTestGTC(ext_matrix_uint2x3_sized)
glmCreateTestGTC(ext_matrix_uint2x4_sized)
glmCreateTestGTC(ext_matrix_uint3x2_sized)
glmCreateTestGTC(ext_matrix_uint3x3_sized)
glmCreateTestGTC(ext_matrix_uint3x4_sized)
glmCreateTestGTC(ext_matrix_uint4x2_sized)
glmCreateTestGTC(ext_matrix_uint4x3_sized)
glmCreateTestGTC(ext_matrix_uint4x4_sized)
glmCreateTestGTC(ext_quaternion_common)
glmCreateTestGTC(ext_quaternion_exponential)
glmCreateTestGTC(ext_quaternion_geometric)
glmCreateTestGTC(ext_quaternion_relational)
glmCreateTestGTC(ext_quaternion_transform)
glmCreateTestGTC(ext_quaternion_trigonometric)
glmCreateTestGTC(ext_quaternion_type)
glmCreateTestGTC(ext_scalar_common)
glmCreateTestGTC(ext_scalar_constants)
glmCreateTestGTC(ext_scalar_int_sized)
glmCreateTestGTC(ext_scalar_uint_sized)
glmCreateTestGTC(ext_scalar_integer)
glmCreateTestGTC(ext_scalar_ulp)
glmCreateTestGTC(ext_scalar_reciprocal)
glmCreateTestGTC(ext_scalar_relational)
glmCreateTestGTC(ext_vec1)
glmCreateTestGTC(ext_vector_bool1)
glmCreateTestGTC(ext_vector_common)
glmCreateTestGTC(ext_vector_iec559)
glmCreateTestGTC(ext_vector_int1_sized)
glmCreateTestGTC(ext_vector_int2_sized)
glmCreateTestGTC(ext_vector_int3_sized)
glmCreateTestGTC(ext_vector_int4_sized)
glmCreateTestGTC(ext_vector_integer)
glmCreateTestGTC(ext_vector_integer_sized)
glmCreateTestGTC(ext_vector_uint1_sized)
glmCreateTestGTC(ext_vector_uint2_sized)
glmCreateTestGTC(ext_vector_uint3_sized)
glmCreateTestGTC(ext_vector_uint4_sized)
glmCreateTestGTC(ext_vector_reciprocal)
glmCreateTestGTC(ext_vector_relational)
glmCreateTestGTC(ext_vector_ulp)


```

## vendor\glm\test\gtc\CMakeLists.txt
```cmake
glmCreateTestGTC(gtc_bitfield)
glmCreateTestGTC(gtc_color_space)
glmCreateTestGTC(gtc_constants)
glmCreateTestGTC(gtc_epsilon)
glmCreateTestGTC(gtc_integer)
glmCreateTestGTC(gtc_matrix_access)
glmCreateTestGTC(gtc_matrix_integer)
glmCreateTestGTC(gtc_matrix_inverse)
glmCreateTestGTC(gtc_matrix_transform)
glmCreateTestGTC(gtc_noise)
glmCreateTestGTC(gtc_packing)
glmCreateTestGTC(gtc_quaternion)
glmCreateTestGTC(gtc_random)
glmCreateTestGTC(gtc_round)
glmCreateTestGTC(gtc_reciprocal)
glmCreateTestGTC(gtc_type_aligned)
glmCreateTestGTC(gtc_type_precision)
glmCreateTestGTC(gtc_type_ptr)
glmCreateTestGTC(gtc_ulp)
glmCreateTestGTC(gtc_vec1)

```

## vendor\glm\test\gtx\CMakeLists.txt
```cmake
glmCreateTestGTC(gtx)
glmCreateTestGTC(gtx_associated_min_max)
glmCreateTestGTC(gtx_closest_point)
glmCreateTestGTC(gtx_color_encoding)
glmCreateTestGTC(gtx_color_space_YCoCg)
glmCreateTestGTC(gtx_color_space)
glmCreateTestGTC(gtx_common)
glmCreateTestGTC(gtx_compatibility)
glmCreateTestGTC(gtx_component_wise)
glmCreateTestGTC(gtx_easing)
glmCreateTestGTC(gtx_euler_angle)
glmCreateTestGTC(gtx_extend)
glmCreateTestGTC(gtx_extended_min_max)
glmCreateTestGTC(gtx_exterior_product)
glmCreateTestGTC(gtx_fast_exponential)
glmCreateTestGTC(gtx_fast_square_root)
glmCreateTestGTC(gtx_fast_trigonometry)
glmCreateTestGTC(gtx_functions)
glmCreateTestGTC(gtx_gradient_paint)
glmCreateTestGTC(gtx_handed_coordinate_space)
glmCreateTestGTC(gtx_hash)
glmCreateTestGTC(gtx_integer)
glmCreateTestGTC(gtx_intersect)
glmCreateTestGTC(gtx_io)
glmCreateTestGTC(gtx_load)
glmCreateTestGTC(gtx_log_base)
glmCreateTestGTC(gtx_matrix_cross_product)
glmCreateTestGTC(gtx_matrix_decompose)
glmCreateTestGTC(gtx_matrix_factorisation)
glmCreateTestGTC(gtx_matrix_interpolation)
glmCreateTestGTC(gtx_matrix_major_storage)
glmCreateTestGTC(gtx_matrix_operation)
glmCreateTestGTC(gtx_matrix_query)
glmCreateTestGTC(gtx_matrix_transform_2d)
glmCreateTestGTC(gtx_norm)
glmCreateTestGTC(gtx_normal)
glmCreateTestGTC(gtx_normalize_dot)
glmCreateTestGTC(gtx_orthonormalize)
glmCreateTestGTC(gtx_optimum_pow)
glmCreateTestGTC(gtx_pca)
glmCreateTestGTC(gtx_perpendicular)
glmCreateTestGTC(gtx_polar_coordinates)
glmCreateTestGTC(gtx_projection)
glmCreateTestGTC(gtx_quaternion)
glmCreateTestGTC(gtx_dual_quaternion)
glmCreateTestGTC(gtx_range)
glmCreateTestGTC(gtx_rotate_normalized_axis)
glmCreateTestGTC(gtx_rotate_vector)
glmCreateTestGTC(gtx_scalar_multiplication)
glmCreateTestGTC(gtx_scalar_relational)
glmCreateTestGTC(gtx_spline)
glmCreateTestGTC(gtx_string_cast)
glmCreateTestGTC(gtx_texture)
glmCreateTestGTC(gtx_type_aligned)
glmCreateTestGTC(gtx_type_trait)
glmCreateTestGTC(gtx_vec_swizzle)
glmCreateTestGTC(gtx_vector_angle)
glmCreateTestGTC(gtx_vector_query)
glmCreateTestGTC(gtx_wrap)

```

## vendor\glm\test\perf\CMakeLists.txt
```cmake
glmCreateTestGTC(perf_matrix_div)
glmCreateTestGTC(perf_matrix_inverse)
glmCreateTestGTC(perf_matrix_mul)
glmCreateTestGTC(perf_matrix_mul_vector)
glmCreateTestGTC(perf_matrix_transpose)
glmCreateTestGTC(perf_vector_mul_matrix)

```

## vendor\imgui\CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.20)
project(imgui CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 创建静态库
add_library(${PROJECT_NAME} STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    imgui_demo.cpp
    imgui_tables.cpp
    imconfig.h
    imgui.h
    imgui_internal.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
)


# 设置包含目录
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Windows平台特定设置
if(WIN32)
    target_compile_definitions(${PROJECT_NAME}
        PUBLIC
            IMGUI_API=__declspec\(dllexport\)
    )
endif()

# macOS平台特定设置
if(APPLE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    target_compile_definitions(${PROJECT_NAME}
        PUBLIC
            "IMGUI_API=__attribute__((visibility(\"default\")))"
    )
endif()

# 修改输出目录设置
set_project_output_directories(${PROJECT_NAME})

# 编译选项设置
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:-O0 -g>
    $<$<CONFIG:Release>:-O3>
)

# 设置静态运行时库
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif() 
```

## vendor\imgui\examples\example_android_opengl3\CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.6)

project(ImGuiExample)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${CMAKE_PROJECT_NAME} SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui_tables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui_widgets.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../backends/imgui_impl_android.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../backends/imgui_impl_opengl3.cpp
  ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

set(CMAKE_SHARED_LINKER_FLAGS
  "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate"
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
  IMGUI_IMPL_OPENGL_ES3
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
  ${CMAKE_CURRENT_SOURCE_DIR}/../../backends
  ${ANDROID_NDK}/sources/android/native_app_glue
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
  android
  EGL
  GLESv3
  log
)

```

## vendor\imgui\examples\example_glfw_vulkan\CMakeLists.txt
```cmake
# Example usage:
#  mkdir build
#  cd build
#  cmake -g "Visual Studio 14 2015" ..

cmake_minimum_required(VERSION 2.8)
project(imgui_example_glfw_vulkan C CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

# GLFW
set(GLFW_DIR ../../../glfw) # Set this to point to an up-to-date GLFW repo
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)

# Dear ImGui
set(IMGUI_DIR ../../)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

# Libraries
find_package(Vulkan REQUIRED)
#find_library(VULKAN_LIBRARY
  #NAMES vulkan vulkan-1)
#set(LIBRARIES "glfw;${VULKAN_LIBRARY}")
set(LIBRARIES "glfw;Vulkan::Vulkan")

# Use vulkan headers from glfw:
include_directories(${GLFW_DIR}/deps)

file(GLOB sources *.cpp)

add_executable(example_glfw_vulkan ${sources} ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp)
target_link_libraries(example_glfw_vulkan ${LIBRARIES})
target_compile_definitions(example_glfw_vulkan PUBLIC -DImTextureID=ImU64)

```

## vendor\spdlog\CMakeLists.txt
```cmake
# Copyright(c) 2019 spdlog authors Distributed under the MIT License (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.10...3.21)

# ---------------------------------------------------------------------------------------
# Start spdlog project
# ---------------------------------------------------------------------------------------
include(cmake/utils.cmake)
include(cmake/ide.cmake)

spdlog_extract_version()

project(spdlog VERSION ${SPDLOG_VERSION} LANGUAGES CXX)
message(STATUS "Build spdlog: ${SPDLOG_VERSION}")

include(GNUInstallDirs)

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif ()

# ---------------------------------------------------------------------------------------
# Compiler config
# ---------------------------------------------------------------------------------------
if (SPDLOG_USE_STD_FORMAT)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif ()


set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_SYSTEM_NAME MATCHES "CYGWIN" OR CMAKE_SYSTEM_NAME MATCHES "MSYS" OR CMAKE_SYSTEM_NAME MATCHES "MINGW")
    set(CMAKE_CXX_EXTENSIONS ON)
endif ()

# ---------------------------------------------------------------------------------------
# Set SPDLOG_MASTER_PROJECT to ON if we are building spdlog
# ---------------------------------------------------------------------------------------
# Check if spdlog is being used directly or via add_subdirectory, but allow overriding
if (NOT DEFINED SPDLOG_MASTER_PROJECT)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(SPDLOG_MASTER_PROJECT ON)
    else ()
        set(SPDLOG_MASTER_PROJECT OFF)
    endif ()
endif ()

option(SPDLOG_BUILD_ALL "Build all artifacts" OFF)

# build shared option
option(SPDLOG_BUILD_SHARED "Build shared library" OFF)

# precompiled headers option
option(SPDLOG_ENABLE_PCH "Build static or shared library using precompiled header to speed up compilation time" OFF)

# build position independent code
option(SPDLOG_BUILD_PIC "Build position independent code (-fPIC)" OFF)

# example options
option(SPDLOG_BUILD_EXAMPLE "Build example" ${SPDLOG_MASTER_PROJECT})
option(SPDLOG_BUILD_EXAMPLE_HO "Build header only example" OFF)

# testing options
option(SPDLOG_BUILD_TESTS "Build tests" OFF)
option(SPDLOG_BUILD_TESTS_HO "Build tests using the header only version" OFF)

# bench options
option(SPDLOG_BUILD_BENCH "Build benchmarks (Requires https://github.com/google/benchmark.git to be installed)" OFF)

# sanitizer options
option(SPDLOG_SANITIZE_ADDRESS "Enable address sanitizer in tests" OFF)
option(SPDLOG_SANITIZE_THREAD "Enable thread sanitizer in tests" OFF)
if (SPDLOG_SANITIZE_ADDRESS AND SPDLOG_SANITIZE_THREAD)
    message(FATAL_ERROR "SPDLOG_SANITIZE_ADDRESS and SPDLOG_SANITIZE_THREAD are mutually exclusive")
endif ()

# warning options
option(SPDLOG_BUILD_WARNINGS "Enable compiler warnings" OFF)

# install options
option(SPDLOG_SYSTEM_INCLUDES "Include as system headers (skip for clang-tidy)." OFF)
option(SPDLOG_INSTALL "Generate the install target" ${SPDLOG_MASTER_PROJECT})
option(SPDLOG_USE_STD_FORMAT "Use std::format instead of fmt library." OFF)
option(SPDLOG_FMT_EXTERNAL "Use external fmt library instead of bundled" OFF)
option(SPDLOG_FMT_EXTERNAL_HO "Use external fmt header-only library instead of bundled" OFF)
option(SPDLOG_NO_EXCEPTIONS "Compile with -fno-exceptions. Call abort() on any spdlog exceptions" OFF)

if (SPDLOG_FMT_EXTERNAL AND SPDLOG_FMT_EXTERNAL_HO)
    message(FATAL_ERROR "SPDLOG_FMT_EXTERNAL and SPDLOG_FMT_EXTERNAL_HO are mutually exclusive")
endif ()

if (SPDLOG_USE_STD_FORMAT AND SPDLOG_FMT_EXTERNAL_HO)
    message(FATAL_ERROR "SPDLOG_USE_STD_FORMAT and SPDLOG_FMT_EXTERNAL_HO are mutually exclusive")
endif ()

if (SPDLOG_USE_STD_FORMAT AND SPDLOG_FMT_EXTERNAL)
    message(FATAL_ERROR "SPDLOG_USE_STD_FORMAT and SPDLOG_FMT_EXTERNAL are mutually exclusive")
endif ()

# misc tweakme options
if (WIN32)
    option(SPDLOG_WCHAR_SUPPORT "Support wchar api" OFF)
    option(SPDLOG_WCHAR_FILENAMES "Support wchar filenames" OFF)
    option(SPDLOG_WCHAR_CONSOLE "Support wchar output to console" OFF)
else ()
    set(SPDLOG_WCHAR_SUPPORT OFF CACHE BOOL "non supported option" FORCE)
    set(SPDLOG_WCHAR_FILENAMES OFF CACHE BOOL "non supported option" FORCE)
    set(SPDLOG_WCHAR_CONSOLE OFF CACHE BOOL "non supported option" FORCE)
endif ()

if (MSVC)
    option(SPDLOG_MSVC_UTF8 "Enable/disable msvc /utf-8 flag required by fmt lib" ON)
endif ()

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    option(SPDLOG_CLOCK_COARSE "Use CLOCK_REALTIME_COARSE instead of the regular clock," OFF)
else ()
    set(SPDLOG_CLOCK_COARSE OFF CACHE BOOL "non supported option" FORCE)
endif ()

option(SPDLOG_PREVENT_CHILD_FD "Prevent from child processes to inherit log file descriptors" OFF)
option(SPDLOG_NO_THREAD_ID "prevent spdlog from querying the thread id on each log call if thread id is not needed" OFF)
option(SPDLOG_NO_TLS "prevent spdlog from using thread local storage" OFF)
option(
        SPDLOG_NO_ATOMIC_LEVELS
        "prevent spdlog from using of std::atomic log levels (use only if your code never modifies log levels concurrently"
        OFF)
option(SPDLOG_DISABLE_DEFAULT_LOGGER "Disable default logger creation" OFF)
option(SPDLOG_FWRITE_UNLOCKED "Use the unlocked variant of fwrite. Leave this on unless your libc doesn't have it" ON)

# clang-tidy
option(SPDLOG_TIDY "run clang-tidy" OFF)

if (SPDLOG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    message(STATUS "Enabled clang-tidy")
endif ()

if (SPDLOG_BUILD_PIC)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()

find_package(Threads REQUIRED)
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})
# ---------------------------------------------------------------------------------------
# Static/Shared library
# ---------------------------------------------------------------------------------------
set(SPDLOG_SRCS src/spdlog.cpp src/stdout_sinks.cpp src/color_sinks.cpp src/file_sinks.cpp src/async.cpp src/cfg.cpp)

if (NOT SPDLOG_USE_STD_FORMAT AND NOT SPDLOG_FMT_EXTERNAL AND NOT SPDLOG_FMT_EXTERNAL_HO)
    list(APPEND SPDLOG_SRCS src/bundled_fmtlib_format.cpp)
endif ()

if (SPDLOG_BUILD_SHARED OR BUILD_SHARED_LIBS)
    if (WIN32)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in ${CMAKE_CURRENT_BINARY_DIR}/version.rc @ONLY)
        list(APPEND SPDLOG_SRCS ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
    endif ()
    add_library(spdlog SHARED ${SPDLOG_SRCS} ${SPDLOG_ALL_HEADERS})
    target_compile_definitions(spdlog PUBLIC SPDLOG_SHARED_LIB)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(spdlog PUBLIC $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<NOT:$<COMPILE_LANGUAGE:CUDA>>>:/wd4251
                /wd4275>)
    endif ()
    if (NOT SPDLOG_USE_STD_FORMAT AND NOT SPDLOG_FMT_EXTERNAL AND NOT SPDLOG_FMT_EXTERNAL_HO)
        target_compile_definitions(spdlog PRIVATE FMT_LIB_EXPORT PUBLIC FMT_SHARED)
    endif ()
else ()
    add_library(spdlog STATIC ${SPDLOG_SRCS} ${SPDLOG_ALL_HEADERS})
endif ()

add_library(spdlog::spdlog ALIAS spdlog)

set(SPDLOG_INCLUDES_LEVEL "")
if (SPDLOG_SYSTEM_INCLUDES)
    set(SPDLOG_INCLUDES_LEVEL "SYSTEM")
endif ()

target_compile_definitions(spdlog PUBLIC SPDLOG_COMPILED_LIB)
target_include_directories(spdlog ${SPDLOG_INCLUDES_LEVEL} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries(spdlog PUBLIC Threads::Threads)
spdlog_enable_warnings(spdlog)

set_target_properties(spdlog PROPERTIES VERSION ${SPDLOG_VERSION} SOVERSION
        ${SPDLOG_VERSION_MAJOR}.${SPDLOG_VERSION_MINOR})
set_target_properties(spdlog PROPERTIES DEBUG_POSTFIX d)

if (COMMAND target_precompile_headers AND SPDLOG_ENABLE_PCH)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/pch.h.in ${PROJECT_BINARY_DIR}/spdlog_pch.h @ONLY)
    target_precompile_headers(spdlog PRIVATE ${PROJECT_BINARY_DIR}/spdlog_pch.h)
endif ()

# sanitizer support
if (SPDLOG_SANITIZE_ADDRESS)
    spdlog_enable_addr_sanitizer(spdlog)
elseif (SPDLOG_SANITIZE_THREAD)
    spdlog_enable_thread_sanitizer(spdlog)
endif ()

# ---------------------------------------------------------------------------------------
# Header only version
# ---------------------------------------------------------------------------------------
add_library(spdlog_header_only INTERFACE)
add_library(spdlog::spdlog_header_only ALIAS spdlog_header_only)

target_include_directories(
        spdlog_header_only ${SPDLOG_INCLUDES_LEVEL} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries(spdlog_header_only INTERFACE Threads::Threads)

# ---------------------------------------------------------------------------------------
# Use fmt package if using external fmt
# ---------------------------------------------------------------------------------------
if (SPDLOG_FMT_EXTERNAL OR SPDLOG_FMT_EXTERNAL_HO)
    if (NOT TARGET fmt::fmt)
        find_package(fmt CONFIG REQUIRED)
    endif ()
    target_compile_definitions(spdlog PUBLIC SPDLOG_FMT_EXTERNAL)
    target_compile_definitions(spdlog_header_only INTERFACE SPDLOG_FMT_EXTERNAL)

    # use external fmt-header-only
    if (SPDLOG_FMT_EXTERNAL_HO)
        target_link_libraries(spdlog PUBLIC fmt::fmt-header-only)
        target_link_libraries(spdlog_header_only INTERFACE fmt::fmt-header-only)
    else () # use external compile fmt
        target_link_libraries(spdlog PUBLIC fmt::fmt)
        target_link_libraries(spdlog_header_only INTERFACE fmt::fmt)
    endif ()

    set(PKG_CONFIG_REQUIRES fmt) # add dependency to pkg-config
endif ()

# ---------------------------------------------------------------------------------------
# Check if fwrite_unlocked/_fwrite_nolock is available
# ---------------------------------------------------------------------------------------
if (SPDLOG_FWRITE_UNLOCKED)
    include(CheckSymbolExists)
    if (WIN32)
        check_symbol_exists(_fwrite_nolock "stdio.h" HAVE_FWRITE_UNLOCKED)
    else ()
        check_symbol_exists(fwrite_unlocked "stdio.h" HAVE_FWRITE_UNLOCKED)
    endif ()
    if (HAVE_FWRITE_UNLOCKED)
        target_compile_definitions(spdlog PRIVATE SPDLOG_FWRITE_UNLOCKED)
        target_compile_definitions(spdlog_header_only INTERFACE SPDLOG_FWRITE_UNLOCKED)
    endif ()
endif ()

# ---------------------------------------------------------------------------------------
# Add required libraries for Android CMake build
# ---------------------------------------------------------------------------------------
if (ANDROID)
    target_link_libraries(spdlog PUBLIC log)
    target_link_libraries(spdlog_header_only INTERFACE log)
endif ()

# ---------------------------------------------------------------------------------------
# Misc definitions according to tweak options
# ---------------------------------------------------------------------------------------
set(SPDLOG_WCHAR_TO_UTF8_SUPPORT ${SPDLOG_WCHAR_SUPPORT})
set(SPDLOG_UTF8_TO_WCHAR_CONSOLE ${SPDLOG_WCHAR_CONSOLE})
foreach (
        SPDLOG_OPTION
        SPDLOG_WCHAR_TO_UTF8_SUPPORT
        SPDLOG_UTF8_TO_WCHAR_CONSOLE
        SPDLOG_WCHAR_FILENAMES
        SPDLOG_NO_EXCEPTIONS
        SPDLOG_CLOCK_COARSE
        SPDLOG_PREVENT_CHILD_FD
        SPDLOG_NO_THREAD_ID
        SPDLOG_NO_TLS
        SPDLOG_NO_ATOMIC_LEVELS
        SPDLOG_DISABLE_DEFAULT_LOGGER
        SPDLOG_USE_STD_FORMAT)
    if (${SPDLOG_OPTION})
        target_compile_definitions(spdlog PUBLIC ${SPDLOG_OPTION})
        target_compile_definitions(spdlog_header_only INTERFACE ${SPDLOG_OPTION})
    endif ()
endforeach ()

if (MSVC)
    target_compile_options(spdlog PRIVATE "/Zc:__cplusplus")
    target_compile_options(spdlog_header_only INTERFACE "/Zc:__cplusplus")
    if (SPDLOG_MSVC_UTF8)
        # fmtlib requires the /utf-8 flag when building with msvc.
        # see https://github.com/fmtlib/fmt/pull/4159 on the purpose of the additional
        # "$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:MSVC>>"
        target_compile_options(spdlog PUBLIC $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)
        target_compile_options(spdlog_header_only INTERFACE $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)
    endif ()
endif ()

# ---------------------------------------------------------------------------------------
# If exceptions are disabled, disable them in the bundled fmt as well
# ---------------------------------------------------------------------------------------
if (SPDLOG_NO_EXCEPTIONS)
    if (NOT SPDLOG_FMT_EXTERNAL AND NOT SPDLOG_FMT_EXTERNAL_HO)
        target_compile_definitions(spdlog PUBLIC FMT_EXCEPTIONS=0)
    endif ()
    if (NOT MSVC)
        target_compile_options(spdlog PRIVATE -fno-exceptions)
    else ()
        target_compile_options(spdlog PRIVATE /EHs-c-)
    endif ()
endif ()
# ---------------------------------------------------------------------------------------
# Build binaries
# ---------------------------------------------------------------------------------------
if (SPDLOG_BUILD_EXAMPLE OR SPDLOG_BUILD_EXAMPLE_HO OR SPDLOG_BUILD_ALL)
    message(STATUS "Generating example(s)")
    add_subdirectory(example)
    spdlog_enable_warnings(example)
    if (SPDLOG_BUILD_EXAMPLE_HO)
        spdlog_enable_warnings(example_header_only)
    endif ()
endif ()

if (SPDLOG_BUILD_TESTS OR SPDLOG_BUILD_TESTS_HO OR SPDLOG_BUILD_ALL)
    message(STATUS "Generating tests")
    enable_testing()
    add_subdirectory(tests)
endif ()

if (SPDLOG_BUILD_BENCH OR SPDLOG_BUILD_ALL)
    message(STATUS "Generating benchmarks")
    add_subdirectory(bench)
endif ()

# ---------------------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------------------
if (SPDLOG_INSTALL)
    message(STATUS "Generating install")
    set(project_config_in "${CMAKE_CURRENT_LIST_DIR}/cmake/spdlogConfig.cmake.in")
    set(project_config_out "${CMAKE_CURRENT_BINARY_DIR}/spdlogConfig.cmake")
    set(config_targets_file "spdlogConfigTargets.cmake")
    set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/spdlogConfigVersion.cmake")
    set(export_dest_dir "${CMAKE_INSTALL_LIBDIR}/cmake/spdlog")
    set(pkgconfig_install_dir "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
    set(pkg_config "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc")

    # ---------------------------------------------------------------------------------------
    # Include files
    # ---------------------------------------------------------------------------------------
    install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" PATTERN "fmt/bundled" EXCLUDE)
    install(
            TARGETS spdlog spdlog_header_only
            EXPORT spdlog
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    if (NOT SPDLOG_USE_STD_FORMAT AND NOT SPDLOG_FMT_EXTERNAL AND NOT SPDLOG_FMT_EXTERNAL_HO)
        install(DIRECTORY include/${PROJECT_NAME}/fmt/bundled/
                DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/fmt/bundled/")
    endif ()

    # ---------------------------------------------------------------------------------------
    # Install pkg-config file
    # ---------------------------------------------------------------------------------------
    if (IS_ABSOLUTE "${CMAKE_INSTALL_INCLUDEDIR}")
        set(PKG_CONFIG_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}")
    else ()
        set(PKG_CONFIG_INCLUDEDIR "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
    endif ()
    if (IS_ABSOLUTE "${CMAKE_INSTALL_LIBDIR}")
        set(PKG_CONFIG_LIBDIR "${CMAKE_INSTALL_LIBDIR}")
    else ()
        set(PKG_CONFIG_LIBDIR "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
    endif ()
    get_target_property(PKG_CONFIG_DEFINES spdlog INTERFACE_COMPILE_DEFINITIONS)
    string(REPLACE ";" " -D" PKG_CONFIG_DEFINES "${PKG_CONFIG_DEFINES}")
    string(CONCAT PKG_CONFIG_DEFINES "-D" "${PKG_CONFIG_DEFINES}")
    configure_file("cmake/${PROJECT_NAME}.pc.in" "${pkg_config}" @ONLY)
    install(FILES "${pkg_config}" DESTINATION "${pkgconfig_install_dir}")

    # ---------------------------------------------------------------------------------------
    # Install CMake config files
    # ---------------------------------------------------------------------------------------
    export(TARGETS spdlog spdlog_header_only NAMESPACE spdlog::
            FILE "${CMAKE_CURRENT_BINARY_DIR}/${config_targets_file}")
    install(EXPORT spdlog DESTINATION ${export_dest_dir} NAMESPACE spdlog:: FILE ${config_targets_file})

    include(CMakePackageConfigHelpers)
    configure_package_config_file("${project_config_in}" "${project_config_out}" INSTALL_DESTINATION ${export_dest_dir})

    write_basic_package_version_file("${version_config_file}" COMPATIBILITY SameMajorVersion)
    install(FILES "${project_config_out}" "${version_config_file}" DESTINATION "${export_dest_dir}")

    # ---------------------------------------------------------------------------------------
    # Support creation of installable packages
    # ---------------------------------------------------------------------------------------
    include(cmake/spdlogCPack.cmake)
endif ()

```

## vendor\spdlog\bench\CMakeLists.txt
```cmake
# Copyright(c) 2019 spdlog authors Distributed under the MIT License (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.11)
project(spdlog_bench CXX)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog CONFIG REQUIRED)
endif()

find_package(Threads REQUIRED)
find_package(benchmark CONFIG)
if(NOT benchmark_FOUND)
    message(STATUS "Using CMake Version ${CMAKE_VERSION}")
    # User can fetch googlebenchmark
    message(STATUS "Downloading GoogleBenchmark")
    include(FetchContent)

    # disable tests
    set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "")
    # Do not build and run googlebenchmark tests
    FetchContent_Declare(googlebenchmark GIT_REPOSITORY https://github.com/google/benchmark.git GIT_TAG v1.6.0)
    FetchContent_MakeAvailable(googlebenchmark)
endif()

add_executable(bench bench.cpp)
spdlog_enable_warnings(bench)
target_link_libraries(bench PRIVATE spdlog::spdlog)

add_executable(async_bench async_bench.cpp)
target_link_libraries(async_bench PRIVATE spdlog::spdlog)

add_executable(latency latency.cpp)
target_link_libraries(latency PRIVATE benchmark::benchmark spdlog::spdlog)

add_executable(formatter-bench formatter-bench.cpp)
target_link_libraries(formatter-bench PRIVATE benchmark::benchmark spdlog::spdlog)

```

## vendor\spdlog\cmake\ide.cmake
```cmake
# ---------------------------------------------------------------------------------------
# IDE support for headers
# ---------------------------------------------------------------------------------------
set(SPDLOG_HEADERS_DIR "${CMAKE_CURRENT_LIST_DIR}/../include")

file(GLOB SPDLOG_TOP_HEADERS "${SPDLOG_HEADERS_DIR}/spdlog/*.h")
file(GLOB SPDLOG_DETAILS_HEADERS "${SPDLOG_HEADERS_DIR}/spdlog/details/*.h")
file(GLOB SPDLOG_SINKS_HEADERS "${SPDLOG_HEADERS_DIR}/spdlog/sinks/*.h")
file(GLOB SPDLOG_FMT_HEADERS "${SPDLOG_HEADERS_DIR}/spdlog/fmt/*.h")
file(GLOB SPDLOG_FMT_BUNDELED_HEADERS "${SPDLOG_HEADERS_DIR}/spdlog/fmt/bundled/*.h")
set(SPDLOG_ALL_HEADERS ${SPDLOG_TOP_HEADERS} ${SPDLOG_DETAILS_HEADERS} ${SPDLOG_SINKS_HEADERS} ${SPDLOG_FMT_HEADERS}
                       ${SPDLOG_FMT_BUNDELED_HEADERS})

source_group("Header Files\\spdlog" FILES ${SPDLOG_TOP_HEADERS})
source_group("Header Files\\spdlog\\details" FILES ${SPDLOG_DETAILS_HEADERS})
source_group("Header Files\\spdlog\\sinks" FILES ${SPDLOG_SINKS_HEADERS})
source_group("Header Files\\spdlog\\fmt" FILES ${SPDLOG_FMT_HEADERS})
source_group("Header Files\\spdlog\\fmt\\bundled\\" FILES ${SPDLOG_FMT_BUNDELED_HEADERS})

```

## vendor\spdlog\cmake\spdlogCPack.cmake
```cmake
set(CPACK_GENERATOR "TGZ;ZIP" CACHE STRING "Semicolon separated list of generators")

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR}" "${PROJECT_NAME}" ALL .)

set(CPACK_PROJECT_URL "https://github.com/gabime/spdlog")
set(CPACK_PACKAGE_VENDOR "Gabi Melman")
set(CPACK_PACKAGE_CONTACT "Gabi Melman <gmelman1@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Fast C++ logging library")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
if(PROJECT_VERSION_TWEAK)
    set(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION}.${PROJECT_VERSION_TWEAK})
endif()
set(CPACK_PACKAGE_RELOCATABLE ON CACHE BOOL "Build relocatable package")

set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_DEBIAN_PACKAGE_SECTION "libs")
set(CPACK_RPM_PACKAGE_URL ${CPACK_PROJECT_URL})
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${CPACK_PROJECT_URL})
set(CPACK_RPM_PACKAGE_DESCRIPTION "Very fast, header-only/compiled, C++ logging library.")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Very fast, header-only/compiled, C++ logging library.")

if(CPACK_PACKAGE_NAME)
    set(CPACK_RPM_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
    set(CPACK_DEBIAN_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
else()
    set(CPACK_RPM_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
    set(CPACK_DEBIAN_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
    set(CPACK_RPM_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_DEBIAN_PACKAGE_NAME "${PROJECT_NAME}")
endif()

if(CPACK_RPM_PACKAGE_RELEASE)
    set(CPACK_RPM_FILE_NAME "${CPACK_RPM_FILE_NAME}-${CPACK_RPM_PACKAGE_RELEASE}")
endif()
if(CPACK_DEBIAN_PACKAGE_RELEASE)
    set(CPACK_DEBIAN_FILE_NAME "${CPACK_DEBIAN_FILE_NAME}-${CPACK_DEBIAN_PACKAGE_RELEASE}")
endif()

if(CPACK_RPM_PACKAGE_ARCHITECTURE)
    set(CPACK_RPM_FILE_NAME "${CPACK_RPM_FILE_NAME}.${CPACK_RPM_PACKAGE_ARCHITECTURE}")
endif()
if(CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
    set(CPACK_DEBIAN_FILE_NAME "${CPACK_DEBIAN_FILE_NAME}.${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
endif()
set(CPACK_RPM_FILE_NAME "${CPACK_RPM_FILE_NAME}.rpm")
set(CPACK_DEBIAN_FILE_NAME "${CPACK_DEBIAN_FILE_NAME}.deb")

if(NOT CPACK_PACKAGE_RELOCATABLE)
    # Depend on pkgconfig rpm to create the system pkgconfig folder
    set(CPACK_RPM_PACKAGE_REQUIRES pkgconfig)
    set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION
        "${CPACK_PACKAGING_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/pkgconfig")
endif()

include(CPack)

```

## vendor\spdlog\cmake\utils.cmake
```cmake
# Get spdlog version from include/spdlog/version.h and put it in SPDLOG_VERSION
function(spdlog_extract_version)
    file(READ "${CMAKE_CURRENT_LIST_DIR}/include/spdlog/version.h" file_contents)
    string(REGEX MATCH "SPDLOG_VER_MAJOR ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract major version number from spdlog/version.h")
    endif()
    set(ver_major ${CMAKE_MATCH_1})

    string(REGEX MATCH "SPDLOG_VER_MINOR ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract minor version number from spdlog/version.h")
    endif()

    set(ver_minor ${CMAKE_MATCH_1})
    string(REGEX MATCH "SPDLOG_VER_PATCH ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract patch version number from spdlog/version.h")
    endif()
    set(ver_patch ${CMAKE_MATCH_1})

    set(SPDLOG_VERSION_MAJOR ${ver_major} PARENT_SCOPE)
    set(SPDLOG_VERSION_MINOR ${ver_minor} PARENT_SCOPE)
    set(SPDLOG_VERSION_PATCH ${ver_patch} PARENT_SCOPE)
    set(SPDLOG_VERSION "${ver_major}.${ver_minor}.${ver_patch}" PARENT_SCOPE)
endfunction()

# Turn on warnings on the given target
function(spdlog_enable_warnings target_name)
    if(SPDLOG_BUILD_WARNINGS)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            list(APPEND MSVC_OPTIONS "/W3")
            if(MSVC_VERSION GREATER 1900) # Allow non fatal security warnings for msvc 2015
                list(APPEND MSVC_OPTIONS "/WX")
            endif()
        endif()

        target_compile_options(
            ${target_name}
            PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                    -Wall
                    -Wextra
                    -Wconversion
                    -pedantic
                    -Werror
                    -Wfatal-errors>
                    $<$<CXX_COMPILER_ID:MSVC>:${MSVC_OPTIONS}>)
    endif()
endfunction()

# Enable address sanitizer (gcc/clang only)
function(spdlog_enable_addr_sanitizer target_name)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(FATAL_ERROR "Sanitizer supported only for gcc/clang")
    endif()
    message(STATUS "Address sanitizer enabled")
    target_compile_options(${target_name} PRIVATE -fsanitize=address,undefined)
    target_compile_options(${target_name} PRIVATE -fno-sanitize=signed-integer-overflow)
    target_compile_options(${target_name} PRIVATE -fno-sanitize-recover=all)
    target_compile_options(${target_name} PRIVATE -fno-omit-frame-pointer)
    target_link_libraries(${target_name} PRIVATE -fsanitize=address,undefined)
endfunction()

# Enable thread sanitizer (gcc/clang only)
function(spdlog_enable_thread_sanitizer target_name)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(FATAL_ERROR "Sanitizer supported only for gcc/clang")
    endif()
    message(STATUS "Thread sanitizer enabled")
    target_compile_options(${target_name} PRIVATE -fsanitize=thread)
    target_compile_options(${target_name} PRIVATE -fno-omit-frame-pointer)
    target_link_libraries(${target_name} PRIVATE -fsanitize=thread)
endfunction()

```

## vendor\spdlog\example\CMakeLists.txt
```cmake
# Copyright(c) 2019 spdlog authors Distributed under the MIT License (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.11)
project(spdlog_examples CXX)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

# ---------------------------------------------------------------------------------------
# Example of using pre-compiled library
# ---------------------------------------------------------------------------------------
add_executable(example example.cpp)
target_link_libraries(example PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

# ---------------------------------------------------------------------------------------
# Example of using header-only library
# ---------------------------------------------------------------------------------------
if(SPDLOG_BUILD_EXAMPLE_HO)
    add_executable(example_header_only example.cpp)
    target_link_libraries(example_header_only PRIVATE spdlog::spdlog_header_only)
endif()

```

## vendor\spdlog\tests\CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.11)
project(spdlog_utests CXX)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

include(../cmake/utils.cmake)

find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(systemd libsystemd)
endif()

find_package(Catch2 3 QUIET)
if(Catch2_FOUND)
    message(STATUS "Packaged version of Catch will be used.")
else()
    message(STATUS "Bundled version of Catch will be downloaded and used.")
    include(FetchContent)
    FetchContent_Declare(Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG 53d0d913a422d356b23dd927547febdf69ee9081 # v3.5.0
    )
    FetchContent_MakeAvailable(Catch2)
endif()

set(SPDLOG_UTESTS_SOURCES
    test_file_helper.cpp
    test_file_logging.cpp
    test_daily_logger.cpp
    test_misc.cpp
    test_eventlog.cpp
    test_pattern_formatter.cpp
    test_async.cpp
    test_registry.cpp
    test_macros.cpp
    utils.cpp
    main.cpp
    test_mpmc_q.cpp
    test_dup_filter.cpp
    test_fmt_helper.cpp
    test_stdout_api.cpp
    test_backtrace.cpp
    test_create_dir.cpp
    test_custom_callbacks.cpp
    test_cfg.cpp
    test_time_point.cpp
    test_stopwatch.cpp
    test_circular_q.cpp
    test_bin_to_hex.cpp)

if(NOT SPDLOG_NO_EXCEPTIONS)
    list(APPEND SPDLOG_UTESTS_SOURCES test_errors.cpp)
endif()

if(systemd_FOUND)
    list(APPEND SPDLOG_UTESTS_SOURCES test_systemd.cpp)
endif()

enable_testing()

function(spdlog_prepare_test test_target spdlog_lib)
    add_executable(${test_target} ${SPDLOG_UTESTS_SOURCES})
    spdlog_enable_warnings(${test_target})
    target_link_libraries(${test_target} PRIVATE ${spdlog_lib})
    if(systemd_FOUND)
        target_link_libraries(${test_target} PRIVATE ${systemd_LIBRARIES})
    endif()
    target_link_libraries(${test_target} PRIVATE Catch2::Catch2WithMain)
    if(SPDLOG_SANITIZE_ADDRESS)
        spdlog_enable_addr_sanitizer(${test_target})
    elseif (SPDLOG_SANITIZE_THREAD)
        spdlog_enable_thread_sanitizer(${test_target})
    endif ()
    add_test(NAME ${test_target} COMMAND ${test_target})
    set_tests_properties(${test_target} PROPERTIES RUN_SERIAL ON)
endfunction()

# The compiled library tests
if(SPDLOG_BUILD_TESTS OR SPDLOG_BUILD_ALL)
    spdlog_prepare_test(spdlog-utests spdlog::spdlog)
endif()

# The header-only library version tests
if(SPDLOG_BUILD_TESTS_HO OR SPDLOG_BUILD_ALL)
    spdlog_prepare_test(spdlog-utests-ho spdlog::spdlog_header_only)
endif()

```

