#pragma once

#include "Buffer.h"

namespace Framework
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* aVertices, uint32_t aSize);
		virtual ~OpenGLVertexBuffer();
	
		//Bind and unbind
		virtual void Bind()  const override;
		virtual void Unbind() const override;
	
		//Set the layout for this buffer
		virtual void SetLayout(const BufferLayout& aLayout) override { mLayout = aLayout; }
		//Get the layout for this buffer
		virtual const BufferLayout& GetLayout() const override { return mLayout; }

		virtual void SetData(float* a_data, const int a_start, const int& a_end) override;
		virtual void GetData(float* a_data, const int& a_start = 0, const int& a_end = -1) override;

		virtual int GetBufferSize() override;

	private:
		uint32_t m_ID;
		BufferLayout mLayout;
	};
	
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();
	
		//Bind and unbind
		virtual void Bind() const override;
		virtual void Unbind() const override;
	
		//Get the bumber of indices this buffer holds
		virtual uint32_t GetCount() const override { return m_count; }
	private:
		uint32_t m_ID;
		uint32_t m_count;
	};
}