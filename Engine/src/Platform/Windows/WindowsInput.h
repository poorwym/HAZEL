#pragma once
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