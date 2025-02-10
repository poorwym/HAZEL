#pragma once
#include "Hazel/Core.h"
#include <glm/glm.hpp>

namespace Hazel {

	class HAZEL_API	OrthographicCamera {
	public:
        OrthographicCamera(float left, float right, float bottom, float top);
        void SetProjection(float left, float right, float bottom, float top);

        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); };
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); };

        const glm::vec3& GetPosition() const { return m_Position; };
        float GetRotation() const { return m_Rotation; };

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; };
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; };
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; };
    private:
        void RecalculateViewMatrix();
	private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix; // for cache
        
		glm::vec3 m_Position = glm::vec3(0.0f);
        float m_Rotation = 0.0f; // 正交投影相机一般没有3维旋转（除非博那种），一个浮点数表示z轴的旋转角度就行了
	};

}