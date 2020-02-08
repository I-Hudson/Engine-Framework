#include "Platform/Vulkan/Internal/VulkanCommand.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace Framework
{
	namespace Vulkan
	{
		VulkanCommand::VulkanCommand()
		{
		}

		VulkanCommand::~VulkanCommand()
		{
		}

		void VulkanCommand::Setup(VulkanContext* context)
		{
			m_vkContext = context;

			QueueFamilyIndices queueFamilyIndices = m_vkContext->GetVulkanQueue()->FindQueueFamilies(*m_vkContext->GetVulkanDevice()->GetPhyiscalDevice());

			VkCommandPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

			if (vkCreateCommandPool(*m_vkContext->GetVulkanDevice()->GetDevice(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
			{
				EN_CORE_ERROR("VulkanCommand: Command Pool was not created!");
			}

			CreateCommandBuffers();
		}

		void VulkanCommand::CreateCommandBuffers()
		{
			m_commandBuffers.resize(m_vkContext->GetVulkanSwapchain()->GetImageViews()->size());

			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = m_commandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

			if (vkAllocateCommandBuffers(*m_vkContext->GetVulkanDevice()->GetDevice(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
			{
				EN_CORE_ERROR("VulkanCommand: Failed to allocate command buffers!");
			}
		}

		void VulkanCommand::FreeCommandBuffers()
		{
			vkFreeCommandBuffers(*m_vkContext->GetVulkanDevice()->GetDevice(), m_commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
		}

		void VulkanCommand::EndCommandRecord()
		{
			for (size_t i = 0; i < m_commandBuffers.size(); i++)
			{
				vkCmdEndRenderPass(m_commandBuffers[i]);

				if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS)
				{
					EN_CORE_ERROR("Vulkan Render API: Failed to record command buffer");
				}
			}
		}

		void VulkanCommand::DestroyCommandPool()
		{
			vkDestroyCommandPool(*m_vkContext->GetVulkanDevice()->GetDevice(), m_commandPool, nullptr);
		}
	}
}