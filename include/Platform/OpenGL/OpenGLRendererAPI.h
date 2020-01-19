#pragma once

#include "Renderer/RendererAPI.h"

namespace Framework
{
	class OpenGLRendererAPI : public Renderer::RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& a_color) override;
		virtual void Clear() override;
		virtual void SetVSync(bool a_state) override;
		virtual void SetDepthTest(bool a_state) override;
		virtual void SetCullFace(bool a_state) override;

		virtual void DrawIndexed(const std::shared_ptr<Renderer::VertexArray>& a_vertexArray) override;
	};
}