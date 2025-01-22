#include "hzpch.h"

#include "ImGuiLayer.h"
#include "imgui.h"

#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"
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

		
	}

	// 当ImGui层从层栈移除时调用
	void ImGuiLayer::OnDetach()
	{
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		ImGuiIO& io = ImGui::GetIO();

		static bool show = true;
		ImGui::ShowDemoWindow();
		//ImGui::ShowMetricsWindow();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
        
		// Rendering
		ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
		}
	}
}