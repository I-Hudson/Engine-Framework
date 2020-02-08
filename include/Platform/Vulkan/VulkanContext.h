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
#include "Platform/Vulkan/Internal/VulkanCommand.h"
#include "Platform/Vulkan/Internal/VulkanSync.h"

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

			static VulkanContext& Get() { return *static_cast<VulkanContext*>(s_instance); }

			void RecreateSwapChain();

			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData);

			void SetCurrentImageIndex(uint32_t imageIndex);
			uint32_t GetCurrentImageIndex() { return m_currentImageIndex; }

			VulkanValidationLayers* GetVulkanValidation() { return &m_vkValidationLayers; }
			VulkanExtensions* GetVulkanExtensions() { return &m_vkExtensions; }
			VulkanDebug* GetVulkanDebug() { return &m_vkDebug; }
			VulkanDevice* GetVulkanDevice() { return &m_vkDevice; }
			VulkanQueue* GetVulkanQueue() { return &m_vkQueue; }
			VulkanSurface* GetVulkanSurface() { return &m_vkSurface; }
			VulkanSwapchain* GetVulkanSwapchain() { return &m_vkSwapchain; }
			VulkanCommand* GetVulkanCommand() { return &m_vkCommand; }
			VulkanSync* GetVulkanSync() { return &m_vkSync; }

		private:
			void SetWindowCallbacks();
			void CleanupSwapChain();
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
			VulkanCommand m_vkCommand;
			VulkanSync m_vkSync;

			uint32_t m_currentImageIndex;

			VkInstance m_instance;
			std::vector<VkExtensionProperties> m_extensions;
		};
	}
}