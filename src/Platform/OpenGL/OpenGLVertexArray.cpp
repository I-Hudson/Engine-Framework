#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Framework
{
	static GLenum ShaderDataTypeToOpenGLBaseType(Renderer::ShaderDataType aType)
	{
		switch (aType)
		{
		case Renderer::ShaderDataType::Float:	return GL_FLOAT;
		case Renderer::ShaderDataType::Float2:	return GL_FLOAT;
		case Renderer::ShaderDataType::Float3:	return GL_FLOAT;
		case Renderer::ShaderDataType::Float4:	return GL_FLOAT;
		case Renderer::ShaderDataType::Mat3:		return GL_FLOAT;
		case Renderer::ShaderDataType::Mat4:		return GL_FLOAT;
		case Renderer::ShaderDataType::Int:		return GL_INT;
		case Renderer::ShaderDataType::Int2:		return GL_INT;
		case Renderer::ShaderDataType::Int3:		return GL_INT;
		case Renderer::ShaderDataType::Int4:		return GL_INT;
		case Renderer::ShaderDataType::Bool:		return GL_BOOL;
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

	void OpenGLVertexArray::Bind(void* object) const
	{
		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::Unbind(void* object) const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetSubVertexData(const Renderer::Vertex* a_vertices, const unsigned int& a_bufferSize)
	{
		m_vertexBuffers[0]->SetSubData(a_vertices, a_bufferSize);
	}

	void OpenGLVertexArray::SetSubIndexData(const unsigned int* a_indices, const unsigned int& a_count)
	{
		m_indexBuffer->SetSubData(a_indices, a_count);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<Renderer::VertexBuffer>& a_vertexBuffer)
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

	void OpenGLVertexArray::AddIndexBuffer(const std::shared_ptr<Renderer::IndexBuffer>& a_indexBuffer)
	{
		Bind();
		a_indexBuffer->Bind();
		m_indexBuffer = a_indexBuffer;
		Unbind();
	}

	const std::vector<std::shared_ptr<Renderer::VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
	{
		return m_vertexBuffers;
	}

	const std::shared_ptr<Renderer::IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_indexBuffer;
	}
}