#pragma once
#include "Hazel/Core.h"
#include <string> //pch一般只放cpp文件

namespace Hazel {

	class HAZEL_API Shader
	{
    public:	
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};

}