#include "Platform/Vulkan/VulkanGBuffer.h"

namespace Framework
{
	namespace Vulkan
	{
		VulkanGBuffer::VulkanGBuffer()
		{
			Init();
		}

		VulkanGBuffer::~VulkanGBuffer()
		{
		}

		void VulkanGBuffer::Init()
		{
			m_swapChain.CreateSwapchain();
			m_swapChain.CreateImageViews();
			m_swapChain.CreateRenderPass();
			m_swapChain.CreateDepthResources();
			m_swapChain.CreateFrameBuffers();

			VulkanCommand::CreateCommandPool(&m_commandPool);
			m_commandBuffers.resize(3);
			VulkanCommand::CreateCommandBuffers(3, &m_commandBuffers, &m_commandPool);

			m_sync.Init(3);
		}

		void VulkanGBuffer::Free()
		{
			m_swapChain.Destroy();
			VulkanCommand::FreeCommandPool(&m_commandPool);
			m_sync.Free();
		}

		void VulkanGBuffer::Bind()
		{
		}

		void VulkanGBuffer::AddAttachment()
		{
		}
	}
}