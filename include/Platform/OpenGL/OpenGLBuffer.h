#pragma once

#include "Buffer.h"

namespace Framework
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* aVertices, uint32_t aSize);
		OpenGLVertexBuffer(Vertex* aVertices, uint32_t aSize);
		virtual ~OpenGLVertexBuffer();
	
		//Bind and unbind
		virtual void Bind()  const override;
		virtual void Unbind() const override;
	
		//Set the layout for this buffer
		virtual void SetLayout(const BufferLayout& aLayout) override { mLayout = aLayout; }
		//Get the layout for this buffer
		virtual const BufferLayout& GetLayout() const override { return mLayout; }

	private:
		uint32_t m_ID;
		BufferLayout mLayout;
	};
	
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned int* indices, unsigned int count);
		virtual ~OpenGLIndexBuffer();
	
		//Bind and unbind
		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		uint32_t m_ID;
	};
}