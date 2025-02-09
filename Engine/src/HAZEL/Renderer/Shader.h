#pragma once
#include "Hazel/Core.h"
#include <string> //pch一般只放cpp文件
#include <unordered_map>

namespace Hazel {

	class HAZEL_API Shader
	{
    public:	
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string& filePath);
	};

	class ShaderLibrary {
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader); // 这个允许自定义命名
		Ref<Shader> Load(const std::string& filePath);
        Ref<Shader> Load(const std::string& name, const std::string& filePath);

		Ref<Shader> Get(const std::string& name);
		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}