﻿#include "hzpch.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer.h"

namespace Hazel
{
    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            HAZEL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLVertexArray();
        }

        HAZEL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}