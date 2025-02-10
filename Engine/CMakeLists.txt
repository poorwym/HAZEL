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



