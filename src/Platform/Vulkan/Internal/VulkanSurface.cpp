#include "Platform/Vulkan/VulkanContext.h"

namespace Framework
{
	namespace Vulkan
	{
		VulkanSurface::VulkanSurface()
		{
		}

		VulkanSurface::~VulkanSurface()
		{
		}

		void VulkanSurface::CreateSurface(VkInstance instance, VulkanContext* vulkanContext)
		{
			m_vkContext = vulkanContext;
			if (glfwCreateWindowSurface(instance, static_cast<GLFWwindow*>(m_vkContext->GetWindow()), nullptr, &m_surface) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan: Failed to create a widow surface!");
			}
		}

		void VulkanSurface::Destroy(VkInstance instance)
		{
			vkDestroySurfaceKHR(instance, m_surface, nullptr);
		}
	}
}


