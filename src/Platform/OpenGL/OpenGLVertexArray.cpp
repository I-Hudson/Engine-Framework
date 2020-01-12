#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Framework
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType aType)
	{
		switch (aType)
		{
		case ShaderDataType::Float:	return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Bool:		return GL_BOOL;
		}
		//EN_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_ID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetSubVertexData(const Vertex* a_vertices, const unsigned int& a_bufferSize)
	{
		m_vertexBuffers[0]->SetSubData(a_vertices, a_bufferSize);
	}

	void OpenGLVertexArray::SetSubIndexData(const unsigned int* a_indices, const unsigned int& a_count)
	{
		m_indexBuffer->SetSubData(a_indices, a_count);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& a_vertexBuffer)
	{
		if (a_vertexBuffer->GetLayout().GetElements().size() == 0)
		{
			return;
		}

		Bind();
		a_vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = a_vertexBuffer->GetLayout();
		for (auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}

		m_vertexBuffers.push_back(a_vertexBuffer);

		Unbind();
	}

	void OpenGLVertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& a_indexBuffer)
	{
		Bind();
		a_indexBuffer->Bind();
		m_indexBuffer = a_indexBuffer;
		Unbind();
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
	{
		return m_vertexBuffers;
	}

	const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_indexBuffer;
	}
}