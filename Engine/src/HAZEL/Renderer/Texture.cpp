#include "hzpch.h"
#include "Hazel/Renderer/Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Hazel/Renderer/Renderer.h"

namespace Hazel {
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HAZEL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path); //这里make_shared就不需要使用ptr.reset()了，只需要ptr = Texture2D::Create(path)
		}

		HAZEL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}