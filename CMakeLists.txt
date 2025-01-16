cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Hazel)
include(cmake/Common.cmake)

# 添加子项目
add_subdirectory(Engine/vendor/Glad)
add_subdirectory(Engine/vendor/GLFW)
add_subdirectory(Engine)
add_subdirectory(Sandbox)

# 设置构建顺序依赖
add_dependencies(Engine glad)
add_dependencies(Engine glfw)
add_dependencies(Sandbox Engine)