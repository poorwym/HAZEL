#pragma once
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;  

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffer() const override { return m_VertexBuffers; };
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; };


	private:
		// 允许一个vao拥有多个vbo，比如一个vbo存储顶点，一个vbo存储颜色，一个vbo存储纹理坐标，然后用不同的顶点属性来区分数据
		// 这样其实和一个很大的vbo用很多的顶点属性来区分数据差不多
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers; 
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_RendererID;
	};
}