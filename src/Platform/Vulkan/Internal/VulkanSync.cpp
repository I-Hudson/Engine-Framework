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

		void VulkanSync::Init(const int& maxFramesInFlight)
		{
			m_maxFramesInFlight = maxFramesInFlight;
			m_imageAvailableSemaphores.resize(maxFramesInFlight);
			m_renderFinishedSemaphores.resize(maxFramesInFlight);
			m_inFlightFences.resize(maxFramesInFlight);
			m_imagesInFlight.resize(maxFramesInFlight, VK_NULL_HANDLE);

			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			VkFenceCreateInfo fenceInfo = {};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			for (size_t i = 0; i < maxFramesInFlight; i++)
			{
				if (vkCreateSemaphore(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
					vkCreateSemaphore(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
					vkCreateFence(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS)
				{
					EN_CORE_ERROR("VulkanSync: vkCreateSemaphore failed!");
				}
			}
		}

		void VulkanSync::Free()
		{
			for (size_t i = 0; i < m_maxFramesInFlight; i++)
			{
				vkDestroySemaphore(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), m_imageAvailableSemaphores[i], nullptr);
				vkDestroySemaphore(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), m_renderFinishedSemaphores[i], nullptr);
				vkDestroyFence(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), m_inFlightFences[i], nullptr);
			}
		}

		VulkanSyncSemaphore* VulkanSync::GetSyncSemaphore(void* vkObject)
		{
			//for (size_t i = 0; i < length; i++)
			{
				VulkanSyncSemaphore v;
				return &v;
			}
		}

		void VulkanSync::IncermentCurrentFrame()
		{
			m_currentFrame = (1 + m_currentFrame) % m_maxFramesInFlight;
		}
	}
}