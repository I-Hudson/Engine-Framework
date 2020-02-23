#pragma once

#include "VulkanContext.h"

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
				VkDevice localDevice = device;
				if (localDevice == nullptr)
				{
					localDevice = *VulkanContext::Get().GetVulkanDevice()->GetDevice();
				}

				VkPhysicalDevice localPhysicalDevice = physicalDevice;
				if (localPhysicalDevice == nullptr)
				{
					localPhysicalDevice = *VulkanContext::Get().GetVulkanDevice()->GetPhyiscalDevice();
				}

				VkBufferCreateInfo bufferInfo = {};
				bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				bufferInfo.size = size;
				bufferInfo.usage = usage;
				bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				if (vkCreateBuffer(localDevice,
					&bufferInfo, nullptr, &buffer) != VK_SUCCESS)
				{
					EN_CORE_ERROR("Vulkan Vertex Buffer: Failed to create vertex buffer!");
				}

				VkMemoryRequirements memRequirements;
				vkGetBufferMemoryRequirements(localDevice, buffer, &memRequirements);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = FindMemoryType(localPhysicalDevice, memRequirements.memoryTypeBits, properties);
				if (vkAllocateMemory(localDevice,
					&allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
				{
					EN_CORE_ERROR("failed to allocate vertex buffer memory!");
				}

				vkBindBufferMemory(localDevice, buffer, bufferMemory, 0);
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
				VkCommandBuffer commandBuffer = CreateSingleTimeCommand();

				VkBufferCopy copyRegion = {};
				copyRegion.srcOffset = 0;
				copyRegion.dstOffset = 0;
				copyRegion.size = size;
				vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

				EndSingleTimeCommand(commandBuffer);
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
				VkDevice localDevice = device;
				if (localDevice == nullptr)
				{
					localDevice = *VulkanContext::Get().GetVulkanDevice()->GetDevice();
				}

				VkPhysicalDevice localPhysicalDevice = physicalDevice;
				if (localPhysicalDevice == nullptr)
				{
					localPhysicalDevice = *VulkanContext::Get().GetVulkanDevice()->GetPhyiscalDevice();
				}

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

				if (vkCreateImage(localDevice, &imageInfo, nullptr, &image) != VK_SUCCESS) {
					throw std::runtime_error("failed to create image!");
				}

				VkMemoryRequirements memRequirements;
				vkGetImageMemoryRequirements(localDevice, image, &memRequirements);

				VkMemoryAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = FindMemoryType(localPhysicalDevice, memRequirements.memoryTypeBits, properties);

				if (vkAllocateMemory(localDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
					throw std::runtime_error("failed to allocate image memory!");
				}

				vkBindImageMemory(localDevice, image, imageMemory, 0);
			}
		
			static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device)
			{
				SwapChainSupportDetails details;

				vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, *VulkanContext::Get().GetVulkanSurface()->GetSurface(),
					&details.capabilities);

				uint32_t formatCount;
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, *VulkanContext::Get().GetVulkanSurface()->GetSurface(),
					&formatCount, nullptr);
				if (formatCount != 0)
				{
					details.formats.resize(formatCount);
					vkGetPhysicalDeviceSurfaceFormatsKHR(device, *VulkanContext::Get().GetVulkanSurface()->GetSurface(),
						&formatCount, details.formats.data());
				}

				uint32_t presentCount;
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, *VulkanContext::Get().GetVulkanSurface()->GetSurface(),
					&presentCount, nullptr);
				if (presentCount != 0)
				{
					details.presentModes.resize(presentCount);
					vkGetPhysicalDeviceSurfacePresentModesKHR(device, *VulkanContext::Get().GetVulkanSurface()->GetSurface(),
						&presentCount, details.presentModes.data());
				}

				return details;
			}

			static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
			{
				for (const auto& availableFormat : availableFormats)
				{
					if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
					{
						return availableFormat;
					}
				}

				return availableFormats[0];
			}

			static VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
			{
				for (VkFormat format : candidates)
				{
					VkFormatProperties properties;
					vkGetPhysicalDeviceFormatProperties(*VulkanContext::Get().GetVulkanDevice()->GetPhyiscalDevice(), format, &properties);

					if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features)
					{
						return format;
					}
					else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features)
					{
						return format;
					}
				}
				EN_CORE_ERROR("Vulkan SwapChain: Failed to find supported format!");
			}

			static VkFormat FindDepthFormat()
			{
				return FindSupportedFormat(
					{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
					VK_IMAGE_TILING_OPTIMAL,
					VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
				);
			}

			static VkRenderPass CreateRenderPass()
			{
				SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(*VulkanContext::Get().GetVulkanDevice()->GetPhyiscalDevice());
				VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
				VkRenderPass renderPass;

				auto sur = VulkanContext::Get().GetVulkanSurface()->GetSurface();
				VkAttachmentDescription colourAttachment = {};
colourAttachment.format = surfaceFormat.format;
colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

VkAttachmentReference colorAttachmentRef = {};
colorAttachmentRef.attachment = 0;
colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

VkAttachmentDescription depthAttachment = {};
depthAttachment.format = FindDepthFormat();
depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

VkAttachmentReference depthAttachmentRef = {};
depthAttachmentRef.attachment = 1;
depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

VkSubpassDescription subpass = {};
subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
subpass.colorAttachmentCount = 1;
subpass.pColorAttachments = &colorAttachmentRef;
subpass.pDepthStencilAttachment = &depthAttachmentRef;

VkSubpassDependency dependency = {};
dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
dependency.dstSubpass = 0;
dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
dependency.srcAccessMask = 0;
dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

std::array<VkAttachmentDescription, 2> attachments = { colourAttachment, depthAttachment };

VkRenderPassCreateInfo renderPassInfo = {};
renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
renderPassInfo.pAttachments = attachments.data();
renderPassInfo.subpassCount = 1;
renderPassInfo.pSubpasses = &subpass;
renderPassInfo.dependencyCount = 1;
renderPassInfo.pDependencies = &dependency;

if (vkCreateRenderPass(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
{
	EN_CORE_ERROR("Vulkan Shader: Render pass was not created!");
}
return renderPass;
			}

			static VkCommandBuffer CreateSingleTimeCommand()
			{
				VkCommandBufferAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandPool = VulkanContext::Get().GetContextCommandPool();
				allocInfo.commandBufferCount = 1;

				VkCommandBuffer commandBuffer;
				if (vkAllocateCommandBuffers(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS)
				{
					EN_CORE_ERROR("VulkanUtils: SingleTimeCommand buffer not created!");
				}

				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

				vkBeginCommandBuffer(commandBuffer, &beginInfo);

				return commandBuffer;
			}

			static void EndSingleTimeCommand(VkCommandBuffer commandBuffer)
			{
				vkEndCommandBuffer(commandBuffer);

				VkSubmitInfo submitInfo = {};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = 1;
				submitInfo.pCommandBuffers = &commandBuffer;

				if (vkQueueSubmit(*VulkanContext::Get().GetVulkanQueue()->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
				{
					EN_CORE_ERROR("VulkanUtils: EndSingleTimeCommand queue was not submitted!");
				}
				if (vkQueueWaitIdle(*VulkanContext::Get().GetVulkanQueue()->GetGraphicsQueue()) != VK_SUCCESS)
				{
					EN_CORE_ERROR("VulkanUtils: EndSingleTimeCommand queue did not wait for idle!");
				}

				vkFreeCommandBuffers(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), VulkanContext::Get().GetContextCommandPool(), 1, &commandBuffer);
			}
		};
	}
}