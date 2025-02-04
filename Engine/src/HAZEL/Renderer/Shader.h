#pragma once
#include "Hazel/Core.h"
#include <string> //pch一般只放cpp文件
namespace Hazel {

	class HAZEL_API Shader
	{
    public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
        void Unbind() const;
	private:
		uint32_t m_RendererID; //使用uint32_t不用GLuint是因为跨平台需要，不必要引入过多的头文件

	};

}