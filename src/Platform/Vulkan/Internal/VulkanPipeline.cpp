#include "Platform\Vulkan\Internal\VulkanPipeline.h"

#include "Platform/Vulkan/VulkanContext.h"

namespace Framework
{
	namespace Vulkan
	{
		VulkanPipeline::VulkanPipeline()
		{
		}

		VulkanPipeline::~VulkanPipeline()
		{
		}

		void VulkanPipeline::Setup(VulkanContext* vulkanContext)
		{
			m_vulkanContext = vulkanContext;
		}
	}
}