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

			SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

			VkSwapchainKHR* GetSwapChain() { return &m_swapChain; }
			std::vector<VkImage>* GetSwapChainImages() { return &m_swapChainImages; }
			VkFormat* GetSwapChainFormat() { return &m_swapChainImageFormat; }
			VkExtent2D* GetSwapChainExtent() { return &m_swapChainExtent; }

			std::vector<VkImageView>* GetImageViews() { return &m_swapChainImageViews; }

		private:
			VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector< VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		private:
			VulkanContext* m_vulkanContext;

			VkSwapchainKHR m_swapChain;
			std::vector<VkImage> m_swapChainImages;
			VkFormat m_swapChainImageFormat;
			VkExtent2D m_swapChainExtent;
			
			std::vector<VkImageView> m_swapChainImageViews;
		};
	}
}