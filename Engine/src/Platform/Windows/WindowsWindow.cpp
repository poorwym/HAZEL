#include "hzpch.h"
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
        m_Context -> Init();
       
        
        // 设置窗口用户指针，用于回调函数中访问窗口数据
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // 设置GLFW回调函数
        
        // 窗口大小改变回调
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window); // 获取窗口数据,这里有点奇怪，因为WindowData是自定义结构体，不知道为什么这样可以直接转换
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
        m_Context->SwapBuffers();
    }
}