#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace Framework
{
	namespace Vulkan
	{
		class VulkanValidationLayers
		{
		public:
			VulkanValidationLayers();
			~VulkanValidationLayers();

			bool CheckValidationLayerSupport(const std::vector<const char*> layers);

			bool GetValidationLayersState() const {return m_enableValidationLayers;}
			void SetValidationLayersState(const bool& state) { m_enableValidationLayers = state; }

			uint32_t GetValidationLayerCount() const { return (uint32_t)m_validationLayers.size(); }
			const std::vector<const char*>* GetValidationLayers() { return &m_validationLayers; }

		private:
			std::vector<const char*> m_validationLayers;
			bool m_enableValidationLayers;
		};
	}
}