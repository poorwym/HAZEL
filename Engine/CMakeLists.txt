cmake_minimum_required(VERSION 3.20)
project(Engine)

# 设置源文件目录
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(Engine SHARED
    ${SOURCE_DIR}/HAZEL/Core.h
    ${SOURCE_DIR}/HAZEL/Application.h
    ${SOURCE_DIR}/HAZEL/Application.cpp
)

message(STATUS "build Engine")

# 根据不同处理器架构设置编译选项
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    message(STATUS "Building for ARM64 architecture")
    add_compile_options(-march=armv8-a)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm|ARM")
    message(STATUS "Building for ARM architecture") 
    add_compile_options(-march=armv7-a)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    message(STATUS "Building for x86_64 architecture")
    if(MSVC)
        add_compile_options(/arch:AVX2)
    else()
        add_compile_options(-march=x86-64 -mavx2)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i686|i386")
    message(STATUS "Building for x86 architecture")
    if(MSVC)
        add_compile_options(/arch:SSE2)
    else()
        add_compile_options(-march=i686 -msse2)
    endif()
endif()

set(CMAKE_BUILD_TYPE Debug)

# 根据系统类型添加平台相关的宏定义
if(WIN32)
    add_definitions(-DENGINE_EXPORTS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_compile_definitions(Engine PRIVATE
        HAZEL_PLATFORM_WINDOWS
        HAZEL_BUILD_DLL
    )
elseif(APPLE)
    target_compile_definitions(Engine PRIVATE
        HAZEL_PLATFORM_MACOS
        HAZEL_BUILD_DLL
    )
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(Engine PRIVATE
        HAZEL_PLATFORM_LINUX
        HAZEL_BUILD_DLL
    )
endif()

# 根据生成器类型添加相关的宏定义
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    target_compile_definitions(Engine PRIVATE HAZEL_GENERATOR_VS)
elseif(CMAKE_GENERATOR MATCHES "Ninja")
    target_compile_definitions(Engine PRIVATE HAZEL_GENERATOR_NINJA)
elseif(CMAKE_GENERATOR MATCHES "Unix Makefiles")
    target_compile_definitions(Engine PRIVATE HAZEL_GENERATOR_MAKE)
elseif(CMAKE_GENERATOR MATCHES "Xcode")
    target_compile_definitions(Engine PRIVATE HAZEL_GENERATOR_XCODE)
endif()

set(CMAKE_CXX_STANDARD 17)


# 设置输出目录
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_PROCESSOR}/${PROJECT_NAME})
set(INT_DIR ${CMAKE_BINARY_DIR}/../bin/int/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_PROCESSOR}/${PROJECT_NAME})
set(LIB_DIR ${CMAKE_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_PROCESSOR}/${PROJECT_NAME})


set_target_properties(Engine PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR}

    LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR}
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIB_DIR}
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIB_DIR}

    ARCHIVE_OUTPUT_DIRECTORY ${INT_DIR}
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${INT_DIR}
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${INT_DIR}
)

file(MAKE_DIRECTORY ${OUTPUT_DIR} ${INT_DIR} ${LIB_DIR})

message("output_dir: ${OUTPUT_DIR}")
message("int_dir: ${INT_DIR}")
message("lib_dir: ${LIB_DIR}")
message(STATUS "Generator: ${CMAKE_GENERATOR}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

message(STATUS "Compile definitions:")
get_target_property(COMPILE_DEFS Engine COMPILE_DEFINITIONS)
foreach(DEF ${COMPILE_DEFS})
    message(STATUS "  ${DEF}")
endforeach()

message(STATUS "Public compile definitions:")
get_target_property(PUBLIC_DEFS Engine INTERFACE_COMPILE_DEFINITIONS) 
foreach(DEF ${PUBLIC_DEFS})
    message(STATUS "  ${DEF}")
endforeach()


message(STATUS "build Engine done")


