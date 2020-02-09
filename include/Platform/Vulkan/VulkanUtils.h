#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Log.h"

namespace Framework
{
	namespace Vulkan
	{
		class VulkanUtils
		{
		public:

			static void CreateBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage,
										VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
			{
				VkBufferCreateInfo bufferInfo = {};
				bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				bufferInfo.size = size;
				bufferInfo.usage = usage;
				bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				if (vkCreateBuffer(device,
					&bufferInfo, nullptr, &buffer) != VK_SUCCESS)
				{
					EN_CORE_ERROR("Vulkan Vertex Buffer: Failed to create vertex buffer!");
				}

				VkMemoryRequirements memRequirements;
				vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);
				if (vkAllocateMemory(device,
					&allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
				{
					EN_CORE_ERROR("failed to allocate vertex buffer memory!");
				}

				vkBindBufferMemory(device, buffer, bufferMemory, 0);
			}

			static uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
			{
				VkPhysicalDeviceMemoryProperties memProperties;
				vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

				for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
				{
					if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
					{
						return i;
					}
				}

				EN_CORE_ERROR("Vulkan Vertex Buffer: Failed to find suitable memory type!");

				return 0;
			}

			static void CopyBuffer(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
			{
				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandPool = commandPool;
				allocInfo.commandBufferCount = 1;

				VkCommandBuffer commandBuffer;
				vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

				vkBeginCommandBuffer(commandBuffer, &beginInfo);

				VkBufferCopy copyRegion = {};
				copyRegion.srcOffset = 0;
				copyRegion.dstOffset = 0;
				copyRegion.size = size;
				vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

				vkEndCommandBuffer(commandBuffer);

				VkSubmitInfo submitInfo = {};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = 1;
				submitInfo.pCommandBuffers = &commandBuffer;

				vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
				vkQueueWaitIdle(queue);

				vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
			}

			static VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
			{
				VkImageViewCreateInfo viewInfo = {};
				viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				viewInfo.image = image;
				viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				viewInfo.format = format;
				viewInfo.subresourceRange.aspectMask = aspectFlags;
				viewInfo.subresourceRange.baseMipLevel = 0;
				viewInfo.subresourceRange.levelCount = 1;
				viewInfo.subresourceRange.baseArrayLayer = 0;
				viewInfo.subresourceRange.layerCount = 1;

				VkImageView imageView;
				if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
					throw std::runtime_error("Vulkan Utils: Failed to create texture image view!");
				}

				return imageView;
			}

			static void CreateImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
			{
				VkImageCreateInfo imageInfo = {};
				imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				imageInfo.imageType = VK_IMAGE_TYPE_2D;
				imageInfo.extent.width = width;
				imageInfo.extent.height = height;
				imageInfo.extent.depth = 1;
				imageInfo.mipLevels = 1;
				imageInfo.arrayLayers = 1;
				imageInfo.format = format;
				imageInfo.tiling = tiling;
				imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				imageInfo.usage = usage;
				imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
				imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
					throw std::runtime_error("failed to create image!");
				}

				VkMemoryRequirements memRequirements;
				vkGetImageMemoryRequirements(device, image, &memRequirements);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

				if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
					throw std::runtime_error("failed to allocate image memory!");
				}

				vkBindImageMemory(device, image, imageMemory, 0);
			}
		};
	}
}