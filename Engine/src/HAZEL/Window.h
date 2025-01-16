#pragma once

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


        // 创建窗口的静态工厂方法
        // @param props 窗口属性,包含标题、宽度和高度
        // @return 返回创建的窗口实例指针
        static Window* Create(const WindowProps& props = WindowProps());
    };
}