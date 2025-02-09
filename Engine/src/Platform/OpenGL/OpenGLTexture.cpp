#include "hzpch.h"

#include "OpenGLTexture.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace Hazel {


	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1); // 翻转图片
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		HAZEL_CORE_ASSERT(data, "Failed to load image!");

        m_Width = width;
        m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		// internalFormat 指定了OpenGL如何在GPU内存中存储纹理数据
		// 例如 GL_RGBA8 表示使用8位存储每个颜色分量(红、绿、蓝、透明度)
		// 这是纹理在GPU中的内部格式

		// dataFormat 指定了输入图像数据的格式
		// 例如 GL_RGBA 表示输入数据是按照红、绿、蓝、透明度的顺序排列的
		// 这是从CPU传输到GPU的数据格式
		
		if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
		}
		else if(channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
		}

		HAZEL_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID); // 创建纹理

        glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height); // 分配空间

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 设置纹理参数,GL_TEXTURE_MIN_FILTER意思是缩小时使用线性插值
		 // glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 设置纹理参数,GL_TEXTURE_MAG_FILTER意思是放大时使用线性插值
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 设置纹理参数,GL_NEAREST 使用最接近的像素
		
		// 更新纹理数据,参数依次为:纹理ID,mipmap级别,x偏移,y偏移,宽度,高度,像素数据格式,像素数据类型,像素数据
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
		// 之后纹理数据都存在于GPU了

        stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

}