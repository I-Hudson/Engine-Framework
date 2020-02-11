#include "Platform/Vulkan/VulkanMaterial.h"
#include "Platform/Vulkan/VulkanUtils.h"

#include <memory>
#include <functional>
#include <algorithm>

#include "Application.h"

namespace Framework
{
	namespace Vulkan
	{
		VulkanMaterial::VulkanMaterial()
		{
		}

		VulkanMaterial::~VulkanMaterial()
		{

		}

		void VulkanMaterial::OnRelease()
		{
			auto device = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice();
			for (size_t i = 0; i < m_uniformBuffers.size(); ++i)
			{
				vkDestroyBuffer(*device.GetDevice(), m_uniformBuffers[i], nullptr);
				vkFreeMemory(*device.GetDevice(), m_uniformBuffersMemory[i], nullptr);
			}
			vkDestroyDescriptorPool(*device.GetDevice(), m_descriptorPool, nullptr);
		}

		void VulkanMaterial::OnSetShader()
		{
			CreateUniformBuffers();
			static_cast<VulkanShader*>(&*GetShader())->SetSwapChainCallback([this]()
			{
				RecreateUniformBuffers();
			});
			CreateDescriptorPool();
			CreateDescriptorSets();
		}

		void VulkanMaterial::CreateDescriptorPool()
		{
			auto swapChain = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanSwapchain();

			VkDescriptorPoolSize poolSize = {};
			poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSize.descriptorCount = static_cast<uint32_t>(swapChain.GetSwapChainImages()->size());

			VkDescriptorPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = 1;
			poolInfo.pPoolSizes = &poolSize;
			poolInfo.maxSets = static_cast<uint32_t>(swapChain.GetSwapChainImages()->size());
		
			auto device = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice();
			
			if (vkCreateDescriptorPool(*device.GetDevice(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan Material: Failed to create Descriptor pool!");
			}
		}

		void VulkanMaterial::CreateDescriptorSets()
		{
			auto device = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice();
			auto swapChain = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanSwapchain();
			auto shader = static_cast<VulkanShader*>(&*GetShader());
			
			std::vector<VkDescriptorSetLayout> layouts(swapChain.GetSwapChainImages()->size(), shader->GetDescriptorSetLayout());
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = m_descriptorPool;
			allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChain.GetSwapChainImages()->size());
			allocInfo.pSetLayouts = layouts.data();

			m_descriptorSets.resize(swapChain.GetSwapChainImages()->size());
			if (vkAllocateDescriptorSets(*device.GetDevice(), &allocInfo, m_descriptorSets.data()) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan Material: Failed to allocate descriptor sets!");
			}

			for (size_t i = 0; i < m_descriptorSets.size(); ++i)
			{
				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = m_uniformBuffers[i];
				bufferInfo.offset = 0;
				bufferInfo.range = sizeof(UniformVertexObject);

				VkWriteDescriptorSet descriptorWrite = {};
				descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrite.dstSet = m_descriptorSets[i];
				descriptorWrite.dstBinding = 0;
				descriptorWrite.dstArrayElement = 0;

				descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				descriptorWrite.descriptorCount = 1;

				descriptorWrite.pBufferInfo = &bufferInfo;
				descriptorWrite.pImageInfo = nullptr; // Optional
				descriptorWrite.pTexelBufferView = nullptr; // Optional

				vkUpdateDescriptorSets(*device.GetDevice(), 1, &descriptorWrite, 0, nullptr);
			}
		}

		void VulkanMaterial::RecreateUniformBuffers()
		{
			auto device = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice();
			for (size_t i = 0; i < m_uniformBuffers.size(); ++i)
			{
				vkDestroyBuffer(*device.GetDevice(), m_uniformBuffers[i], nullptr);
				vkFreeMemory(*device.GetDevice(), m_uniformBuffersMemory[i], nullptr);
			}

			vkDestroyDescriptorPool(*device.GetDevice(), m_descriptorPool, nullptr);

			CreateUniformBuffers();
			CreateDescriptorPool();
			CreateDescriptorSets();
			
		}

		void VulkanMaterial::SetUniforms()
		{
			m_uvo.u_ObjectMatrix = m_uniforms["u_ObjectMatrix"].Mat4;
			m_uvo.u_Projection = m_uniforms["u_Projection"].Mat4;
			m_uvo.u_View = m_uniforms["u_View"].Mat4;
			m_uvo.u_AmbiantLight = m_uniforms["u_AmbiantLight"].Vec4;
			m_uvo.u_AmbiantInten = m_uniforms["u_AmbiantLightInten"].Float;
			m_uvo.u_DirLight = m_uniforms["u_DirLight"].Vec4;
			m_uvo.u_ViewPos = m_uniforms["u_ViewPos"].Vec4;

			m_uvo.u_Projection[1][1] *= -1;
			
			auto device = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice();
			auto context = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext());
			auto command = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanCommand();

			uint32_t currentImage = context.GetCurrentImageIndex();

			void* data;
			vkMapMemory(*device.GetDevice(), m_uniformBuffersMemory[currentImage], 0, sizeof(UniformVertexObject), 0, &data);
			memcpy(data, &m_uvo, sizeof(UniformVertexObject));
			vkUnmapMemory(*device.GetDevice(), m_uniformBuffersMemory[currentImage]);

			auto commandBuffers = *command.GetCommandBuffers();
			auto shader = static_cast<VulkanShader*>(&*GetShader());
			for (size_t i = 0; i < commandBuffers.size(); ++i)
			{
				vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipelineLayout(), 0, 1, &m_descriptorSets[i], 0, nullptr);
			}
		}

		void VulkanMaterial::CreateUniformBuffers()
		{
			VkDeviceSize bufferSize = sizeof(Vulkan::UniformVertexObject);

			auto swapChain = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanSwapchain();
			auto device = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanDevice();
			auto command = *static_cast<Vulkan::VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext())->GetVulkanCommand();

			m_uniformBuffers.resize(swapChain.GetSwapChainImages()->size());
			m_uniformBuffersMemory.resize(m_uniformBuffers.size());

			for (size_t i = 0; i < m_uniformBuffers.size(); ++i)
			{
				VulkanUtils::CreateBuffer(*device.GetDevice(), *device.GetPhyiscalDevice(), bufferSize,
					VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
					m_uniformBuffers[i], m_uniformBuffersMemory[i]);
			}
		}

		void VulkanMaterial::ProcessShaderUniforms()
		{
		}
	}
}