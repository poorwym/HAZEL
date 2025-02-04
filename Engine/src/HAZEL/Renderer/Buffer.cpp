#include "hzpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Hazel/Log.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
        case RendererAPI::API::None:
			HAZEL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
        case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}
		HAZEL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
	}

	
    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None :
            HAZEL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
        case RendererAPI::API::OpenGL:
            return new OpenGLIndexBuffer(indices, count);
        }
        HAZEL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}