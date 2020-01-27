#pragma once

#include "Renderer/RendererAPI.h"

namespace Framework
{
	namespace Vulkan
	{
		class VulkanContext;

		class VulkanRendererAPI : public Renderer::RendererAPI
		{
		public:
			virtual void BeginRender() override;
			virtual void EndRender() override;
			virtual void SetClearColor(const glm::vec4& a_color) override;
			virtual void Clear() override;
			virtual void SetVSync(bool a_state) override;
			virtual void SetDepthTest(bool a_state) override;
			virtual void SetCullFace(bool a_state) override;

			virtual void SetContext(Framework::Renderer::GraphicsContext* context) override;

			virtual void DrawIndexed(const std::shared_ptr<Renderer::VertexArray>& a_vertexArray) override;

		private:
			VulkanContext* m_vkContext;
		};
	}
}