#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Framework
{
	/////////////////////////////////////////////////////////////////////////
	// VERTEX BUFFER
	/////////////////////////////////////////////////////////////////////////
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* a_vertices, uint32_t a_size)
	{
		glCreateBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, a_size, a_vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(float* a_data, const int a_start, const int& a_end)
	{
		Bind();
		int nBufferSize = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &nBufferSize);
		int originalVertexArraySize = (nBufferSize / sizeof(float));

		glBufferSubData(GL_ARRAY_BUFFER, a_start == 0 ? 0 : a_start, a_end == -1 ? nBufferSize : a_end, a_data);
		Unbind();
	}

	void OpenGLVertexBuffer::GetData(float* a_data, const int& a_start, const int& a_end)
	{
		int nBufferSize = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &nBufferSize);
		int originalVertexArraySize = (nBufferSize / sizeof(float));
		
		if (a_data == nullptr)
		{
			a_data = new float[a_end];
		}
		Bind();
		glGetBufferSubData(GL_ARRAY_BUFFER, a_start == 0 ? 0 : a_start, a_end == -1 ? nBufferSize : a_end, a_data);
		Unbind();
	}

	int OpenGLVertexBuffer::GetBufferSize()
	{
		int nBufferSize = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &nBufferSize);
		return nBufferSize;
	}

	/////////////////////////////////////////////////////////////////////////
	// INDEX BUFFER
	/////////////////////////////////////////////////////////////////////////
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* a_indices, uint32_t a_count)
		: m_count(a_count)
	{
		glCreateBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_count * sizeof(uint32_t), a_indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}