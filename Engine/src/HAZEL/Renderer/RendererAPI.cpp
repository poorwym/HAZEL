#include "hzpch.h"
#include "RendererAPI.h"

namespace Hazel {
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL; //这样调用RendererAPI就会直接用OpenGLRendererAPI
}