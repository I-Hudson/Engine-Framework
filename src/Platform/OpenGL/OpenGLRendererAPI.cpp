#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Framework
{
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& a_color)
	{
		glClearColor(a_color.r, a_color.g, a_color.b, a_color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& a_vertexArray)
	{
		glDrawElements(GL_TRIANGLES, a_vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}