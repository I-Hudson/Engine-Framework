#include "Platform\Vulkan\Internal\VulkanSwapchain.h"

#include "Platform/Vulkan/VulkanUtils.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace Framework
{
	namespace Vulkan
	{
		VulkanSwapchain::VulkanSwapchain()
		{
		}

		VulkanSwapchain::~VulkanSwapchain()
		{
		}

		void VulkanSwapchain::Destroy()
		{
			for (auto framebuffer : m_swapChainFramebuffers)
			{
				vkDestroyFramebuffer(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), framebuffer, nullptr);
			}

			vkDestroyRenderPass(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), m_renderPass, nullptr);

			for (auto imageView : m_swapChainImageViews)
			{
				vkDestroyImageView(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), imageView, nullptr);
			}
			vkDestroyImageView(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), m_depthImageView, nullptr);
			vkDestroyImage(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), m_depthImage, nullptr);
			vkFreeMemory(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), m_depthImageMemory, nullptr);

			vkDestroySwapchainKHR(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), m_swapChain, nullptr);
		}

		void VulkanSwapchain::CreateSwapchain()
		{
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(*VulkanContext::Get().GetVulkanDevice()->GetPhyiscalDevice());

			VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
			VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
			VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

			uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
			if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
			{
				imageCount = swapChainSupport.capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = *VulkanContext::Get().GetVulkanSurface()->GetSurface();
			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // use VK_IMAGE_USAGE_TRANSFER_DST_BIT for derfered 

			QueueFamilyIndices indices = VulkanContext::Get().GetVulkanQueue()->FindQueueFamilies(*VulkanContext::Get().GetVulkanDevice()->GetPhyiscalDevice());
			uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

			if (indices.GraphicsFamily != indices.PresentFamily)
			{
				createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				createInfo.queueFamilyIndexCount = 2;
				createInfo.pQueueFamilyIndices = queueFamilyIndices;
			}
			else
			{
				createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				createInfo.queueFamilyIndexCount = 0;
				createInfo.pQueueFamilyIndices = nullptr;
			}

			createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // don't have transparency on this window.
			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE; // if another window is in front, clip the pixels.
			createInfo.oldSwapchain = VK_NULL_HANDLE;

			if (vkCreateSwapchainKHR(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan: Swap chain was not created!");
			}

			vkGetSwapchainImagesKHR(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), m_swapChain, &imageCount, nullptr);
			m_swapChainImages.resize(imageCount);
			vkGetSwapchainImagesKHR(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), m_swapChain, &imageCount, m_swapChainImages.data());

			m_swapChainImageFormat = surfaceFormat.format;
			m_swapChainExtent = extent;
		}

		void VulkanSwapchain::CreateImageViews()
		{
			m_swapChainImageViews.resize(m_swapChainImages.size());

			for (size_t i = 0; i < m_swapChainImages.size(); ++i)
			{
				m_swapChainImageViews[i] = VulkanUtils::CreateImageView(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), m_swapChainImages[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
			}
		}

		void VulkanSwapchain::CreateRenderPass()
		{
			VkAttachmentDescription colourAttachment = {};
			colourAttachment.format = *VulkanContext::Get().GetVulkanSwapchain()->GetSwapChainFormat();
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

			std::array<VkAttachmentDescription, 2> attachments = { colourAttachment, depthAttachment};

			VkRenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			renderPassInfo.pAttachments = attachments.data();
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;
			renderPassInfo.dependencyCount = 1;
			renderPassInfo.pDependencies = &dependency;

			if (vkCreateRenderPass(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan Shader: Render pass was not created!");
			}
		}

		void VulkanSwapchain::CreateFrameBuffers()
		{
			auto swapChainImageViews = *VulkanContext::Get().GetVulkanSwapchain()->GetImageViews();
			m_swapChainFramebuffers.resize(swapChainImageViews.size());

			for (size_t i = 0; i < swapChainImageViews.size(); i++)
			{
				std::array<VkImageView, 2> attachments[] =
				{
					swapChainImageViews[i],
					m_depthImageView
				};

				VkFramebufferCreateInfo framebufferInfo = {};
				framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebufferInfo.renderPass = m_renderPass;
				framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments->size());
				framebufferInfo.pAttachments = attachments->data();
				framebufferInfo.width = VulkanContext::Get().GetVulkanSwapchain()->GetSwapChainExtent()->width;
				framebufferInfo.height = VulkanContext::Get().GetVulkanSwapchain()->GetSwapChainExtent()->height;
				framebufferInfo.layers = 1;

				if (vkCreateFramebuffer(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS)
				{
					EN_CORE_ERROR("Vulkan Shader: Failed to create framebuffer!");
				}
			}
		}

		void VulkanSwapchain::CreateDepthResources()
		{
			VkFormat depthFormat = FindDepthFormat();

			VulkanUtils::CreateImage(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), *VulkanContext::Get().GetVulkanDevice()->GetPhyiscalDevice(),
				m_swapChainExtent.width, m_swapChainExtent.height, depthFormat,
				VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				m_depthImage, m_depthImageMemory);
			m_depthImageView = VulkanUtils::CreateImageView(*VulkanContext::Get().GetVulkanDevice()->GetDevice(), m_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
		}

		SwapChainSupportDetails VulkanSwapchain::QuerySwapChainSupport(VkPhysicalDevice device)
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

		VkSurfaceFormatKHR VulkanSwapchain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
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

		VkPresentModeKHR VulkanSwapchain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
		{
			for (const auto& availablePresentMode : availablePresentModes) 
			{
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
				{
					return availablePresentMode;
				}
			}

			return VK_PRESENT_MODE_FIFO_KHR;
		}

		VkExtent2D VulkanSwapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
		{
			if (capabilities.currentExtent.width != UINT32_MAX)
			{
				return capabilities.currentExtent;
			}
			else
			{
				int width, height;
				glfwGetWindowSize(static_cast<GLFWwindow*>(VulkanContext::Get().GetWindow()), &width, &height);
				VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

				actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
				actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

				return actualExtent;
			}
		}

		VkFormat VulkanSwapchain::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
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

		VkFormat VulkanSwapchain::FindDepthFormat()
		{
			return FindSupportedFormat(
				{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
				VK_IMAGE_TILING_OPTIMAL,
				VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
			);
		}

		bool VulkanSwapchain::HasStencilComponent(VkFormat format)
		{
			return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
		}
	}
}