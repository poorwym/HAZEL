#pragma once
#include "Hazel/Core.h"


namespace Hazel {

	enum class HAZEL_API RendererAPI {
		None = 0,
		OpenGL = 1
	};

	class HAZEL_API Renderer {
	public:
		inline static RendererAPI GetAPI() { return s_API; }
	private:
		static RendererAPI s_API;
	};
}
