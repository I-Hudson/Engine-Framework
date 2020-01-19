#include "Platform/Vulkan/VulkanContext.h"

namespace Framework
{
	namespace Vulkan
	{
		VulkanContext::VulkanContext(const int& a_width, const int& a_height, const std::string& a_title, const bool& a_fullscreen, Window::WindowData* a_windowData)
			: m_window(nullptr)
		{
			Init(a_width, a_height, a_title, a_fullscreen, nullptr, a_windowData);
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
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			m_window = glfwCreateWindow(a_width, a_height, a_title.c_str(), nullptr, nullptr);

			CreateInstance(a_title);

			if (m_vkDebug.SetupDebugger(m_instance, nullptr) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan: Debugger not setup Correctly!!");
			}

			m_vkQueue.SetContext(this);

			m_vkSurface.CreateSurface(m_instance, this);

			m_vkSwapchain.SetupSwapChain(this);

			m_vkDevice.Setup(m_instance, this, { VK_KHR_SWAPCHAIN_EXTENSION_NAME });

			m_vkSwapchain.CreateSwapchain();

			m_vkPipeline.Setup(this);
		}

		void VulkanContext::Destroy()
		{
			m_vkSwapchain.Destroy();

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
	}
}
