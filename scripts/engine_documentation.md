# Engine 项目文件结构
生成时间: 2025-02-11 00:19:40

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


# 源代码文件内容

## CMakeLists.txt
```
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

## src\Hazel.h
```
﻿#pragma once

#include "Hazel/Application.h"
#include "Hazel/Log.h"
#include "Hazel/Layer.h"
#include "Hazel/Input.h"
#include "Hazel/KeyCodes.h"
#include "Hazel/MouseButtonCodes.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Core/Timestep.h"

// -----------Renderer----------
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/RendererAPI.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/OrthographicCameraController.h"
// -----------------------------

// -----------Entry point-----------
#include "Hazel/EntryPoint.h"
// --------------------------------

```

## src\hzpch.cpp
```
﻿#include "hzpch.h"
```

## src\hzpch.h
```
﻿#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <string>
#include <array>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>


#ifdef HAZEL_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

#ifdef HAZEL_PLATFORM_LINUX
    #include <X11/Xlib.h>
#endif
```

## src\Hazel\Application.cpp
```
﻿#include "hzpch.h"
#include "Application.h"
#include "Log.h"
#include "Hazel/Input.h"
#include "glm/glm.hpp"  
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Hazel {

// 这里绑定要this的原因是成员函数需要具体实例才能运行
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application()         
    {
        HAZEL_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create()); // 这里是创建一个Window
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent)); // 这里是绑定OnEvent函数，当窗口收到事件时，就会调用OnEvent函数。

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application() {
    }

    void Application::Run() {
        while (m_Running) {             
            float time = static_cast<float>(glfwGetTime()); //Platform::GetTime();
            Timestep timestep = time - m_LastFrameTime; // 这里可以相当于用右侧构造了一个Timestep对象
            m_LastFrameTime = time;

            if(!m_Minimized){ // 最小化就停止渲染
                
                // 这里从begin开始渲染，到end结束渲染。
				for (Layer* layer : m_LayerStack) {
					layer->OnUpdate(timestep);
				}
           
            
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }
    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
        // HAZEL_CORE_TRACE("{0}", e);
        // 这里从end开始处理事件，到这个处理结束为止。
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->OnEvent(e); // 这里是Layer的OnEvent接口，不是Application的OnEvent接口。
            if (e.IsHandled()) {
                break;
            }
        }

        auto [x, y] = Input::GetMousePosition();
        // HAZEL_CORE_TRACE("{0}, {1}", x, y);
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    bool Hazel::Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0) {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;

        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
        return false;
    }
}

```

## src\Hazel\Application.h
```
﻿#pragma once
/**
 * @note: 这里是application，HAZEL引擎可以创建一个这样的Application然后运行。
 */
#include "Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Window.h"
#include "LayerStack.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"

namespace Hazel {
    /**
     * @brief 应用程序的主类，管理窗口、事件和层
     */
    class HAZEL_API Application {
    public:
        /**
         * @brief 构造函数，初始化应用程序实例
         */
        Application();

        /**
         * @brief 虚析构函数，确保正确清理资源
         */
        virtual ~Application();

        /**
         * @brief 运行应用程序的主循环
         */
        void Run();

        /**
         * @brief 处理应用程序事件
         * @param e 要处理的事件
         */
        void OnEvent(Event& e);
        
        /**
         * @brief 向层栈添加新的层
         * @param layer 要添加的层指针
         */
        void PushLayer(Layer* layer);

        /**
         * @brief 向层栈添加新的覆盖层
         * @param overlay 要添加的覆盖层指针
         */
        void PushOverlay(Layer* overlay);

        /**
         * @brief 获取应用程序的单例实例
         * @return 应用程序实例的引用
         */
        inline static Application& Get() { return *s_Instance; }

        /**
         * @brief 获取应用程序窗口
         * @return 窗口实例的引用
         */
        inline Window& GetWindow() { return *m_Window; }

    private:
        /**
         * @brief 处理窗口关闭事件的回调函数
         * @param e 窗口关闭事件
         * @return 事件是否已处理
         */
        bool OnWindowClose(WindowCloseEvent& e);

        bool OnWindowResize(WindowResizeEvent& e);

    private:
        std::unique_ptr<Window> m_Window;      // 应用程序窗口的智能指针
        ImGuiLayer* m_ImGuiLayer; // 
        bool m_Running = true;                 // 应用程序运行状态标志
        LayerStack m_LayerStack;              // 层栈，用于管理应用程序的各个层
        static Application* s_Instance;        // 应用程序的单例实例指针     
        float m_LastFrameTime = 0.0f;
        bool m_Minimized = false;
    };

    /**
     * @brief 创建应用程序实例的函数声明
     * @return 新创建的应用程序实例指针
     * @note 此函数需要由客户端实现
     */
    Application* CreateApplication();
}

```

## src\Hazel\Core.h
```
﻿#pragma once
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
```

## src\Hazel\EntryPoint.h
```
﻿#pragma once

/**
 * @note: 这里是程序进入的入口，目前基本上就是Init然后进行Run，然后进行Destroy。
 */

#if defined(HAZEL_PLATFORM_WINDOWS) || defined(HAZEL_PLATFORM_LINUX) || defined(HAZEL_PLATFORM_MACOS)

extern Hazel::Application* Hazel::CreateApplication();
int main(int argc, char** argv){
    Hazel::Log::Init();
    HAZEL_CORE_WARN("Initialized Log!");
    HAZEL_INFO("Initialized Log!");
    auto app = Hazel::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

#endif


```

## src\Hazel\Input.h
```
﻿#pragma once
/**
 * @note: 这里和Window一样，是一个纯虚的类，可以在不同平台通过不同方法实现。
 */

#include "Hazel/Core.h"

namespace Hazel
{

	class HAZEL_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode);  };

		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressed(button); };
		
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); };
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); };
        inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); };
	protected:
        virtual bool IsKeyPressedImpl(int keycode) = 0;

        virtual std::pair<float, float> GetMousePositionImpl() = 0;
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;
	private:
		static Input* s_Instance;
	};
}

```

## src\Hazel\KeyCodes.h
```
﻿#pragma once

// from glfw3
#define HAZEL_KEY_SPACE              32
#define HAZEL_KEY_APOSTROPHE         39  /* ' */
#define HAZEL_KEY_COMMA              44  /* , */
#define HAZEL_KEY_MINUS              45  /* - */
#define HAZEL_KEY_PERIOD             46  /* . */
#define HAZEL_KEY_SLASH              47  /* / */
#define HAZEL_KEY_0                  48
#define HAZEL_KEY_1                  49
#define HAZEL_KEY_2                  50
#define HAZEL_KEY_3                  51
#define HAZEL_KEY_4                  52
#define HAZEL_KEY_5                  53
#define HAZEL_KEY_6                  54
#define HAZEL_KEY_7                  55
#define HAZEL_KEY_8                  56
#define HAZEL_KEY_9                  57
#define HAZEL_KEY_SEMICOLON          59  /* ; */
#define HAZEL_KEY_EQUAL              61  /* = */
#define HAZEL_KEY_A                  65
#define HAZEL_KEY_B                  66
#define HAZEL_KEY_C                  67
#define HAZEL_KEY_D                  68
#define HAZEL_KEY_E                  69
#define HAZEL_KEY_F                  70
#define HAZEL_KEY_G                  71
#define HAZEL_KEY_H                  72
#define HAZEL_KEY_I                  73
#define HAZEL_KEY_J                  74
#define HAZEL_KEY_K                  75
#define HAZEL_KEY_L                  76
#define HAZEL_KEY_M                  77
#define HAZEL_KEY_N                  78
#define HAZEL_KEY_O                  79
#define HAZEL_KEY_P                  80
#define HAZEL_KEY_Q                  81
#define HAZEL_KEY_R                  82
#define HAZEL_KEY_S                  83
#define HAZEL_KEY_T                  84
#define HAZEL_KEY_U                  85
#define HAZEL_KEY_V                  86
#define HAZEL_KEY_W                  87
#define HAZEL_KEY_X                  88
#define HAZEL_KEY_Y                  89
#define HAZEL_KEY_Z                  90
#define HAZEL_KEY_LEFT_BRACKET       91  /* [ */
#define HAZEL_KEY_BACKSLASH          92  /* \ */
#define HAZEL_KEY_RIGHT_BRACKET      93  /* ] */
#define HAZEL_KEY_GRAVE_ACCENT       96  /* ` */
#define HAZEL_KEY_WORLD_1            161 /* non-US #1 */
#define HAZEL_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define HAZEL_KEY_ESCAPE             256
#define HAZEL_KEY_ENTER              257
#define HAZEL_KEY_TAB                258
#define HAZEL_KEY_BACKSPACE          259
#define HAZEL_KEY_INSERT             260
#define HAZEL_KEY_DELETE             261
#define HAZEL_KEY_RIGHT              262
#define HAZEL_KEY_LEFT               263
#define HAZEL_KEY_DOWN               264
#define HAZEL_KEY_UP                 265
#define HAZEL_KEY_PAGE_UP            266
#define HAZEL_KEY_PAGE_DOWN          267
#define HAZEL_KEY_HOME               268
#define HAZEL_KEY_END                269
#define HAZEL_KEY_CAPS_LOCK          280
#define HAZEL_KEY_SCROLL_LOCK        281
#define HAZEL_KEY_NUM_LOCK           282
#define HAZEL_KEY_PRINT_SCREEN       283
#define HAZEL_KEY_PAUSE              284
#define HAZEL_KEY_F1                 290
#define HAZEL_KEY_F2                 291
#define HAZEL_KEY_F3                 292
#define HAZEL_KEY_F4                 293
#define HAZEL_KEY_F5                 294
#define HAZEL_KEY_F6                 295
#define HAZEL_KEY_F7                 296
#define HAZEL_KEY_F8                 297
#define HAZEL_KEY_F9                 298
#define HAZEL_KEY_F10                299
#define HAZEL_KEY_F11                300
#define HAZEL_KEY_F12                301
#define HAZEL_KEY_F13                302
#define HAZEL_KEY_F14                303
#define HAZEL_KEY_F15                304
#define HAZEL_KEY_F16                305
#define HAZEL_KEY_F17                306
#define HAZEL_KEY_F18                307
#define HAZEL_KEY_F19                308
#define HAZEL_KEY_F20                309
#define HAZEL_KEY_F21                310
#define HAZEL_KEY_F22                311
#define HAZEL_KEY_F23                312
#define HAZEL_KEY_F24                313
#define HAZEL_KEY_F25                314
#define HAZEL_KEY_KP_0               320
#define HAZEL_KEY_KP_1               321
#define HAZEL_KEY_KP_2               322
#define HAZEL_KEY_KP_3               323
#define HAZEL_KEY_KP_4               324
#define HAZEL_KEY_KP_5               325
#define HAZEL_KEY_KP_6               326
#define HAZEL_KEY_KP_7               327
#define HAZEL_KEY_KP_8               328
#define HAZEL_KEY_KP_9               329
#define HAZEL_KEY_KP_DECIMAL         330
#define HAZEL_KEY_KP_DIVIDE          331
#define HAZEL_KEY_KP_MULTIPLY        332
#define HAZEL_KEY_KP_SUBTRACT        333
#define HAZEL_KEY_KP_ADD             334
#define HAZEL_KEY_KP_ENTER           335
#define HAZEL_KEY_KP_EQUAL           336
#define HAZEL_KEY_LEFT_SHIFT         340
#define HAZEL_KEY_LEFT_CONTROL       341
#define HAZEL_KEY_LEFT_ALT           342
#define HAZEL_KEY_LEFT_SUPER         343
#define HAZEL_KEY_RIGHT_SHIFT        344
#define HAZEL_KEY_RIGHT_CONTROL      345
#define HAZEL_KEY_RIGHT_ALT          346
#define HAZEL_KEY_RIGHT_SUPER        347
#define HAZEL_KEY_MENU               348
```

## src\Hazel\Layer.cpp
```
﻿#include "hzpch.h"
#include "Layer.h"

namespace Hazel {

	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{
	}

	Layer::~Layer()
	{
	}

}
```

## src\Hazel\Layer.h
```
﻿#pragma once
/**
 * @note: Layer是一个基类，没啥实现的东西，就是定义接口。
 */
#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Core/Timestep.h"

namespace Hazel {

	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(Timestep ts) {};
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Event& event) {};

		inline const std::string& GetName() const { return m_DebugName; };

	protected:
		std::string m_DebugName;
	};

}
```

## src\Hazel\LayerStack.cpp
```
﻿#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
        for (auto layer : m_Layers)
        {
            // layer->OnDetach();
            delete layer;
        }
	}

	void LayerStack::PushLayer(Layer* layer)
	{
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer); // 在m_LayerInsert位置插入layer
		m_LayerInsertIndex++;
        layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
        m_Layers.emplace_back(overlay); // 在m_Layers的最后插入overlay
        overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer); // 在m_Layers中找到layer
        if (it != m_Layers.end())
        {
            m_Layers.erase(it); // 删除layer
            m_LayerInsertIndex--; // 将m_LayerInsert向前移动
        }
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay); // 在m_Layers中找到overlay
        if (it != m_Layers.end())
        {
            m_Layers.erase(it); // 删除overlay
        }
	}
}
```

## src\Hazel\LayerStack.h
```
﻿#pragma once
/**
 * @note: 这个是Layer的管理器，提供一些layer的管理方法，
 *	overlay 和 layer 的区别是：overlay 永远在 layer 上面一层。
 * 渲染顺序是 先 layer 再 overlay
 * 而对事件的处理顺序是：先判断overlay，再判断layer
 */
#include "Hazel/Core.h"
#include "Layer.h"
#include <vector>

namespace Hazel {

	class HAZEL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer); 
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		// std::vector<Layer*>::iterator m_LayerInsert; //这里不再使用iterator是因为如果利用iterator记录layer和overlay的分界线，vector移动后iterator指向的地址会失效

		unsigned int m_LayerInsertIndex = 0;
	};
};

```

## src\Hazel\Log.cpp
```
﻿#include "hzpch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
namespace Hazel {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        
        s_CoreLogger = spdlog::stdout_color_mt("HAZEL");
        s_CoreLogger->set_level(spdlog::level::trace);

        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
    }
} 
```

## src\Hazel\Log.h
```
﻿#pragma once
/**
 * @note:   这里是一个log，分为Core和Client，
 *          Core是引擎的log，Client是客户端的log。
 */
#include "hzpch.h"
#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>


namespace Hazel{
    class HAZEL_API Log{
    private:    
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
    public:
        static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    };
}

// core log macros
#define HAZEL_CORE_TRACE(...) ::Hazel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HAZEL_CORE_INFO(...) ::Hazel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HAZEL_CORE_WARN(...) ::Hazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HAZEL_CORE_ERROR(...) ::Hazel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HAZEL_CORE_CRITICAL(...) ::Hazel::Log::GetCoreLogger()->critical(__VA_ARGS__)

// client log macros
#define HAZEL_TRACE(...) ::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HAZEL_INFO(...) ::Hazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define HAZEL_WARN(...) ::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HAZEL_ERROR(...) ::Hazel::Log::GetClientLogger()->error(__VA_ARGS__)
#define HAZEL_CRITICAL(...) ::Hazel::Log::GetClientLogger()->critical(__VA_ARGS__)
```

## src\Hazel\MouseButtonCodes.h
```
﻿#pragma once

// from glfw3

#define HAZEL_MOUSE_BUTTON_1         0
#define HAZEL_MOUSE_BUTTON_2         1
#define HAZEL_MOUSE_BUTTON_3         2
#define HAZEL_MOUSE_BUTTON_4         3
#define HAZEL_MOUSE_BUTTON_5         4
#define HAZEL_MOUSE_BUTTON_6         5
#define HAZEL_MOUSE_BUTTON_7         6
#define HAZEL_MOUSE_BUTTON_8         7
#define HAZEL_MOUSE_BUTTON_LAST      HAZEL_MOUSE_BUTTON_8
#define HAZEL_MOUSE_BUTTON_LEFT      HAZEL_MOUSE_BUTTON_1
#define HAZEL_MOUSE_BUTTON_RIGHT     HAZEL_MOUSE_BUTTON_2
#define HAZEL_MOUSE_BUTTON_MIDDLE    HAZEL_MOUSE_BUTTON_3
```

## src\Hazel\OrthographicCameraController.cpp
```
﻿#include "hzpch.h"
#include "OrthographicCameraController.h"
#include "Input.h"
#include "KeyCodes.h"

namespace Hazel {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_AllowRotation(rotation)
	{

	}

	void OrthographicCameraController::SetProjection(float left, float right, float bottom, float top)
	{
		m_Camera.SetProjection(left, right, bottom, top);
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(HAZEL_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(HAZEL_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(HAZEL_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(HAZEL_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

        if (m_AllowRotation)
        {
			if (Input::IsKeyPressed(HAZEL_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(HAZEL_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			m_Camera.SetRotation(m_CameraRotation);
        }

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel * 5;
	}
	 
	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(HAZEL_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(HAZEL_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));

	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.5;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}

```

## src\Hazel\OrthographicCameraController.h
```
﻿#pragma once
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void SetProjection(float left, float right, float bottom, float top);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }

		void SetRotation(bool rotation) { m_AllowRotation = rotation; }
		bool GetRotation() const { return m_AllowRotation; }
	private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		bool m_AllowRotation = false;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = glm::vec3(0.0f);
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f; 
	};
};
```

## src\Hazel\Window.h
```
﻿#pragma once
/**
 * @note:   这里是一个纯虚基类，用于定义窗口接口。
 *          窗口类是一个抽象类，它定义了窗口的属性和行为。
 *          窗口类提供了窗口的创建、更新、事件处理等基本功能。
 *          子类必须实现这些功能，以实现具体的窗口功能。
 *          这里之所以使用抽象类是因为不希望任何依赖于平台或者外界vendor的代码来实现，增加了代码的灵活性和可维护性。
 */
#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {
    // 窗口属性

    struct WindowProps {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "Hazel Engine",
                    unsigned int width = 1280,
                    unsigned int height = 720)
            : Title(title), Width(width), Height(height) {}
    };
 
    // 这里都是抽象接口，不需要实现具体的功能
    class HAZEL_API Window {
    public:
        // 事件回调函数类型定义
        // 用于处理窗口事件(如键盘输入、鼠标移动等)
        using EventCallbackFn = std::function<void(Event&)>; // 回调函数,接收一个Event引用作为参数,返回void


        // 窗口析构函数
        virtual ~Window() {}

        // 窗口更新函数,处理窗口事件和渲染
        virtual void OnUpdate() = 0;

        // 获取窗口宽度
        // @return 返回窗口宽度(像素)
        virtual unsigned int GetWidth() const = 0;

        // 获取窗口高度 
        // @return 返回窗口高度(像素)
        virtual unsigned int GetHeight() const = 0;

        // 设置窗口事件回调函数
        // @param callback 事件处理回调函数
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

        // 设置垂直同步
        // @param enabled 是否启用垂直同步
        virtual void SetVSync(bool enabled) = 0;

        // 获取垂直同步状态
        // @return 返回是否启用了垂直同步
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;


        // 创建窗口的静态工厂方法
        // @param props 窗口属性,包含标题、宽度和高度
        // @return 返回创建的窗口实例指针
        static Window* Create(const WindowProps& props = WindowProps());
    };
}
```

## src\Hazel\Core\Timestep.h
```
﻿#pragma once
/**
* @note: 这里Timestep用秒为单位，本质就是个float的封装
*/

namespace Hazel {
	class Timestep {
	public:
		Timestep(float time = 0.0f)
                : m_Time(time)
        {
        }

        operator float() const { return m_Time; }

        float GetSeconds() const { return m_Time;}
        float GetMilliseconds() const { return m_Time * 1000.0f; }

	private:
        float m_Time;
	};
}
```

## src\Hazel\Events\ApplicationEvent.h
```
#pragma once
#include "hzpch.h"
#include "Event.h"

namespace Hazel {
    class HAZEL_API WindowResizeEvent : public Event{
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
        :m_Width(width), m_Height(height){}

        inline unsigned int GetWidth() const { return m_Width; }
        inline unsigned int GetHeight() const { return m_Height; }

        std::string ToString() const override {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::WindowResize);
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication);
    private:
        unsigned int m_Width, m_Height;
    };

    class HAZEL_API WindowCloseEvent : public Event{
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(EventType::WindowClose);
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication);
    };

    class HAZEL_API AppTickEvent : public Event{
    public:
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(EventType::AppTick);
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication);
    };

    class HAZEL_API AppUpdateEvent : public Event{
    public:
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(EventType::AppUpdate);
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication);
    };

    class HAZEL_API AppRenderEvent : public Event{
    public:
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(EventType::AppRender);
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication);
    };
}
```

## src\Hazel\Events\Event.h
```
﻿#pragma once
#include "hzpch.h"
#include "Hazel/Core.h"
#include "Hazel/Log.h"
#include "spdlog/fmt/fmt.h"


namespace Hazel {
    // 事件类型枚举
    // None - 空事件
    // WindowClose - 窗口关闭事件
    // WindowResize - 窗口大小改变事件
    // WindowFocus - 窗口获得焦点事件
    // WindowLostFocus - 窗口失去焦点事件
    // WindowMoved - 窗口移动事件
    // AppTick - 应用程序tick事件
    // AppUpdate - 应用程序更新事件
    // AppRender - 应用程序渲染事件
    // KeyPressed - 按键按下事件
    // KeyReleased - 按键释放事件
    // MouseButtonPressed - 鼠标按键按下事件
    // MouseButtonReleased - 鼠标按键释放事件
    // MouseMoved - 鼠标移动事件
    // MouseScrolledEvent - 鼠标滚轮事件
    enum class EventType{
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    // 事件类别枚举
    // None - 空类别
    // EventCategoryApplication - 应用程序类别
    // EventCategoryInput - 输入类别
    // EventCategoryKeyboard - 键盘类别
    // EventCategoryMouse - 鼠标类别
    // EventCategoryMouseButton - 鼠标按键类别
    // BIT(n) - 位运算，表示第n位为1，其他位为0 BIT(x) = 1 << x
    enum EventCategory{
        None = 0,
        EventCategoryApplication    = BIT(0),
        EventCategoryInput          = BIT(1),
        EventCategoryKeyboard       = BIT(2),
        EventCategoryMouse          = BIT(3),
        EventCategoryMouseButton    = BIT(4)
    };

// 事件类型宏定义
// EVENT_CLASS_TYPE(type) - 定义事件类型相关的函数
// GetStaticType() - 获取事件的静态类型
// GetEventType() - 获取事件的类型
// GetName() - 获取事件的名称
// type - 事件类型
// 这里就是定义Event类中的通用接口
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return type; } \
                                virtual EventType GetEventType() const override { return type; } \
                                virtual const char* GetName() const override { return #type; } 
// 这是 C++ 预处理器中的字符串化操作，将宏参数转为字符串。
// 获取事件的静态类型，这样就可以直接通过Event::GetStaticType()来获取事件的类型，不依赖于具体的对象，比较省性能
// EVENT_CLASS_CATEGORY(category) - 定义事件类别相关的函数
// GetCategoryFlags() - 获取事件的类别
// category - 事件类别
// 这里就是定义EventCategory类中的通用接口
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }
    class HAZEL_API Event {
        friend class EventDispatcher;
    public:
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }
        bool IsInCategory(EventCategory category) {
            return GetCategoryFlags() & category;
        }
        bool IsHandled() const { return m_Handled; }
    protected:
        bool m_Handled = false;
    };
    // 事件分发器类
    // 用于处理和分发不同类型的事件
    // 通过模板函数Dispatch实现事件的动态分发
    // 可以根据事件类型调用相应的处理函数
    class HAZEL_API EventDispatcher {
        template<typename T>
        using EventFn = std::function<bool(T&)>; // 意味着所有用于Dispatch的函数都是接受一个T的引用，然后返回一个bool
    public:
        EventDispatcher(Event& event)
        :m_Event(event){}

        template<typename T>
        bool Dispatch(EventFn<T> func){ 
            if(m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.m_Handled = func(*(T*)&m_Event);
                return true;
            }
            else{
                return false;
            }
        }
    private:
        Event& m_Event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e){
        return (os << e.ToString());
    }
}

// 一定要写这个，我也不太清楚为什么，大概就是注册一下让spdlog知道这个可以用格式化
// template <>
// struct fmt::formatter<Hazel::Event> : fmt::ostream_formatter {};
```

## src\Hazel\Events\KeyEvent.h
```
﻿
#pragma once

#include "hzpch.h"
#include "Event.h"

namespace Hazel {

    class HAZEL_API KeyEvent : public Event {
    
    public:
        inline int GetKeyCode() const { return m_KeyCode; }
        // 获取事件类别标志
        // 返回值是键盘事件和输入事件的组合
        // 使用位运算符 | 来组合多个事件类别
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput);
    
    protected:
        int m_KeyCode;
        // 构造函数，这个类无法被除了子类以外的类实例化 
        KeyEvent(const int keycode)
            : m_KeyCode(keycode) {}
    };

    class HAZEL_API KeyPressedEvent : public KeyEvent {
    public:
        // 获取按键重复次数
        // 返回值是按键重复的次数
        // 当按键被持续按下时,会产生重复事件
        inline int GetRepeatCount() const { return m_RepeatCount; }
        KeyPressedEvent(const int keycode, const int repeatCount)
            : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::KeyPressed);
    private:
        int m_RepeatCount;
    };

    class HAZEL_API KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(const int keycode)
            : KeyEvent(keycode) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::KeyReleased);
    };

    class HAZEL_API KeyTypedEvent : public KeyEvent {
    public:
        KeyTypedEvent(const int keycode)
            : KeyEvent(keycode) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::KeyTyped);
    };
}
```

## src\Hazel\Events\MouseEvent.h
```
﻿#pragma once 

#include "hzpch.h"
#include "Event.h"

namespace Hazel {
    //先写public，再写private，因为public是给用户看的，接口比内部实现更重要
    class HAZEL_API MouseMovedEvent : public Event{
    public:
        MouseMovedEvent(float x, float y)
        :m_MouseX(x), m_MouseY(y){}

        inline float GetX() const { return m_MouseX; }
        inline float GetY() const { return m_MouseY; }

        std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseMoved);
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput);
    private:
        float m_MouseX, m_MouseY;
    };

    class HAZEL_API MouseScrolledEvent : public Event{
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
        :m_XOffset(xOffset), m_YOffset(yOffset){}

        inline float GetXOffset() const { return m_XOffset; }
        inline float GetYOffset() const { return m_YOffset; }

        std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseScrolled);
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput);
    private:
        float m_XOffset, m_YOffset;
    };

    class HAZEL_API MouseButtonEvent : public Event{
    public:
        inline int GetMouseButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput);
    protected:
        MouseButtonEvent(const int button)
        :m_Button(button){}

        int m_Button;
    };

    class HAZEL_API MouseButtonPressedEvent : public MouseButtonEvent{
    public:
        MouseButtonPressedEvent(const int button)
        :MouseButtonEvent(button){}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseButtonPressed);
    };

    class HAZEL_API MouseButtonReleasedEvent : public MouseButtonEvent{
    public:
        MouseButtonReleasedEvent(const int button)
        :MouseButtonEvent(button){}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseButtonReleased);
    };
}

 

```

## src\Hazel\ImGui\ImGuiBuild.cpp
```
﻿#include "hzpch.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD //我感觉新版好像这句没必要
#include "backends/imgui_impl_opengl3.cpp"
#include "backends/imgui_impl_glfw.cpp" 
```

## src\Hazel\ImGui\ImGuiLayer.cpp
```
﻿#include "hzpch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "Hazel/Application.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
namespace Hazel {


	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

}

```

## src\Hazel\ImGui\ImGuiLayer.h
```
﻿#pragma once
/**
* @note:    这里非常有趣一个点是dll会删去静态库中没有被使用的函数，所以如果sandbox要使用没有被使用的函数，会出现链接错误。
*           解决方案是module definition文件，里面定义了需要使用的函数  。 
*           当然也可以用静态库。 
*/
#include "Hazel/Layer.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {
    class HAZEL_API ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();

    private:
        float m_Time = 0.0f;

    };

}
```

## src\Hazel\Renderer\Buffer.cpp
```
﻿#include "hzpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Hazel/Log.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
        case RendererAPI::API::None:
			HAZEL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
        case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}
		HAZEL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
	}

	
    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None :
            HAZEL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
        case RendererAPI::API::OpenGL:
            return new OpenGLIndexBuffer(indices, count);
        }
        HAZEL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
```

## src\Hazel\Renderer\Buffer.h
```
﻿#pragma once
/**
* @note:这里是纯虚函数接口，不同平台有不同实现
*/
#include "Hazel/Core.h"

namespace Hazel {
    // 这里所有的BufferLayout都是跨平台的
    enum class ShaderDataType {
        None = 0,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        Mat3, Mat4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type) {
        switch (type) {
		    case ShaderDataType::Float:  return 4;
		    case ShaderDataType::Float2: return 4 * 2;
		    case ShaderDataType::Float3: return 4 * 3;
		    case ShaderDataType::Float4: return 4 * 4;
		    case ShaderDataType::Int:    return 4;
		    case ShaderDataType::Int2:   return 4 * 2;
		    case ShaderDataType::Int3:   return 4 * 3;
		    case ShaderDataType::Int4:   return 4 * 4;
		    case ShaderDataType::Mat3:   return 4 * 3 * 3;
		    case ShaderDataType::Mat4:   return 4 * 4 * 4;
            case ShaderDataType::Bool:   return 1;
        }
        HAZEL_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    struct BufferElement {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        uint32_t Offset;
        bool Normalized;
        
        BufferElement() = default;

        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {
        }

        inline uint32_t GetComponentCount() const {
            switch (Type) {
                case ShaderDataType::Float:     return 1;
                case ShaderDataType::Float2:    return 2;
                case ShaderDataType::Float3:    return 3;
                case ShaderDataType::Float4:    return 4;
                case ShaderDataType::Int:       return 1;
                case ShaderDataType::Int2:      return 2;
                case ShaderDataType::Int3:      return 3;
                case ShaderDataType::Int4:      return 4;
                case ShaderDataType::Mat3:      return 3 * 3;
                case ShaderDataType::Mat4:      return 4 * 4;
                case ShaderDataType::Bool:      return 1;
            }
            HAZEL_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
        }
    };
    
    class HAZEL_API BufferLayout {
    public:
        BufferLayout() = default;

        BufferLayout(const std::initializer_list<BufferElement>& elements) // 这里使用initializer_list,不可修改,vector可以由initializer_list直接构造 
            : m_Elements(elements), m_Stride(0){
            CalculateOffsetsAndStride();
        }
        inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
        inline uint32_t GetStride() const { return m_Stride; } 

        //这两个函数允许类似for(auto& element : layout)这样的遍历
        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

        //这两个函数允许类似for(const auto& element : layout)这样的遍历
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

    private:
        void CalculateOffsetsAndStride() {
            uint32_t offset = 0;
            uint32_t stride = 0;
            for (auto& element : m_Elements) {
                element.Offset = offset;
                offset += element.Size;
                stride += element.Size;
            }
            m_Stride = stride;
        }
    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride;
    };

    // 以上内容都是跨平台的

    class HAZEL_API VertexBuffer {
    public:
        virtual ~VertexBuffer() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        //virtual void SetData(const void* data, uint32_t size) = 0;

        virtual const BufferLayout& GetLayout() const = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;

        static VertexBuffer* Create(float* vertices, uint32_t size);
    };

    class HAZEL_API IndexBuffer {
    public:
        virtual ~IndexBuffer() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;

        static IndexBuffer* Create(uint32_t* indices, uint32_t count);
    };
}
```

## src\Hazel\Renderer\GraphicsContext.h
```
﻿#pragma once
#include "Hazel/Core.h"
namespace Hazel {
	class HAZEL_API GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}
```

## src\Hazel\Renderer\OrthographicCamera.cpp
```
﻿#include "hzpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel{

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * // 基础的位置 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1)); // 旋转

		m_ViewMatrix = glm::inverse(transform); // ViewMatrix是TransformMatrix的逆矩阵
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; // glm主列,所以先p再v，directx主行，所以先v再p

	}

}

```

## src\Hazel\Renderer\OrthographicCamera.h
```
﻿#pragma once
#include "Hazel/Core.h"
#include <glm/glm.hpp>

namespace Hazel {

	class HAZEL_API	OrthographicCamera {
	public:
        OrthographicCamera(float left, float right, float bottom, float top);
        void SetProjection(float left, float right, float bottom, float top);

        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); };
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); };

        const glm::vec3& GetPosition() const { return m_Position; };
        float GetRotation() const { return m_Rotation; };

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; };
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; };
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; };
    private:
        void RecalculateViewMatrix();
	private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix; // for cache
        
		glm::vec3 m_Position = glm::vec3(0.0f);
        float m_Rotation = 0.0f; // 正交投影相机一般没有3维旋转（除非博那种），一个浮点数表示z轴的旋转角度就行了
	};

}
```

## src\Hazel\Renderer\RenderCommand.cpp
```
﻿#include "hzpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}
```

## src\Hazel\Renderer\RenderCommand.h
```
﻿#pragma once
#include "RendererAPI.h"

namespace Hazel {
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear(); 
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
};
```

## src\Hazel\Renderer\Renderer.cpp
```
﻿#include "hzpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
        RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
        RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
        vertexArray->Bind();

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		RenderCommand::DrawIndexed(vertexArray);
	}

}
```

## src\Hazel\Renderer\Renderer.h
```
﻿#pragma once
#include "Hazel/Core.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "OrthographicCamera.h"

namespace Hazel {

	class HAZEL_API Renderer {
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);

        static void EndScene();
		 
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}

```

## src\Hazel\Renderer\RendererAPI.cpp
```
﻿#include "hzpch.h"
#include "RendererAPI.h"

namespace Hazel {
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL; //这样调用RendererAPI就会直接用OpenGLRendererAPI
}
```

## src\Hazel\Renderer\RendererAPI.h
```
﻿#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel {

	class RendererAPI
	{
	public:
		enum class API { // 这里嵌套两层的意义是区分自己定义的RendererAPI和使用的平台API，同时也更符合逻辑，使用的API自然在RendererAPI下 
			None = 0,
			OpenGL = 1
		};
	public:
		virtual void Init() = 0;
		virtual void Clear() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	private:
        static API s_API;
	};
}
```

## src\Hazel\Renderer\Shader.cpp
```
﻿#include "hzpch.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer.h"

namespace Hazel
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HAZEL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		HAZEL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string& filePath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HAZEL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(filePath);
		}

		HAZEL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		HAZEL_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		HAZEL_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	Ref<Hazel::Shader> Hazel::ShaderLibrary::Load(const std::string& filePath)
	{
		auto shader = Shader::Create(filePath); 
		Add(shader);
		return shader;
	}

	Ref<Hazel::Shader> Hazel::ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Ref<Hazel::Shader> Hazel::ShaderLibrary::Get(const std::string& name)
	{
		HAZEL_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}



```

## src\Hazel\Renderer\Shader.h
```
﻿#pragma once
#include "Hazel/Core.h"
#include <string> //pch一般只放cpp文件
#include <unordered_map>

namespace Hazel {

	class HAZEL_API Shader
	{
    public:	
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string& filePath);
	};

	class ShaderLibrary {
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader); // 这个允许自定义命名
		Ref<Shader> Load(const std::string& filePath);
        Ref<Shader> Load(const std::string& name, const std::string& filePath);

		Ref<Shader> Get(const std::string& name);
		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}
```

## src\Hazel\Renderer\Texture.cpp
```
﻿#include "hzpch.h"
#include "Hazel/Renderer/Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Hazel/Renderer/Renderer.h"

namespace Hazel {
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HAZEL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path); //这里make_shared就不需要使用ptr.reset()了，只需要ptr = Texture2D::Create(path)
		}

		HAZEL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
```

## src\Hazel\Renderer\Texture.h
```
﻿#pragma once
#include "Hazel/Core.h"
#include <string>

namespace Hazel
{
    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        //virtual void SetData(void* data, uint32_t size) = 0;

        virtual void Bind(uint32_t slot = 0) const = 0;

        //virtual bool operator==(const Texture& other) const = 0;
    };


    class Texture2D : public Texture
    {
    public:
        // static Ref<Texture2D> Create(uint32_t width, uint32_t height);
        static Ref<Texture2D> Create(const std::string& path);
    };
}
```

## src\Hazel\Renderer\VertexArray.cpp
```
﻿#include "hzpch.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer.h"

namespace Hazel
{
    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            HAZEL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLVertexArray();
        }

        HAZEL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
```

## src\Hazel\Renderer\VertexArray.h
```
﻿#pragma once

#include "Hazel/Core.h"
#include "Hazel/Renderer/Buffer.h"
#include <memory>

namespace Hazel { 
	class HAZEL_API VertexArray {
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const = 0;
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}
```

## src\Platform\OpenGL\OpenGLBuffer.cpp
```
﻿#include "hzpch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace Hazel {

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// ---------------------------------------------------------------------------------------------------------------
	// -------------------------------------------IndexBuffer---------------------------------------------------------

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW); //glBufferData其实接受的是size而不是count,但是我们已经知道了uint32_t,所以传输count就能知道size
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}

```

## src\Platform\OpenGL\OpenGLBuffer.h
```
﻿#pragma once
#include "Hazel/Core.h"
#include "Hazel/Renderer/Buffer.h"

namespace Hazel {

	class HAZEL_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

    class HAZEL_API OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual uint32_t GetCount() const override { return m_Count; }

    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };
}
```

## src\Platform\OpenGL\OpenGLContext.cpp
```
﻿#include "hzpch.h"
#include "OpenGLContext.h"
#include "Hazel/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HAZEL_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		// 设置OpenGL上下文
		glfwMakeContextCurrent(m_WindowHandle);

		// 初始化GLAD
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HAZEL_CORE_ASSERT(status, "Failed to initialize Glad!");

        HAZEL_CORE_INFO("OpenGL Info:");
		HAZEL_CORE_INFO("Vendor: {0}", (const char*)glGetString(GL_VENDOR));
        HAZEL_CORE_INFO("Renderer: {0}", (const char*)glGetString(GL_RENDERER));
        HAZEL_CORE_INFO("OpenGL Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		// 交换缓冲区
		glfwSwapBuffers(m_WindowHandle);
	}

}
```

## src\Platform\OpenGL\OpenGLContext.h
```
﻿#pragma once
#include "Hazel/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Hazel
{
	class HAZEL_API OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}
```

## src\Platform\OpenGL\OpenGLRendererAPI.cpp
```
﻿#include "hzpch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

namespace Hazel {
	void OpenGLRendererAPI::Init()
	{
        glEnable(GL_BLEND); // 启用混合

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}
```

## src\Platform\OpenGL\OpenGLRendererAPI.h
```
﻿#pragma once

#include "Hazel/Renderer/RendererAPI.h"

namespace Hazel {
	class OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}
```

## src\Platform\OpenGL\OpenGLShader.cpp
```
﻿#include "hzpch.h"
#include "OpenGLShader.h"
#include "Hazel/Log.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
namespace Hazel {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") {
            return GL_VERTEX_SHADER;
		}
        if (type == "fragment" || type == "pixel") {
            return GL_FRAGMENT_SHADER;
		}

        HAZEL_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		std::string source = ReadFile(filePath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// assets/shaders/Texture.glsl
		auto lastSlash = filePath.find_last_of("/\\"); // 因为windows有/或者\\，非常奇怪
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1; //有可能路径上没有slash
		// 还需要考虑文件名有没有dot（这个真的细）
		auto lastDot = filePath.rfind('.'); 
		size_t count = lastDot == std::string::npos ? filePath.length() - lastSlash : lastDot - lastSlash;
		/*
		* @note:如果你要查找某个字符串（子串）最后一次出现的位置 → 用 rfind
		*		如果你要查找多个字符（字符集合）中的任意一个最后一次出现的位置 → 用 find_last_of
		*/
        m_Name = filePath.substr(lastSlash, count);
	} 

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		std::string result;
		// 打开着色器文件
		// filePath: 文件路径
		// std::ios::in: 以读取模式打开文件
		// std::ios::binary: 以二进制模式打开文件，防止文本转换导致的问题
		std::ifstream in(filePath, std::ios::in | std::ios::binary); // 这里只能接受2个参数

		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			HAZEL_CORE_ERROR("Could not open shader file! {0}", filePath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos) // std::string::npos 表示找不到
        {
            size_t eol = source.find_first_of("\r\n", pos); // 找到换行符的位置, eol 是换行符的位置
			HAZEL_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1; // "#type"后面只能有一个空格, begin 是那个空格之后的位置
			std::string type = source.substr(begin, eol - begin); //begin 到 eol 之间的内容
            HAZEL_CORE_ASSERT(ShaderTypeFromString(type) != 0, "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); // 找到下一个换行符的位置
            pos = source.find(typeToken, nextLinePos); // 找到下一个 #type
            shaderSources[ShaderTypeFromString(type)] = //
				(pos == std::string::npos) // 如果找不到 #type, 说明是 shader 最后一个片段
				? source.substr(nextLinePos) // 从 nextLinePos 到 最后
				: source.substr(nextLinePos, pos - nextLinePos); // 从 nextLinePos 到 pos
        }

		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
	{
		GLuint program = glCreateProgram(); //这里和另外一个构造函数不一样，因为如果编译失败m_RendererID仍然不为0
		// std::vector<GLuint> glShaderIDs; // vector在堆上分配内存，貌似慢一点
		HAZEL_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now!");
		std::array<GLuint, 2> glShaderIDs; // 创建一个数组，用来存储编译后的着色器
		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources) {
            GLenum type = kv.first;
            const std::string& source = kv.second; 

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				HAZEL_CORE_ERROR("{0}", infoLog.data());
				HAZEL_CORE_ASSERT(false, "Shader compliation failed!");
				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs) {
                glDeleteShader(id);
			}

			// HAZEL_CORE_ERROR("fragment shader compilation failure!");
			HAZEL_CORE_ERROR("{0}", infoLog.data());
			HAZEL_CORE_ASSERT(false, "program links error!");
			return;
		}

		for (auto id : glShaderIDs) {
            glDetachShader(program, id);
		}

		m_RendererID = program; // 如果m_RendererID不为0，说明编译成功
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
		// HAZEL_CORE_ASSERT(m_UniformLocationCache.find(name) != m_UniformLocationCache.end(), "Uniform not found!");
	}

	

}

```

## src\Platform\OpenGL\OpenGLShader.h
```
﻿#pragma once
#include "Hazel/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Hazel {

	typedef unsigned int GLenum; //这里很诡异，因为我们不能在这个文件里包含glad，因为glad这个库的路径只能在Engine项目中找到，而sandbox项目使用了这个头文件

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filePath);
		virtual	~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; };

		void UploadUniformInt(const std::string& name, const int values);

		void UploadUniformFloat(const std::string& name, const float values);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source); // 这里是为了可能文件中有多个shader，除了vs和fs外还有几何shader
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);
	private:
		uint32_t m_RendererID; //使用uint32_t不用GLuint是因为跨平台需要，不必要引入过多的头文件
		std::string m_Name;
	};

}

```

## src\Platform\OpenGL\OpenGLTexture.cpp
```
﻿#include "hzpch.h"

#include "OpenGLTexture.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace Hazel {


	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1); // 翻转图片
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		HAZEL_CORE_ASSERT(data, "Failed to load image!");

        m_Width = width;
        m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		// internalFormat 指定了OpenGL如何在GPU内存中存储纹理数据
		// 例如 GL_RGBA8 表示使用8位存储每个颜色分量(红、绿、蓝、透明度)
		// 这是纹理在GPU中的内部格式

		// dataFormat 指定了输入图像数据的格式
		// 例如 GL_RGBA 表示输入数据是按照红、绿、蓝、透明度的顺序排列的
		// 这是从CPU传输到GPU的数据格式
		
		if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
		}
		else if(channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
		}

		HAZEL_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID); // 创建纹理

        glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height); // 分配空间

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 设置纹理参数,GL_TEXTURE_MIN_FILTER意思是缩小时使用线性插值
		 // glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 设置纹理参数,GL_TEXTURE_MAG_FILTER意思是放大时使用线性插值
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 设置纹理参数,GL_NEAREST 使用最接近的像素
		
		// 更新纹理数据,参数依次为:纹理ID,mipmap级别,x偏移,y偏移,宽度,高度,像素数据格式,像素数据类型,像素数据
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
		// 之后纹理数据都存在于GPU了

        stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

}
```

## src\Platform\OpenGL\OpenGLTexture.h
```
﻿#pragma once
#include "Hazel/Renderer/Texture.h"

namespace Hazel
{
	class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const std::string& path);
        virtual ~OpenGLTexture2D();

        virtual uint32_t GetWidth() const override { return m_Width; };
        virtual uint32_t GetHeight() const override { return m_Height; };

		//virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const override;
    private:
        uint32_t m_RendererID;
        std::string m_Path;
        uint32_t m_Width, m_Height;
        // glm::ivec2 m_Size;
    };
}
```

## src\Platform\OpenGL\OpenGLVertexArray.cpp
```
﻿#include "hzpch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace Hazel {
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float:     return GL_FLOAT;
		case ShaderDataType::Float2:    return GL_FLOAT;
		case ShaderDataType::Float3:    return GL_FLOAT;
		case ShaderDataType::Float4:    return GL_FLOAT;
		case ShaderDataType::Mat3:      return GL_FLOAT;
		case ShaderDataType::Mat4:      return GL_FLOAT;
		case ShaderDataType::Int:       return GL_INT;
		case ShaderDataType::Int2:      return GL_INT;
		case ShaderDataType::Int3:      return GL_INT;
		case ShaderDataType::Int4:      return GL_INT;
		case ShaderDataType::Bool:      return GL_BOOL;
		}

		HAZEL_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();
		// 这样两步操作后，这个vbo就会和这个vao绑定在一起，之后只需要绑定vao就可以同时绑定vbo

		HAZEL_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!"); // 如果vbo没有layout，则报错

		uint32_t index = 0;
		const BufferLayout& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
        indexBuffer->Bind();
		// 这样两步操作后，这个ibo就会和这个vao绑定在一起，之后只需要绑定vao就可以同时绑定ibo

        m_IndexBuffer = indexBuffer;
	}

}
```

## src\Platform\OpenGL\OpenGLVertexArray.h
```
﻿#pragma once
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;  

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const override { return m_VertexBuffers; };
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; };


	private:
		// 允许一个vao拥有多个vbo，比如一个vbo存储顶点，一个vbo存储颜色，一个vbo存储纹理坐标，然后用不同的顶点属性来区分数据
		// 这样其实和一个很大的vbo用很多的顶点属性来区分数据差不多
		std::vector<Ref<VertexBuffer>> m_VertexBuffers; 
        Ref<IndexBuffer> m_IndexBuffer;
		uint32_t m_RendererID;
	};
}
```

## src\Platform\Windows\WindowsInput.cpp
```
﻿#include "hzpch.h"
#include "WindowsInput.h"
#include "Hazel/Application.h"
#include <GLFW/glfw3.h>


namespace Hazel {

	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);  // 0 = false, 1 = true, -1 = error
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, button); 
        return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return { (float)xpos, (float)ypos };

	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
        return (float)x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
        return (float)y;
	}

}
```

## src\Platform\Windows\WindowsInput.h
```
﻿#pragma once
#include "Hazel/Input.h"


namespace Hazel {
	class HAZEL_API WindowsInput : public Input {
	protected:
		//这里一个细节，之所以使用virtual关键字是为了提醒这是由虚函数实现的，同时方便继承，只是一个比较好的编程习惯
		virtual bool IsKeyPressedImpl(int keycode) override;

        virtual bool IsMouseButtonPressedImpl(int button) override;
        virtual std::pair<float, float> GetMousePositionImpl() override;
        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;
		 
	};
}
```

## src\Platform\Windows\WindowsWindow.cpp
```
﻿#include "hzpch.h"
#include "WindowsWindow.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Hazel {
	// 静态变量，用于跟踪GLFW是否已初始化
	static bool s_GLFWInitialized = false;

	// GLFW错误回调函数
	static void GLFWErrorCallback(int error, const char* description)
	{
		HAZEL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	// 创建窗口的静态工厂方法
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	// WindowsWindow构造函数
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	// WindowsWindow析构函数
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	// 设置垂直同步
	void WindowsWindow::SetVSync(bool enabled)
	{
		// 根据enabled参数设置交换间隔
		// 1表示启用垂直同步，0表示禁用
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		// 保存VSync状态
		m_Data.VSync = enabled;
	}

	// 获取垂直同步状态
	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	// 初始化窗口
	void WindowsWindow::Init(const WindowProps& props)
	{
		// 初始化窗口数据
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;


		HAZEL_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		// 如果GLFW未初始化，进行初始化
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			HAZEL_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		// 创建GLFW窗口
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();


		// 设置窗口用户指针，用于回调函数中访问窗口数据
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// 设置GLFW回调函数

		// 窗口大小改变回调
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				// 这里已经搞懂了，前面有一句glfwSetWindowUserPointer(m_Window, &m_Data);意思是把窗口的指针设为m_Data的指针
				// 所以glfwGetWindowUserPointer就会返回m_Data的指针
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window); // 获取窗口数据,这里有点奇怪，因为WindowData是自定义结构体，不知道为什么这样可以直接转换

				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				HAZEL_CORE_WARN("{0}, {1}", width, height);
				data.EventCallback(event);
			});

		// 窗口关闭回调
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		// 键盘按键回调
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});

		// 字符输入回调
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
			});

		// 鼠标按钮回调
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});

		// 鼠标滚轮回调
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		// 鼠标位置回调
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
	}

	// 关闭窗口
	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	// 窗口更新函数
	void WindowsWindow::OnUpdate()
	{
		// 处理GLFW事件
		glfwPollEvents();
		m_Context->SwapBuffers();
	}
}
```

## src\Platform\Windows\WindowsWindow.h
```
﻿#pragma once

#include "Hazel/Window.h"
#include "Hazel/Renderer/GraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Hazel {
    class HAZEL_API WindowsWindow : public Window {
    // 基本上就是对抽象窗口的一些实现
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetHeight() const override { return m_Data.Height; };
        inline unsigned int GetWidth() const override { return m_Data.Width; };

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        // 获取原生窗口
        inline virtual void* GetNativeWindow() const { return m_Window; };
    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();
    private:
        GLFWwindow* m_Window;
        GraphicsContext* m_Context;

        struct WindowData {
            std::string Title;
            unsigned int Width;
            unsigned int Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}
```

