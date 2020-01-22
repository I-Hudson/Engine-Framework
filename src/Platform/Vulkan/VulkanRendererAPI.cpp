#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace Framework
{
	namespace Vulkan
	{
		void VulkanRendererAPI::SetClearColor(const glm::vec4& a_color)
		{
		}

		void VulkanRendererAPI::Clear()
		{
		}

		void VulkanRendererAPI::SetVSync(bool a_state)
		{
		}

		void VulkanRendererAPI::SetDepthTest(bool a_state)
		{
		}

		void VulkanRendererAPI::SetCullFace(bool a_state)
		{
		}

		void VulkanRendererAPI::SetContext(Framework::Renderer::GraphicsContext* context)
		{
			if (context == nullptr)
			{
				EN_CORE_ERROR("VulkanRendererAPI: Graphics Context is null.");
			}

			m_vkContext = static_cast<VulkanContext*>(context);
			if (m_vkContext == nullptr)
			{
				EN_CORE_ERROR("VulkanRendererAPI: Cast to \"VulklanContext*\" failed.");
			}
		}

		void VulkanRendererAPI::DrawIndexed(const std::shared_ptr<Renderer::VertexArray>& a_vertexArray)
		{
		}
	}
}