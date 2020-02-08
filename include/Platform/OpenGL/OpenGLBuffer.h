#pragma once

#include "Renderer/Buffer.h"

namespace Framework
{
	class OpenGLVertexBuffer : public Renderer::VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* aVertices, uint32_t aSize);
		OpenGLVertexBuffer(Renderer::Vertex* aVertices, uint32_t aSize);
		virtual ~OpenGLVertexBuffer();
	
		//Bind and unbind
		virtual void Bind(void* object = nullptr)  const override;
		virtual void Unbind(void* object = nullptr) const override;
	
		virtual void SetSubData(const Renderer::Vertex* a_vertices, const unsigned int& a_bufferSize) override;

		//Set the layout for this buffer
		virtual void SetLayout(const Renderer::BufferLayout& aLayout) override { mLayout = aLayout; }
		//Get the layout for this buffer
		virtual const Renderer::BufferLayout& GetLayout() const override { return mLayout; }

	private:
		uint32_t m_ID;
		Renderer::BufferLayout mLayout;
	};
	
	class OpenGLIndexBuffer : public Renderer::IndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned int* indices, unsigned int count);
		virtual ~OpenGLIndexBuffer();
	
		virtual void SetSubData(const unsigned int* a_indices, const unsigned int& a_count) override;

		//Bind and unbind
		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		uint32_t m_ID;
	};
}