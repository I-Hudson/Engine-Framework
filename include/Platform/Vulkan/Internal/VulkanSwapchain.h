#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace Framework
{
	namespace Vulkan
	{
		class VulkanContext;

		struct SwapChainSupportDetails 
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		class VulkanSwapchain
		{
		public:
			VulkanSwapchain();
			~VulkanSwapchain();

			void SetupSwapChain(VulkanContext* vulkanContext);
			void Destroy();

			void CreateSwapchain();
			void CreateImageViews();
			void CreateRenderPass();
			void CreateFrameBuffers();
			void CreateDepthResources();

			SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

			VkSwapchainKHR* GetSwapChain() { return &m_swapChain; }
			std::vector<VkImage>* GetSwapChainImages() { return &m_swapChainImages; }
			VkFormat* GetSwapChainFormat() { return &m_swapChainImageFormat; }
			VkExtent2D* GetSwapChainExtent() { return &m_swapChainExtent; }
			VkRenderPass* GetRenderPass() { return &m_renderPass; }
			std::vector<VkFramebuffer>* GetSwapChainFrameBuffers() { return &m_swapChainFramebuffers; }

			std::vector<VkImageView>* GetImageViews() { return &m_swapChainImageViews; }

		private:
			VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector< VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

			VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
			VkFormat FindDepthFormat();
			bool HasStencilComponent(VkFormat format);


		private:
			VulkanContext* m_vulkanContext;

			VkSwapchainKHR m_swapChain;
			std::vector<VkImage> m_swapChainImages;
			VkFormat m_swapChainImageFormat;
			VkExtent2D m_swapChainExtent;

			VkImage m_depthImage;
			VkDeviceMemory m_depthImageMemory;
			VkImageView m_depthImageView;

			//@TODO: Move the render pass object and swapchain framebuffers to VulkanContext not the shader. 
			// this object would live in a "GBuffer" class. Has not need for shaders, describes framebuffers
			// properties, how many color bits/depth, samples.
			VkRenderPass m_renderPass;
			std::vector<VkFramebuffer> m_swapChainFramebuffers;

			std::vector<VkImageView> m_swapChainImageViews;
		};
	}
}