#pragma once
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