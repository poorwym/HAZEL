#include "hzpch.h"
#include "OpenGLShader.h"
#include "Hazel/Log.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
namespace Hazel {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") {
            return GL_VERTEX_SHADER;
		}
        if (type == "fragment" || type == "pixel") {
            return GL_FRAGMENT_SHADER;
		}

        HAZEL_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		std::string source = ReadFile(filePath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// assets/shaders/Texture.glsl
		auto lastSlash = filePath.find_last_of("/\\"); // 因为windows有/或者\\，非常奇怪
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1; //有可能路径上没有slash
		// 还需要考虑文件名有没有dot（这个真的细）
		auto lastDot = filePath.rfind('.'); 
		size_t count = lastDot == std::string::npos ? filePath.length() - lastSlash : lastDot - lastSlash;
		/*
		* @note:如果你要查找某个字符串（子串）最后一次出现的位置 → 用 rfind
		*		如果你要查找多个字符（字符集合）中的任意一个最后一次出现的位置 → 用 find_last_of
		*/
        m_Name = filePath.substr(lastSlash, count);
	} 

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		std::string result;
		// 打开着色器文件
		// filePath: 文件路径
		// std::ios::in: 以读取模式打开文件
		// std::ios::binary: 以二进制模式打开文件，防止文本转换导致的问题
		std::ifstream in(filePath, std::ios::in | std::ios::binary); // 这里只能接受2个参数

		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			HAZEL_CORE_ERROR("Could not open shader file! {0}", filePath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos) // std::string::npos 表示找不到
        {
            size_t eol = source.find_first_of("\r\n", pos); // 找到换行符的位置, eol 是换行符的位置
			HAZEL_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1; // "#type"后面只能有一个空格, begin 是那个空格之后的位置
			std::string type = source.substr(begin, eol - begin); //begin 到 eol 之间的内容
            HAZEL_CORE_ASSERT(ShaderTypeFromString(type) != 0, "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); // 找到下一个换行符的位置
            pos = source.find(typeToken, nextLinePos); // 找到下一个 #type
            shaderSources[ShaderTypeFromString(type)] = //
				(pos == std::string::npos) // 如果找不到 #type, 说明是 shader 最后一个片段
				? source.substr(nextLinePos) // 从 nextLinePos 到 最后
				: source.substr(nextLinePos, pos - nextLinePos); // 从 nextLinePos 到 pos
        }

		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
	{
		GLuint program = glCreateProgram(); //这里和另外一个构造函数不一样，因为如果编译失败m_RendererID仍然不为0
		// std::vector<GLuint> glShaderIDs; // vector在堆上分配内存，貌似慢一点
		HAZEL_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now!");
		std::array<GLuint, 2> glShaderIDs; // 创建一个数组，用来存储编译后的着色器
		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources) {
            GLenum type = kv.first;
            const std::string& source = kv.second; 

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				HAZEL_CORE_ERROR("{0}", infoLog.data());
				HAZEL_CORE_ASSERT(false, "Shader compliation failed!");
				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs) {
                glDeleteShader(id);
			}

			// HAZEL_CORE_ERROR("fragment shader compilation failure!");
			HAZEL_CORE_ERROR("{0}", infoLog.data());
			HAZEL_CORE_ASSERT(false, "program links error!");
			return;
		}

		for (auto id : glShaderIDs) {
            glDetachShader(program, id);
		}

		m_RendererID = program; // 如果m_RendererID不为0，说明编译成功
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
		// HAZEL_CORE_ASSERT(m_UniformLocationCache.find(name) != m_UniformLocationCache.end(), "Uniform not found!");
	}

	

}
