#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Framework
{
	namespace Vulkan
	{
		class VulkanContext;

		class VulkanSurface
		{
		public: 
			VulkanSurface();
			~VulkanSurface();

			void CreateSurface(VkInstance instance, VulkanContext* vulkanContext);
			void Destroy(VkInstance instance);

			VkSurfaceKHR* GetSurface() { return &m_surface; }

		private:
			VulkanContext* m_vkContext;

			VkSurfaceKHR m_surface;
		};
	}
}