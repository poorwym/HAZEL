#pragma once
/**
 * @note: 这里是application，HAZEL引擎可以创建一个这样的Application然后运行。
 */
#include "Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Window.h"
#include "LayerStack.h"

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
