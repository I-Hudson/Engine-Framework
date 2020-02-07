#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Renderer/Material.h"
#include "Platform/Vulkan/VulkanShader.h"

namespace Framework
{
	namespace Vulkan
	{
		class VulkanMaterial : public Renderer::Material
		{
		public:
			VulkanMaterial();
			~VulkanMaterial();

			virtual void OnRelease() override;
			virtual void OnSetShader() override;

			void RecreateUniformBuffers();

			virtual void SetUniforms() override;

		protected:
			virtual void ProcessShaderUniforms() override;

		private:
			void CreateUniformBuffers();
			void CreateDescriptorPool();
			void CreateDescriptorSets();

		private:
			std::vector<VkBuffer> m_uniformBuffers;
			std::vector<VkDeviceMemory> m_uniformBuffersMemory;

			VkDescriptorPool m_descriptorPool;
			std::vector<VkDescriptorSet> m_descriptorSets;

			UniformVertexObject m_uvo;
		};
	}
}