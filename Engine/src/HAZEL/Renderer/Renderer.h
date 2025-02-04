﻿#pragma once
#include "Hazel/Core.h"

#include "RenderCommand.h"

namespace Hazel {

	class HAZEL_API Renderer {
	public:
		static void BeginScene();

        static void EndScene();
		 
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}
