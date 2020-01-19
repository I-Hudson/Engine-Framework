#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Framework
{
	namespace Vulkan
	{
		class VulkanDebug
		{
		public:
			VulkanDebug();
			~VulkanDebug();

			VkResult SetupDebugger(VkInstance instance, const VkAllocationCallbacks* allocator);
			VkDebugUtilsMessengerCreateInfoEXT CreateDebugMessengerInfo();

			void Destroy(VkInstance instance);

			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData);

		private:
			VkDebugUtilsMessengerEXT m_debugMessager;
		};
	}
}