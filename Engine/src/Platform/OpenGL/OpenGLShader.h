#pragma once
#include "Hazel/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Hazel {

	typedef unsigned int GLenum; //这里很诡异，因为我们不能在这个文件里包含glad，因为glad这个库的路径只能在Engine项目中找到，而sandbox项目使用了这个头文件

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filePath);
		virtual	~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; };

		void UploadUniformInt(const std::string& name, const int values);

		void UploadUniformFloat(const std::string& name, const float values);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source); // 这里是为了可能文件中有多个shader，除了vs和fs外还有几何shader
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);
	private:
		uint32_t m_RendererID; //使用uint32_t不用GLuint是因为跨平台需要，不必要引入过多的头文件
		std::string m_Name;
	};

}
