#include "Platform/Vulkan/Internal/VulkanValidationLayers.h"

namespace Framework
{
	namespace Vulkan
	{
		VulkanValidationLayers::VulkanValidationLayers()
		{
		}

		VulkanValidationLayers::~VulkanValidationLayers()
		{
		}

		bool VulkanValidationLayers::CheckValidationLayerSupport(const std::vector<const char*> layers)
		{
			m_validationLayers = layers;

			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			for (const char* layerName : layers)
			{
				bool layerFound = false;

				for (const auto& layerProperties : availableLayers)
				{
					if (strcmp(layerName, layerProperties.layerName) == 0)
					{
						layerFound = true;
						break;
					}
				}

				if (!layerFound)
				{
					return false;
				}
			}

			return true;
		}
	}
}