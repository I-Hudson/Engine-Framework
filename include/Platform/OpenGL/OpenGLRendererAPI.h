#pragma once

#include "RendererAPI.h"

namespace Framework
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& a_color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& a_vertexArray) override;
	};
}