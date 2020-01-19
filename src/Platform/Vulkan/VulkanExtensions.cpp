#include "Platform/Vulkan/Internal/VulkanExtensions.h"


namespace Framework
{
	namespace Vulkan
	{
		VulkanExtensions::VulkanExtensions()
		{
		}

		VulkanExtensions::~VulkanExtensions()
		{
		}

		std::vector<const char*> VulkanExtensions::GetRequiredExtensions(bool enableValidationLayers)
		{
			uint32_t glfwExtenstionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtenstionCount);

			std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtenstionCount);

			if (enableValidationLayers)
			{
				extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}

			return extensions;
		}
	}
}