#pragma once
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void SetProjection(float left, float right, float bottom, float top);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }

		void SetRotation(bool rotation) { m_AllowRotation = rotation; }
		bool GetRotation() const { return m_AllowRotation; }
	private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		bool m_AllowRotation = false;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = glm::vec3(0.0f);
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f; 
	};
};