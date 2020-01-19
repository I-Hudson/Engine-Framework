#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace Framework
{
	namespace Vulkan
	{
		class VulkanContext;

		class VulkanPipeline
		{
		public:
			VulkanPipeline();
			~VulkanPipeline();

			void Setup(VulkanContext* vulkanContext);

		private:
			VulkanContext* m_vulkanContext;
		};
	}
}