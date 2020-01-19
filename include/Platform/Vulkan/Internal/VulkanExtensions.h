#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace Framework
{
	namespace Vulkan
	{
		class VulkanExtensions
		{
		public:
			VulkanExtensions();
			~VulkanExtensions();

			std::vector<const char*> GetRequiredExtensions(bool enableValidationLayers);
		};
	}
}