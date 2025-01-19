#include "hzpch.h"

#include "ImGuiLayer.h"
#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "Hazel/Application.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Hazel {

	// ImGui层构造函数
	// 通过基类Layer的构造函数设置层名为"ImGuiLayer"
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	// ImGui层析构函数
	ImGuiLayer::~ImGuiLayer()
	{
	}

	// 当ImGui层被添加到层栈时调用
	void ImGuiLayer::OnAttach()
	{ 
		// 创建ImGui上下文
		ImGui::CreateContext();
		// 设置ImGui暗色主题
		ImGui::StyleColorsDark();

		// 获取ImGui IO对象的引用
		ImGuiIO& io = ImGui::GetIO();
		// 设置后端标志
		// 表示后端支持鼠标光标
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		// 表示后端支持设置鼠标位置
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// 设置键盘映射
		// 将ImGui的按键映射到GLFW的按键码
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		// 初始化ImGui的OpenGL3渲染器
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	// 当ImGui层从层栈移除时调用
	void ImGuiLayer::OnDetach()
	{
	}

	// ImGui层的更新函数，每帧调用
	void ImGuiLayer::OnUpdate()
	{
		// 获取ImGui IO对象和应用实例
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		
		// 设置显示尺寸为当前窗口尺寸
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// 计算帧间隔时间
		float time = (float)glfwGetTime();
		io.DeltaTime = time - m_Time;
		m_Time = time;

		// 开始新的ImGui帧
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// 显示ImGui演示窗口
		static bool show_demo_window = true;
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 渲染ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(HAZEL_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(HAZEL_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(HAZEL_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(HAZEL_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
        dispatcher.Dispatch<KeyPressedEvent>(HAZEL_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(HAZEL_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(HAZEL_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<WindowResizeEvent>(HAZEL_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));

        //dispatcher.Dispatch<WindowCloseEvent>(HAZEL_BIND_EVENT_FN(ImGuiLayer::OnWindowCloseEvent))
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = true;
		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{ 
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = false;
		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
        ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
        io.MouseWheel += e.GetYOffset();
		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
        ImGuiIO& io = ImGui::GetIO();
		int KeyCode = e.GetKeyCode();

        if (KeyCode > 0 && KeyCode < 0x10000)
            io.AddInputCharacter((unsigned short)KeyCode);
		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, e.GetWidth(), e.GetHeight());
		return false;
	}

	


















}