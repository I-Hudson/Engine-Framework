#include "Platform\Vulkan\Internal\VulkanSwapchain.h"

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

		void VulkanSwapchain::SetupSwapChain(VulkanContext* vulkanContext)
		{
			m_vulkanContext = vulkanContext;
		}

		void VulkanSwapchain::Destroy()
		{
			for (auto framebuffer : m_swapChainFramebuffers)
			{
				vkDestroyFramebuffer(*m_vulkanContext->GetVulkanDevice()->GetDevice(), framebuffer, nullptr);
			}

			vkDestroyRenderPass(*m_vulkanContext->GetVulkanDevice()->GetDevice(), m_renderPass, nullptr);

			for (auto imageView : m_swapChainImageViews)
			{
				vkDestroyImageView(*m_vulkanContext->GetVulkanDevice()->GetDevice(), imageView, nullptr);
			}

			vkDestroySwapchainKHR(*m_vulkanContext->GetVulkanDevice()->GetDevice(), m_swapChain, nullptr);
		}

		void VulkanSwapchain::CreateSwapchain()
		{
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(*m_vulkanContext->GetVulkanDevice()->GetPhyiscalDevice());

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
			createInfo.surface = *m_vulkanContext->GetVulkanSurface()->GetSurface();
			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // use VK_IMAGE_USAGE_TRANSFER_DST_BIT for derfered 

			QueueFamilyIndices indices = m_vulkanContext->GetVulkanQueue()->FindQueueFamilies(*m_vulkanContext->GetVulkanDevice()->GetPhyiscalDevice());
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

			if (vkCreateSwapchainKHR(*m_vulkanContext->GetVulkanDevice()->GetDevice(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan: Swap chain was not created!");
			}

			vkGetSwapchainImagesKHR(*m_vulkanContext->GetVulkanDevice()->GetDevice(), m_swapChain, &imageCount, nullptr);
			m_swapChainImages.resize(imageCount);
			vkGetSwapchainImagesKHR(*m_vulkanContext->GetVulkanDevice()->GetDevice(), m_swapChain, &imageCount, m_swapChainImages.data());

			m_swapChainImageFormat = surfaceFormat.format;
			m_swapChainExtent = extent;
		}

		void VulkanSwapchain::CreateImageViews()
		{
			m_swapChainImageViews.resize(m_swapChainImages.size());

			for (size_t i = 0; i < m_swapChainImages.size(); ++i)
			{
				VkImageViewCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				createInfo.image = m_swapChainImages[i];
				createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				createInfo.format = m_swapChainImageFormat;

				createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

				createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				createInfo.subresourceRange.baseMipLevel = 0;
				createInfo.subresourceRange.levelCount = 1;
				createInfo.subresourceRange.baseArrayLayer = 0;
				createInfo.subresourceRange.layerCount = 1;

				if (vkCreateImageView(*m_vulkanContext->GetVulkanDevice()->GetDevice(), &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
				{
					EN_CORE_ERROR("Vulkan: Failed to create image view!");
				}
			}
		}

		void VulkanSwapchain::CreateRenderPass()
		{
			VkAttachmentDescription colourAttachment = {};
			colourAttachment.format = *m_vulkanContext->GetVulkanSwapchain()->GetSwapChainFormat();
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

			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			VkSubpassDependency dependency = {};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			VkRenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = &colourAttachment;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;
			renderPassInfo.dependencyCount = 1;
			renderPassInfo.pDependencies = &dependency;

			if (vkCreateRenderPass(*m_vulkanContext->GetVulkanDevice()->GetDevice(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan Shader: Render pass was not created!");
			}
		}

		void VulkanSwapchain::CreateFrameBuffers()
		{
			auto swapChainImageViews = *m_vulkanContext->GetVulkanSwapchain()->GetImageViews();
			m_swapChainFramebuffers.resize(swapChainImageViews.size());

			for (size_t i = 0; i < swapChainImageViews.size(); i++)
			{
				VkImageView attachments[] =
				{
					swapChainImageViews[i]
				};

				VkFramebufferCreateInfo framebufferInfo = {};
				framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebufferInfo.renderPass = m_renderPass;
				framebufferInfo.attachmentCount = 1;
				framebufferInfo.pAttachments = attachments;
				framebufferInfo.width = m_vulkanContext->GetVulkanSwapchain()->GetSwapChainExtent()->width;
				framebufferInfo.height = m_vulkanContext->GetVulkanSwapchain()->GetSwapChainExtent()->height;
				framebufferInfo.layers = 1;

				if (vkCreateFramebuffer(*m_vulkanContext->GetVulkanDevice()->GetDevice(), &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS)
				{
					EN_CORE_ERROR("Vulkan Shader: Failed to create framebuffer!");
				}
			}
		}

		SwapChainSupportDetails VulkanSwapchain::QuerySwapChainSupport(VkPhysicalDevice device)
		{
			SwapChainSupportDetails details;

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, *m_vulkanContext->GetVulkanSurface()->GetSurface(),
														&details.capabilities);

			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, *m_vulkanContext->GetVulkanSurface()->GetSurface(),
													&formatCount, nullptr);
			if (formatCount != 0)
			{
				details.formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, *m_vulkanContext->GetVulkanSurface()->GetSurface(),
														&formatCount, details.formats.data());
			}

			uint32_t presentCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, *m_vulkanContext->GetVulkanSurface()->GetSurface(),
														&presentCount, nullptr);
			if (presentCount != 0)
			{
				details.presentModes.resize(presentCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, *m_vulkanContext->GetVulkanSurface()->GetSurface(),
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
				glfwGetWindowSize(static_cast<GLFWwindow*>(m_vulkanContext->GetWindow()), &width, &height);
				VkExtent2D actualExtent = { width, height };

				actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
				actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

				return actualExtent;
			}
		}
	}
}