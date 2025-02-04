#pragma once
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
