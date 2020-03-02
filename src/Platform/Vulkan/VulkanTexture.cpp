#include "Platform/Vulkan/VulkanTexture.h"

#include "Platform/Vulkan/VulkanUtils.h"

#include <stbi/stb_image.h>

namespace Framework
{
	namespace Vulkan
	{
		VulkanTexture::VulkanTexture(const std::string& a_name, const std::string& a_textureFile, const Renderer::TextureType& type)
			: m_name(a_name), m_texture(VK_NULL_HANDLE), m_textureMemory(VK_NULL_HANDLE)
		{
			Load(a_textureFile, type);
		}

		VulkanTexture::~VulkanTexture()
		{
		}

		void VulkanTexture::Bind()
		{
		}

		void VulkanTexture::Unbind()
		{
		}

		void VulkanTexture::Load(const std::string& a_filePath, const Framework::Renderer::TextureType& type)
		{
			int width, height, nrChannels;
			unsigned char* pixels = nullptr;
			pixels = stbi_load(a_filePath.c_str(), &width, &height, &nrChannels, 0);
			if (pixels != nullptr)
			{
				VkBuffer stagingBuffer;
				VkDeviceMemory stagingMemory;
				int bufferSize = width * height;
			
				VkFormat imageFormat = VK_FORMAT_MAX_ENUM;
				switch (nrChannels)
				{
				case 1:
					imageFormat = VK_FORMAT_R8_SRGB;
					break;
				case 2:
					imageFormat = VK_FORMAT_R8G8_SRGB;
					bufferSize *= 2;
					break;
				case 3:
					imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
					bufferSize *= 4;
					break;
				case 4:
					imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
					bufferSize *= 4;
					break;

				default:
					imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
					bufferSize *= 4;
					break;
				}

				VulkanUtils::CreateBuffer(nullptr, nullptr, bufferSize, 
					VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

				void* data;
				vkMapMemory(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), stagingMemory, 0, bufferSize, 0, &data);
				memcpy(data, pixels, bufferSize);
				vkUnmapMemory(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), stagingMemory);



				VulkanUtils::CreateImage(nullptr, nullptr, width, height, 
					imageFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_texture, m_textureMemory);

				TransitionImageLayout(m_texture, imageFormat, VK_IMAGE_LAYOUT_UNDEFINED,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
				CopyBufferToImage(stagingBuffer, m_texture, width, height);
				TransitionImageLayout(m_texture, imageFormat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

				vkDestroyBuffer(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), stagingBuffer, nullptr);
				vkFreeMemory(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), stagingMemory, nullptr);
				stbi_image_free(pixels);
			}
		}

		void VulkanTexture::Release()
		{
			if (m_texture != VK_NULL_HANDLE)
			{
				vkDestroyImage(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), m_texture, nullptr);
				vkFreeMemory(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), m_textureMemory, nullptr);
			}
		}

		const std::string& VulkanTexture::GetName()
		{
			return m_name;
		}

		void VulkanTexture::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
		{
			VkCommandBuffer commandBuffer = VulkanUtils::CreateSingleTimeCommand();

			VkImageMemoryBarrier barrier = {};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = oldLayout;
			barrier.newLayout = newLayout;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			barrier.image = image;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;

			VkPipelineStageFlags sourceStage;
			VkPipelineStageFlags destinationStage;
			if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				barrier.srcAccessMask = 0;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}
			else
			{
				EN_CORE_ERROR("VulkanTexture: TransitionImageLayout access masks not set!");
			}

			vkCmdPipelineBarrier(commandBuffer, 
				sourceStage, destinationStage, 0, 
				0, nullptr, 
				0, nullptr, 
				1, &barrier);

			VulkanUtils::EndSingleTimeCommand(commandBuffer);
		}

		void VulkanTexture::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
		{
			VkCommandBuffer commandBuffer = VulkanUtils::CreateSingleTimeCommand();

			VkBufferImageCopy region = {};
			region.bufferOffset = 0;
			region.bufferRowLength = 0;
			region.bufferImageHeight = 0;

			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel = 0;
			region.imageSubresource.baseArrayLayer = 0;
			region.imageSubresource.layerCount = 1;

			region.imageOffset = { 0, 0, 0 };
			region.imageExtent = { width, height, 1 };

			vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

			VulkanUtils::EndSingleTimeCommand(commandBuffer);
		}
	}
}