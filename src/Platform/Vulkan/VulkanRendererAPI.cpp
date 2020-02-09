#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace Framework
{
	namespace Vulkan
	{
		VulkanRendererAPI::VulkanRendererAPI()
		{
			VkClearValue clearColour = { 0.0f, 0.0f, 0.0f, 1.0f };
			VkClearValue depthClearColour = { 1.0f, 0.0f };

			m_clearColours.push_back(clearColour);
			m_clearColours.push_back(depthClearColour);
		}

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

				renderPassInfo.clearValueCount = static_cast<uint32_t>(m_clearColours.size());
				renderPassInfo.pClearValues = m_clearColours.data();

				vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			}
		}

		void VulkanRendererAPI::GetNextFrameRender()
		{
			m_vkContext->GetVulkanCommand()->EndCommandRecord();

			auto commandBuffers = *m_vkContext->GetVulkanCommand()->GetCommandBuffers();
			auto swapChain = *m_vkContext->GetVulkanSwapchain();
			auto device = *m_vkContext->GetVulkanDevice()->GetDevice();
			auto sync = *m_vkContext->GetVulkanSync();


			vkWaitForFences(device, 1, sync.GetCurrentInFlightFence(), VK_TRUE, UINT64_MAX);

			uint32_t imageIndex;
			VkResult result = vkAcquireNextImageKHR(device, *swapChain.GetSwapChain(),
				UINT64_MAX, *sync.GetCurrentImageSemaphore(), VK_NULL_HANDLE, &imageIndex);
			m_vkContext->SetCurrentImageIndex(imageIndex);

			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				m_vkContext->RecreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			{
				EN_CORE_ERROR("VulkanRendererAPI: Failed to get swap chain image!");
			}
		}

		void VulkanRendererAPI::EndRender()
		{
			GetNextFrameRender();

			auto commandBuffers = *m_vkContext->GetVulkanCommand()->GetCommandBuffers();
			uint32_t imageIndex = m_vkContext->GetCurrentImageIndex();

			if (*m_vkContext->GetVulkanSync()->GetCurrentInFlightImage() != VK_NULL_HANDLE)
			{
				vkWaitForFences(*m_vkContext->GetVulkanDevice()->GetDevice(), 1, m_vkContext->GetVulkanSync()->GetCurrentInFlightImage(), VK_TRUE, UINT64_MAX);
			}

			(*m_vkContext->GetVulkanSync()->GetInFlightImages())[imageIndex] = *m_vkContext->GetVulkanSync()->GetCurrentInFlightFence();

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = { *m_vkContext->GetVulkanSync()->GetCurrentImageSemaphore() };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

			VkSemaphore signalSemaphores[] = { *m_vkContext->GetVulkanSync()->GetCurrentRenderemaphore() };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			vkResetFences(*m_vkContext->GetVulkanDevice()->GetDevice(), 1, m_vkContext->GetVulkanSync()->GetCurrentInFlightFence());

			if (vkQueueSubmit(*m_vkContext->GetVulkanQueue()->GetGraphicsQueue(), 1, &submitInfo, *m_vkContext->GetVulkanSync()->GetCurrentInFlightFence()) != VK_SUCCESS)
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

			vkQueueWaitIdle(*m_vkContext->GetVulkanQueue()->GetPresentQueue());

			m_vkContext->GetVulkanSync()->IncermentCurrentFrame();
		}

		void VulkanRendererAPI::SetClearColor(const glm::vec4& a_color)
		{
			m_clearColours[0] = { a_color.x, a_color.y, a_color.z, a_color.w };
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

		void VulkanRendererAPI::DrawIndexed(const Renderer::VertexArray* a_vertexArray)
		{
			auto commandBuffers = *m_vkContext->GetVulkanCommand()->GetCommandBuffers();
			for (size_t i = 0; i < commandBuffers.size(); i++)
			{
				a_vertexArray->Bind(commandBuffers[i]);

				vkCmdDrawIndexed(commandBuffers[i], a_vertexArray->GetIndexBuffer()->GetCount(), 1, 0, 0, 0);
			}
		}
	}
}