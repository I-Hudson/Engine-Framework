#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace Framework
{
	namespace Vulkan
	{
		void VulkanRendererAPI::BeginRender()
		{
			auto commandBuffers = *m_vkContext->GetVulkanCommand()->GetCommandBuffers();
			for (size_t i = 0; i < commandBuffers.size(); i++) 
			{
				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = 0; // Optional
				beginInfo.pInheritanceInfo = nullptr; // Optional

				if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) 
				{
					EN_CORE_ERROR("Vulkan Render API: Failed to begin recording command buffer!");
				}

				VkRenderPassBeginInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = *m_vkContext->GetVulkanSwapchain()->GetRenderPass();
				renderPassInfo.framebuffer = static_cast<std::vector<VkFramebuffer>>(*m_vkContext->GetVulkanSwapchain()->GetSwapChainFrameBuffers())[i];
				
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = *m_vkContext->GetVulkanSwapchain()->GetSwapChainExtent();

				VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
				renderPassInfo.clearValueCount = 1;
				renderPassInfo.pClearValues = &clearColor;

				vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			}
		}

		void VulkanRendererAPI::EndRender()
		{
			auto commandBuffers = *m_vkContext->GetVulkanCommand()->GetCommandBuffers();
			for (size_t i = 0; i < commandBuffers.size(); i++)
			{
				vkCmdEndRenderPass(commandBuffers[i]);

				if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
				{
					EN_CORE_ERROR("Vulkan Render API: Failed to record command buffer");
				}
			}

			uint32_t imageIndex;
			vkAcquireNextImageKHR(*m_vkContext->GetVulkanDevice()->GetDevice(), *m_vkContext->GetVulkanSwapchain()->GetSwapChain(),
				UINT64_MAX, *m_vkContext->GetVulkanSync()->GetImageSemaphore(), VK_NULL_HANDLE, &imageIndex);

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = { *m_vkContext->GetVulkanSync()->GetImageSemaphore() };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

			VkSemaphore signalSemaphores[] = { *m_vkContext->GetVulkanSync()->GetRenderSemaphore() };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			if (vkQueueSubmit(*m_vkContext->GetVulkanQueue()->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
			{
				EN_CORE_ERROR("VulkanRendererAPI: Failed to submit draw command to bffer!");
			}

			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			VkSwapchainKHR swapChains[] = { *m_vkContext->GetVulkanSwapchain()->GetSwapChain() };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &imageIndex;
			presentInfo.pResults = nullptr; // Optional
			vkQueuePresentKHR(*m_vkContext->GetVulkanQueue()->GetPresentQueue(), &presentInfo);
		}

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
			auto commandBuffers = *m_vkContext->GetVulkanCommand()->GetCommandBuffers();
			for (size_t i = 0; i < commandBuffers.size(); i++)
			{
				vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
			}
		}
	}
}