#include "Platform/Vulkan/VulkanContext.h"
#include <Events\ApplicationEvent.h>

namespace Framework
{
	namespace Vulkan
	{
		VulkanContext::VulkanContext()
			: m_window(nullptr)
		{
		}

		VulkanContext::~VulkanContext()
		{
		}

		void VulkanContext::Init(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen, void* a_window, Window::WindowData* a_windowData)
		{
			if (!glfwInit())
			{
				EN_CORE_FATEL("GLFW did not init.");
			}

			glfwSetErrorCallback([](int code,  const char* name)
			{
				EN_CORE_ERROR("Error code: {0}, Name: {1}.", code, name);
			});

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			m_window = glfwCreateWindow(a_width, a_height, a_title.c_str(), nullptr, nullptr);
			//glfwMakeContextCurrent(m_window);
			glfwSetWindowUserPointer(m_window, a_windowData);

			SetWindowCallbacks();

			CreateInstance(a_title);

			if (m_vkDebug.SetupDebugger(m_instance, nullptr) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan: Debugger not setup Correctly!!");
			}

			m_vkQueue.SetContext(this);

			m_vkSurface.CreateSurface(m_instance, this);

			m_vkDevice.Setup(m_instance, this, { VK_KHR_SWAPCHAIN_EXTENSION_NAME });

			m_vkSwapchain.CreateSwapchain();

			m_vkSwapchain.CreateImageViews();

			m_vkSwapchain.CreateRenderPass();

			m_vkSwapchain.CreateDepthResources();

			m_vkSwapchain.CreateFrameBuffers();

			VulkanCommand::CreateCommandPool(&m_commandPool);
			VulkanCommand::CreateCommandBuffers(3, &m_commandBuffers, &m_commandPool);

			m_vkSync.Init(3);
		}

		void VulkanContext::Destroy()
		{
			vkDeviceWaitIdle(*m_vkDevice.GetDevice());

			CleanupSwapChain();

			m_vkSync.Free();

			VulkanCommand::FreeCommandPool(&m_commandPool);

			if (m_vkValidationLayers.GetValidationLayersState())
			{
				m_vkDebug.Destroy(m_instance);
			}

			m_vkDevice.Destroy();

			m_vkSurface.Destroy(m_instance);
			vkDestroyInstance(m_instance, nullptr);

			glfwDestroyWindow(m_window);
			glfwTerminate();
		}

		void VulkanContext::SwapBuffers()
		{
			glfwPollEvents();
		}

		void* VulkanContext::GetNativeContext()
		{
			return this;
		}

		void VulkanContext::SetWindowCallbacks()
		{
			glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
				{
					Window::WindowData& data = *(Window::WindowData*)glfwGetWindowUserPointer(window);

					data.Width = width;
					data.Height = height;

					WindowResizeEvent event(width, height);
					data.EventCallback(event);
				});
		}

		void VulkanContext::CleanupSwapChain()
		{
			VulkanCommand::FreeCommandBuffers(&m_commandBuffers, &m_commandPool);

			m_vkSwapchain.Destroy();
		}

		void VulkanContext::RecreateSwapChain()
		{
			vkDeviceWaitIdle(*m_vkDevice.GetDevice());

			CleanupSwapChain();

			m_vkSwapchain.CreateSwapchain();

			m_vkSwapchain.CreateImageViews();

			m_vkSwapchain.CreateRenderPass();

			Window::WindowData& data = *(Window::WindowData*)glfwGetWindowUserPointer(m_window);
			VulkanRecreateShaders recreateShadersEvent;
			data.EventCallback(recreateShadersEvent);
			
			m_vkSwapchain.CreateDepthResources();
			m_vkSwapchain.CreateFrameBuffers();

			VulkanCommand::CreateCommandBuffers(3, &m_commandBuffers, &m_commandPool);
		}

		void VulkanContext::CreateInstance(const std::string& title)
		{
			m_vkValidationLayers.SetValidationLayersState(true);
			if (!m_vkValidationLayers.CheckValidationLayerSupport({ "VK_LAYER_KHRONOS_validation" }))
			{
				EN_CORE_ERROR("Vulkan: Validation layers request, but not available!");
			}

			VkApplicationInfo appInfo = {};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = title.c_str();
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "No Engine";
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			auto extensions = m_vkExtensions.GetRequiredExtensions(m_vkValidationLayers.GetValidationLayersState());
			createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
			createInfo.ppEnabledExtensionNames = extensions.data();

			if (m_vkValidationLayers.GetValidationLayersState())
			{
				createInfo.enabledLayerCount = m_vkValidationLayers.GetValidationLayerCount();
				createInfo.ppEnabledLayerNames = m_vkValidationLayers.GetValidationLayers()->data();
			
				createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &m_vkDebug.CreateDebugMessengerInfo();
			}
			else
			{
				createInfo.enabledLayerCount = 0;

				createInfo.pNext = nullptr;
			}

			if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan: Failed to create instance!");
			}
		}

		VKAPI_ATTR VkBool32 VKAPI_CALL VulkanContext::DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData) {

			if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			{
				std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
			}

			return VK_FALSE;
		}

		void VulkanContext::SetCurrentImageIndex(uint32_t imageIndex)
		{
			m_currentImageIndex = imageIndex;
		}
	}
}
