cmake_minimum_required(VERSION 3.20)
project(Sandbox)

# 设置源文件目录
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
# 设置 Engine 源文件目录
set(ENGINE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/src)
# 设置 Engine 库目录
set(ENGINE_LIB_DIR ${CMAKE_BINARY_DIR}/../bin/${OUTPUT_FORMAT}/Engine)
set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/vendor)

# 添加 Sandbox 源文件
add_executable(${PROJECT_NAME}
    ${SOURCE_DIR}/SandboxApp.cpp
)

# 添加库搜索路径
link_directories(${ENGINE_LIB_DIR})

# 链接 Engine 库

#if(APPLE)
#    target_link_libraries(${PROJECT_NAME} PRIVATE "${ENGINE_LIB_DIR}/libEngine.dylib")
#elseif(WIN32)
#    target_link_libraries(${PROJECT_NAME} PRIVATE "${ENGINE_LIB_DIR}/Engine.dll")
#else()
#    target_link_libraries(${PROJECT_NAME} PRIVATE "${ENGINE_LIB_DIR}/libEngine.so")
#endif()

# 添加头文件路径
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SOURCE_DIR}
    ${VENDOR_DIR}/spdlog/include
    ${ENGINE_SRC_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Engine)

# 设置架构相关的编译选项
set_architecture_options()

# 设置平台相关的宏定义
set_platform_definitions(${PROJECT_NAME})

# 设置输出目录
set_project_output_directories(${PROJECT_NAME})

# 复制 Engine 库文件到 Sandbox 输出目录
if(APPLE)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ENGINE_LIB_DIR}/libEngine.dylib"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
elseif(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ENGINE_LIB_DIR}/libEngine.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
else()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ENGINE_LIB_DIR}/libEngine.so"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

# 打印项目信息
print_project_info(${PROJECT_NAME})


