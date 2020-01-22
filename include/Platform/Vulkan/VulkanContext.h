#pragma once

#include "Renderer/GraphicsContext.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>

#include "Platform/Vulkan/Internal/VulkanValidationLayers.h"
#include "Platform/Vulkan/Internal/VulkanExtensions.h"
#include "Platform/Vulkan/Internal/VulkanDebug.h"
#include "Platform/Vulkan/Internal/VulkanDevice.h"
#include "Platform/Vulkan/Internal/VulkanQueue.h"
#include "Platform/Vulkan/Internal/VulkanSurface.h"
#include "Platform/Vulkan/Internal/VulkanSwapchain.h"
#include "Platform/Vulkan/Internal/VulkanPipeline.h"
#include "Platform/Vulkan/Internal/VulkanCommand.h"

#include "Log.h"

struct GLFWwindow;

namespace Framework
{
	namespace Vulkan
	{
		class VulkanContext : public Renderer::GraphicsContext
		{
		public:
			VulkanContext(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen = false, Window::WindowData* a_windowData = &Window::WindowData());
			~VulkanContext();

			virtual void Init(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen, void* a_window, Window::WindowData* a_windowData) override;
			virtual void Destroy() override;

			virtual void SwapBuffers() override;

			virtual void* GetNativeContext() override;
			virtual void* GetWindow() override { return m_window; }

			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData);

			VulkanValidationLayers* GetVulkanValidation() { return &m_vkValidationLayers; }
			VulkanExtensions* GetVulkanExtensions() { return &m_vkExtensions; }
			VulkanDebug* GetVulkanDebug() { return &m_vkDebug; }
			VulkanDevice* GetVulkanDevice() { return &m_vkDevice; }
			VulkanQueue* GetVulkanQueue() { return &m_vkQueue; }
			VulkanSurface* GetVulkanSurface() { return &m_vkSurface; }
			VulkanSwapchain* GetVulkanSwapchain() { return &m_vkSwapchain; }
			VulkanPipeline* GetVulkanPipeline() { return &m_vkPipeline; }
			VulkanCommand* GetVulkanCommand() { return &m_vkCommand; }

		private:
			void CreateInstance(const std::string& title);

		private:
			GLFWwindow* m_window;

			VulkanValidationLayers m_vkValidationLayers;
			VulkanExtensions m_vkExtensions;
			VulkanDebug m_vkDebug;
			VulkanDevice m_vkDevice;
			VulkanQueue m_vkQueue;
			VulkanSurface m_vkSurface;
			VulkanSwapchain m_vkSwapchain;
			VulkanPipeline m_vkPipeline;
			VulkanCommand m_vkCommand;

			VkInstance m_instance;
			std::vector<VkExtensionProperties> m_extensions;
		};
	}
}