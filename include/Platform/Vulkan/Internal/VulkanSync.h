#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace Framework
{
	namespace Vulkan
	{
		class VulkanContext;

		// Create a struct

		struct VulkanSyncSemaphore
		{
			VkSemaphore Semaphore;
			void* VKObject;
		};

		class VulkanSync
		{
		public:
			VulkanSync();
			~VulkanSync();

			void Init(const int& maxFramesInFlight);
			void Free();

			VulkanSyncSemaphore* GetSyncSemaphore(void* vkObject);

			std::vector<VkSemaphore>* GetImageSemaphores() { return &m_imageAvailableSemaphores; }
			std::vector<VkSemaphore>* GetRenderSemaphores() { return &m_renderFinishedSemaphores; }
			std::vector<VkFence>* GetInFlightFences() { return &m_inFlightFences; }
			std::vector<VkFence>* GetInFlightImages() { return &m_imagesInFlight; }

			VkSemaphore* GetCurrentImageSemaphore() { return &m_imageAvailableSemaphores[m_currentFrame]; }
			VkSemaphore* GetCurrentRenderemaphore() { return &m_renderFinishedSemaphores[m_currentFrame]; }
			VkFence* GetCurrentInFlightFence() { return &m_inFlightFences[m_currentFrame]; }
			VkFence* GetCurrentInFlightImage() { return &m_imagesInFlight[m_currentFrame]; }

			int GetMaxFramesInFlight() const { return m_maxFramesInFlight; }
			int GetCurrentFrame() const { return m_currentFrame; }
			void IncermentCurrentFrame();

		private:
			std::vector<VkSemaphore> m_imageAvailableSemaphores;
			std::vector<VkSemaphore> m_renderFinishedSemaphores;
			std::vector<VkFence> m_inFlightFences;
			std::vector<VkFence> m_imagesInFlight;

			int m_maxFramesInFlight;
			int m_currentFrame;
		};
	}
}