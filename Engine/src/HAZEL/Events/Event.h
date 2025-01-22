#pragma once
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