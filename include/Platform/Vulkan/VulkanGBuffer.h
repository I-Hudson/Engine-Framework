#pragma once

#include "Renderer/GBuffer.h"

#include "Platform/Vulkan/Internal/VulkanSwapchain.h"
#include "Platform/Vulkan/Internal/VulkanCommand.h"
#include "Platform/Vulkan/Internal/VulkanSync.h"

namespace Framework
{
	namespace Vulkan
	{
		class VulkanGBuffer : public Renderer::GBuffer
		{
		public:
			VulkanGBuffer();
			~VulkanGBuffer();

			virtual void Init() override;
			virtual void Free() override;

			virtual void Bind() override;
			virtual void AddAttachment() override;

			VkCommandPool GetCommandPool() { return m_commandPool; }
			std::vector<VkCommandBuffer>* GetCommandBuffers() { return &m_commandBuffers; }
			VulkanSwapchain* GetSwapChain() { return &m_swapChain; }
			VulkanSync* GetSync() { return &m_sync; }

		private :

			VkCommandPool m_commandPool;
			std::vector<VkCommandBuffer> m_commandBuffers;
			VulkanSwapchain m_swapChain;
			VulkanSync m_sync;
		};
	}
}