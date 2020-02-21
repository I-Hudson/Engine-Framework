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

		void VulkanCommand::CreateCommandPool(VkCommandPool* commandPool)
		{
			QueueFamilyIndices queueFamilyIndices = VulkanContext::Get().GetVulkanQueue()->FindQueueFamilies();

			VkCommandPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

			if (vkCreateCommandPool(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), &poolInfo, nullptr, commandPool) != VK_SUCCESS)
			{
				EN_CORE_ERROR("VulkanCommand: Command Pool was not created!");
			}
		}

		void VulkanCommand::FreeCommandPool(VkCommandPool* commandPool)
		{
			vkDestroyCommandPool(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), *commandPool, nullptr);
		}

		void VulkanCommand::CreateCommandBuffers(unsigned int count, std::vector<VkCommandBuffer>* buffers, VkCommandPool* commandPool)
		{
			if (buffers->size() != count)
			{
				buffers->resize(count);
			}

			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = *commandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = (uint32_t)buffers->size();

			if (vkAllocateCommandBuffers(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), &allocInfo, buffers->data()) != VK_SUCCESS)
			{
				EN_CORE_ERROR("VulkanCommand: Failed to allocate command buffers!");
			}
		}

		void VulkanCommand::FreeCommandBuffers(std::vector<VkCommandBuffer>* buffers, VkCommandPool* commandPool)
		{
			vkFreeCommandBuffers(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), *commandPool, static_cast<uint32_t>(buffers->size()), buffers->data());
		}

		void VulkanCommand::EndCommandRecord(std::vector<VkCommandBuffer>* buffers)
		{
			for (size_t i = 0; i < buffers->size(); i++)
			{
				vkCmdEndRenderPass((*buffers)[i]);

				if (vkEndCommandBuffer((*buffers)[i]) != VK_SUCCESS)
				{
					EN_CORE_ERROR("Vulkan Render API: Failed to record command buffer");
				}
			}
		}
	}
}