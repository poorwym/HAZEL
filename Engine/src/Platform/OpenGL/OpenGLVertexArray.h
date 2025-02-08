#pragma once
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;  

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const override { return m_VertexBuffers; };
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; };


	private:
		// 允许一个vao拥有多个vbo，比如一个vbo存储顶点，一个vbo存储颜色，一个vbo存储纹理坐标，然后用不同的顶点属性来区分数据
		// 这样其实和一个很大的vbo用很多的顶点属性来区分数据差不多
		std::vector<Ref<VertexBuffer>> m_VertexBuffers; 
        Ref<IndexBuffer> m_IndexBuffer;
		uint32_t m_RendererID;
	};
}