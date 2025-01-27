#include "hzpch.h"
#include "OpenGLContext.h"
#include "Hazel/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HAZEL_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		// 设置OpenGL上下文
		glfwMakeContextCurrent(m_WindowHandle);

		// 初始化GLAD
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HAZEL_CORE_ASSERT(status, "Failed to initialize Glad!");

        HAZEL_CORE_INFO("OpenGL Info:");
		HAZEL_CORE_INFO("Vendor: {0}", (const char*)glGetString(GL_VENDOR));
        HAZEL_CORE_INFO("Renderer: {0}", (const char*)glGetString(GL_RENDERER));
        HAZEL_CORE_INFO("OpenGL Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		// 交换缓冲区
		glfwSwapBuffers(m_WindowHandle);
	}

}