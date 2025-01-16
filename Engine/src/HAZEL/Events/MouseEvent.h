#pragma once 

#include "hzpch.h"
#include "Event.h"

namespace Hazel {
    //先写public，再写private，因为public是给用户看的，接口比内部实现更重要
    class HAZEL_API MouseMovement : public Event{
    public:
        MouseMovement(float x, float y)
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

    class HAZEL_API MouseScrolled : public Event{
    public:
        MouseScrolled(float xOffset, float yOffset)
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

    class HAZEL_API MouseButtonRleasedEvent : public MouseButtonEvent{
    public:
        MouseButtonRleasedEvent(const int button)
        :MouseButtonEvent(button){}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseButtonReleased);
    };
}

 
