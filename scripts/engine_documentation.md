# Engine 项目文件结构
生成时间: 2025-01-23 02:02:09

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
      Window.h
      Events/
        ApplicationEvent.h
        Event.h
        KeyEvent.h
        MouseEvent.h
      ImGui/
        ImGuiBuild.cpp
        ImGuiLayer.cpp
        ImGuiLayer.h
    Platform/
      OpenGL/
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
)

# 输出相对路径的源文件列表（可选，用于调试）
foreach(SOURCE ${HAZEL_SOURCES})
    file(RELATIVE_PATH REL_SOURCE ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCE})
    message(STATUS "Found source file: ${REL_SOURCE}")
endforeach()
# message(STATUS "Found source files: ${HAZEL_SOURCES}")

add_library(Engine SHARED
    ${HAZEL_SOURCES}
)

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HAZEL
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform
    ${INCLUDE_DIR}
    ${VENDOR_DIR}/GLFW/include
    ${VENDOR_DIR}/spdlog/include
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

// -----------Entry point-----------
#include "Hazel/EntryPoint.h"
// --------------------------------

```

## src\hzpch.cpp
```
#include "hzpch.h"
```

## src\hzpch.h
```
#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <string>
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
#include <gl/GL.h>
#include "Hazel/Input.h"
#include "glm/glm.hpp"  

namespace Hazel {

// 这里绑定要this的原因是成员函数需要具体实例才能运行
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application() {
        HAZEL_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application() {
    }

    void Application::Run() {
        while (m_Running) { 
            //glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // 添加背景色
            glClear(GL_COLOR_BUFFER_BIT);
            // 这里从begin开始渲染，到end结束渲染。
            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate();
            }
            m_Window->OnUpdate();

            m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
            m_ImGuiLayer->End();
        }
    }
    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        //HAZEL_CORE_TRACE("{0}", e);

        // 这里从end开始处理事件，到这个处理结束为止。
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->OnEvent(e); // 这里是Layer的OnEvent接口，不是Application的OnEvent接口。
            if (e.IsHandled()) {
                break;
            }
        }

        auto [x, y] = Input::GetMousePosition();
        HAZEL_CORE_TRACE("{0}, {1}", x, y);
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

        std::unique_ptr<Window> m_Window;      // 应用程序窗口的智能指针
        ImGuiLayer* m_ImGuiLayer; // 
        bool m_Running = true;                 // 应用程序运行状态标志
        LayerStack m_LayerStack;              // 层栈，用于管理应用程序的各个层

    private:
        static Application* s_Instance;        // 应用程序的单例实例指针
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

namespace Hazel {

	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
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
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger(){return s_CoreLogger;}
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger(){return s_ClientLogger;}
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
        using EventFn = std::function<bool(T&)>;
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
template <>
struct fmt::formatter<Hazel::Event> : fmt::ostream_formatter {};
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
		: Layer("ImGuiLayer") {
	}

	ImGuiLayer::~ImGuiLayer() {}
	// 初始化设置ImGui所有窗口的属性，使ImGui窗口能有停靠、独立的UI窗口特性

	void ImGuiLayer::OnAttach() {
		// 不需要手动写ImGui的键值对应GLFW的键值、ImGui接收GLFW窗口事件，ImGui自动完成
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

	void ImGuiLayer::OnDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End() {
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
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
	void ImGuiLayer::OnImGuiRender() {
		static bool show = true;
		ImGui::ShowDemoWindow(&show);// 当前OnImGuiRender层显示DemoUI窗口
	}
}

```

## src\Hazel\ImGui\ImGuiLayer.h
```
﻿#pragma once

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
        
        // 设置OpenGL上下文
        glfwMakeContextCurrent(m_Window);
        
        // 初始化GLAD
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        HAZEL_CORE_ASSERT(status, "Failed to initialize Glad!");
        
        // 设置窗口用户指针，用于回调函数中访问窗口数据
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // 设置GLFW回调函数
        
        // 窗口大小改变回调
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;

                WindowResizeEvent event(width, height);
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
        // 交换缓冲区
        glfwSwapBuffers(m_Window);
    }
}
```

## src\Platform\Windows\WindowsWindow.h
```
﻿#pragma once

#include "Hazel/Window.h"

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

