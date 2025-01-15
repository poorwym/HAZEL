# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置输出目录格式（与 premake 保持一致）
set(OUTPUT_FORMAT "${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

# 定义输出目录函数
function(set_project_output_directories target)
    set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/../bin/${OUTPUT_FORMAT}/${target})
    set(INT_DIR ${CMAKE_BINARY_DIR}/../bin-int/${OUTPUT_FORMAT}/${target})
    
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR}
        
        LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR}
        
        ARCHIVE_OUTPUT_DIRECTORY ${INT_DIR}
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${INT_DIR}
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${INT_DIR}
    )
    
    file(MAKE_DIRECTORY ${OUTPUT_DIR} ${INT_DIR})
endfunction()

# 设置架构相关的编译选项
function(set_architecture_options)
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
endfunction()

# 设置平台相关的宏定义
function(set_platform_definitions target)
    if(WIN32)
        target_compile_definitions(${target} PRIVATE HAZEL_PLATFORM_WINDOWS)
    elseif(APPLE)
        target_compile_definitions(${target} PRIVATE HAZEL_PLATFORM_MACOS)
    elseif(UNIX AND NOT APPLE)
        target_compile_definitions(${target} PRIVATE HAZEL_PLATFORM_LINUX)
    endif()

    if(CMAKE_GENERATOR MATCHES "Visual Studio")
        target_compile_definitions(${target} PRIVATE HAZEL_GENERATOR_VS)
    elseif(CMAKE_GENERATOR MATCHES "Ninja")
        target_compile_definitions(${target} PRIVATE HAZEL_GENERATOR_NINJA)
    elseif(CMAKE_GENERATOR MATCHES "Unix Makefiles")
        target_compile_definitions(${target} PRIVATE HAZEL_GENERATOR_MAKE)
    elseif(CMAKE_GENERATOR MATCHES "Xcode")
        target_compile_definitions(${target} PRIVATE HAZEL_GENERATOR_XCODE)
    endif()
endfunction()

# 打印项目信息
function(print_project_info target)
    get_target_property(target_output_dir ${target} RUNTIME_OUTPUT_DIRECTORY)
    get_target_property(target_int_dir ${target} ARCHIVE_OUTPUT_DIRECTORY)
    
    message(STATUS "Project: ${target}")
    message(STATUS "Output directory: ${target_output_dir}")
    message(STATUS "Intermediate directory: ${target_int_dir}")
    message(STATUS "Generator: ${CMAKE_GENERATOR}")
    message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
    message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
    message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
    
    message(STATUS "Compile definitions:")
    get_target_property(COMPILE_DEFS ${target} COMPILE_DEFINITIONS)
    if(COMPILE_DEFS)
        foreach(DEF ${COMPILE_DEFS})
            message(STATUS "  ${DEF}")
        endforeach()
    endif()
endfunction() 