#include "Platform/Vulkan/Internal/VulkanSync.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace Framework
{
	namespace Vulkan
	{
		VulkanSync::VulkanSync()
		{
		}

		VulkanSync::~VulkanSync()
		{
		}

		void VulkanSync::Setup(VulkanContext* context)
		{
			m_vkContext = context;

			VkSemaphoreCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			if (vkCreateSemaphore(*m_vkContext->GetVulkanDevice()->GetDevice(), &info, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
				vkCreateSemaphore(*m_vkContext->GetVulkanDevice()->GetDevice(), &info, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS)
			{
				EN_CORE_ERROR("VulkanSync: vkCreateSemaphore failed!");
			}
		}

		void VulkanSync::Destroy()
		{
			vkDestroySemaphore(*m_vkContext->GetVulkanDevice()->GetDevice(), m_imageAvailableSemaphore, nullptr);
			vkDestroySemaphore(*m_vkContext->GetVulkanDevice()->GetDevice(), m_renderFinishedSemaphore, nullptr);
		}
	}
}