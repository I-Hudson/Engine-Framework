#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace Framework
{
	namespace Vulkan
	{
		class VulkanContext;

		class VulkanSync
		{
		public:
			VulkanSync();
			~VulkanSync();

			void Setup(VulkanContext* context);
			void Destroy();

			VkSemaphore* GetImageSemaphore() { return &m_imageAvailableSemaphore; }
			VkSemaphore* GetRenderSemaphore() { return &m_renderFinishedSemaphore; }

		private:
			VkSemaphore m_imageAvailableSemaphore;
			VkSemaphore m_renderFinishedSemaphore;

			VulkanContext* m_vkContext;
		};
	}
}