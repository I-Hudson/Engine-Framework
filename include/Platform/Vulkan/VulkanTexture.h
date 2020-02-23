#pragma once

#include "Renderer/Texture.h"

#include "Platform/Vulkan/VulkanContext.h"

namespace Framework
{
	namespace Vulkan
	{
		class VulkanTexture : public Renderer::Texture
		{
		public:
			VulkanTexture(const std::string& a_name, const std::string& a_textureFile, const Renderer::TextureType& type);
			virtual ~VulkanTexture() override;

			virtual void Bind() override;
			virtual void Unbind() override;

			virtual void Load(const std::string& a_filePath, const Framework::Renderer::TextureType& type) override;
			virtual void Release() override;
			virtual const std::string& GetName() override;

		private:
			void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
			void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		private:
			std::string m_name;

			VkImage m_texture;
			VkDeviceMemory m_textureMemory;
		};
	}
}