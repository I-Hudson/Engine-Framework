#pragma once

#include "Renderer/RendererAPI.h"

namespace Framework
{
	class OpenGLRendererAPI : public Renderer::RendererAPI
	{
	public:
		virtual void BeginRender(Renderer::GBuffer* gBuffer) override;
		virtual void EndRender(Renderer::GBuffer* gBuffer) override;
		virtual void SetClearColor(const glm::vec4& a_color) override;
		virtual void Clear() override;
		virtual void SetVSync(bool a_state) override;
		virtual void SetDepthTest(bool a_state) override;
		virtual void SetCullFace(bool a_state) override;

		virtual void SetContext(Framework::Renderer::GraphicsContext* context) override { }

		virtual void DrawIndexed(const Renderer::VertexArray* a_vertexArray, Renderer::GBuffer* gBuffer) override;
	};
}