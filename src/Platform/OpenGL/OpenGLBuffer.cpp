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

	OpenGLVertexBuffer::OpenGLVertexBuffer(Vertex* a_vertices, uint32_t a_size)
	{
		m_bufferSize = a_size;

		glCreateBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, a_size, a_vertices, GL_STATIC_DRAW);


		m_vertics = new Vertex[m_bufferSize];
		memcpy(m_vertics, a_vertices, m_bufferSize);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
		delete[] m_vertics;
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/////////////////////////////////////////////////////////////////////////
	// INDEX BUFFER
	/////////////////////////////////////////////////////////////////////////
	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* a_indices, unsigned int a_count)
	{
		m_count = a_count;

		glCreateBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_count * sizeof(unsigned int), a_indices, GL_STATIC_DRAW);

		m_indices = new unsigned int[m_count];
		int bufferSize = sizeof(unsigned int) * m_count;
		memcpy(m_indices, a_indices, bufferSize);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
		delete[] m_indices;
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