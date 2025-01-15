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
}