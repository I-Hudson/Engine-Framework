#include "VulkanDebug.h"

namespace Framework
{
	namespace Vulkan
	{
		VKAPI_ATTR VkBool32 VKAPI_CALL Framework::Vulkan::VulkanDebug::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
			VkDebugUtilsMessageTypeFlagsEXT messageType, 
			const VkDebugUtilsMessengerCallbackDataEXT* 
			pCallbackData, void* 
			pUserData)
		{
		}
	}
}
