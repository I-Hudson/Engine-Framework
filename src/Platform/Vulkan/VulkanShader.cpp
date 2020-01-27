#include "Platform/Vulkan/VulkanShader.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Application.h"

namespace Framework
{
	namespace Vulkan
	{
		VulkanShader::VulkanShader(const std::string& a_shaderFile)
			: m_ID(0)
		{
			m_vulkanContext = static_cast<VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext());
			if (m_vulkanContext == nullptr)
			{
				EN_CORE_ERROR("Vulkan Shader: Context could not be otained!");
				return;
			}

			std::string sources = ReadFromFile(a_shaderFile);
			auto shaderSources = PreProcess(sources);
			Compile(shaderSources);

			//resources/shaders/demoShader.glsl
			auto lastSlash = a_shaderFile.find_last_of("/\\");
			lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
			auto lastDot = a_shaderFile.rfind('.');
			auto count = lastDot == std::string::npos ? a_shaderFile.size() - lastSlash : lastDot - lastSlash;
			m_name = a_shaderFile.substr(lastSlash, count);
		}

		VulkanShader::VulkanShader(const std::string& a_name, const std::string& a_vertexSrc, const std::string& a_fragSrc)
			: m_ID(0)
		{
			m_vulkanContext = static_cast<VulkanContext*>(Application::Get().GetWindow()->GetGraphicsContext());
			if (m_vulkanContext == nullptr)
			{
				EN_CORE_ERROR("Vulkan Shader: Context could not be otained!");
				return;
			}

			std::unordered_map<GLenum, std::string> sources;
			sources[VK_SHADER_STAGE_VERTEX_BIT] = ReadFromFile(a_vertexSrc);
			sources[VK_SHADER_STAGE_FRAGMENT_BIT] = ReadFromFile(a_fragSrc);
			Compile(sources);

			m_name = a_name;
		}

		VulkanShader::~VulkanShader()
		{
		}

		void VulkanShader::Bind() const
		{
			auto commandBuffers = *m_vulkanContext->GetVulkanCommand()->GetCommandBuffers();
			for (size_t i = 0; i < commandBuffers.size(); ++i)
			{
				vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);
			}
		}

		void VulkanShader::Unbind() const
		{
		}

		const std::string& VulkanShader::GetName()
		{
			return m_name;
		}

		void VulkanShader::UploadUniformBool(const std::string& a_name, const bool& a_value)
		{
		}

		void VulkanShader::UploadUniformInt(const std::string& a_name, const int& a_value)
		{
		}

		void VulkanShader::UploadUniformFloat(const std::string& a_name, const float& a_value)
		{
		}

		void VulkanShader::UploadUniformVec2(const std::string& a_name, const glm::vec2& a_value)
		{
		}

		void VulkanShader::UploadUniformVec3(const std::string& a_name, const glm::vec3& a_value)
		{
		}

		void VulkanShader::UploadUniformVec4(const std::string& a_name, const glm::vec4& a_value)
		{
		}

		void VulkanShader::UploadUniformMat2(const std::string& a_name, const glm::mat2& a_value)
		{
		}

		void VulkanShader::UploadUniformMat3(const std::string& a_name, const glm::mat3& a_value)
		{
		}

		void VulkanShader::UploadUniformMat4(const std::string& a_name, const glm::mat4& a_value)
		{
		}

		void VulkanShader::UploadTexture(const std::string& a_name, const std::shared_ptr<Renderer::Texture> a_texture, const uint8_t& a_textureUint)
		{
		}

		void VulkanShader::Release()
		{
			vkDestroyPipeline(*m_vulkanContext->GetVulkanDevice()->GetDevice(), m_graphicsPipeline, nullptr);
			vkDestroyPipelineLayout(*m_vulkanContext->GetVulkanDevice()->GetDevice(), m_pipelineLayout, nullptr);
		}

		uint32_t VulkanShader::ShaderTypeFromString(const std::string& a_type)
		{
			if (a_type == "Vert") return VK_SHADER_STAGE_VERTEX_BIT;
			if (a_type == "Frag") return VK_SHADER_STAGE_FRAGMENT_BIT;

			return 0;
		}

		void VulkanShader::Compile(const std::unordered_map<GLenum, std::string>& a_shaderSources)
		{
			std::map<uint32_t, VkShaderModule> shaders;

			for (auto it : a_shaderSources)
			{
				shaders[it.first] = CreateShaderModule(it.second);
			}

			VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
			vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertShaderStageInfo.module = shaders[VK_SHADER_STAGE_VERTEX_BIT];
			vertShaderStageInfo.pName = "main";

			VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
			fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragShaderStageInfo.module = shaders[VK_SHADER_STAGE_FRAGMENT_BIT];;
			fragShaderStageInfo.pName = "main";

			VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

			// Goes into vertex array for vulkan
			VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputInfo.vertexBindingDescriptionCount = 0;
			vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
			vertexInputInfo.vertexAttributeDescriptionCount = 0;
			vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

			VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
			inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssembly.primitiveRestartEnable = VK_FALSE;

			VkViewport viewport = {};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float)m_vulkanContext->GetVulkanSwapchain()->GetSwapChainExtent()->width;
			viewport.height = (float)m_vulkanContext->GetVulkanSwapchain()->GetSwapChainExtent()->height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor = {};
			scissor.offset = { 0, 0 };
			scissor.extent = *m_vulkanContext->GetVulkanSwapchain()->GetSwapChainExtent();

			VkPipelineViewportStateCreateInfo viewportState = CreateViewport(viewport, scissor);
			VkPipelineRasterizationStateCreateInfo rasterizer = CreateRasterizer();
			VkPipelineMultisampleStateCreateInfo multisampling = CreateMultismaple();

			VkPipelineColorBlendAttachmentState colorBlendAttachment = CreateColourBlendAttch();

			VkPipelineColorBlendStateCreateInfo colorBlending = CreateColourBlendState(colorBlendAttachment);

			VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 0; // Optional
			pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
			pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
			pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

			if (vkCreatePipelineLayout(*m_vulkanContext->GetVulkanDevice()->GetDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
				throw std::runtime_error("failed to create pipeline layout!");
			}

			CreateGraphicsPipeline(shaderStages, vertexInputInfo, inputAssembly, viewportState, rasterizer, multisampling, colorBlending);

			for (auto it : shaders)
			{
				vkDestroyShaderModule(*m_vulkanContext->GetVulkanDevice()->GetDevice(), it.second, nullptr);
			}
		}

		VkShaderModule VulkanShader::CreateShaderModule(const std::string& byteData)
		{
			std::vector<char> byteCodes;
			int index = 0;
			for (auto it : byteData)
			{
				byteCodes.push_back(it);
			}

			VkShaderModuleCreateInfo moduleCreateInfo = {};
			moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleCreateInfo.codeSize = byteCodes.size();
			moduleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(byteCodes.data());

			VkShaderModule shaderModule;
			if (vkCreateShaderModule(*m_vulkanContext->GetVulkanDevice()->GetDevice(), &moduleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan Shader: Shader Module was not created!");
			}

			return shaderModule;
		}

		VkPipelineViewportStateCreateInfo VulkanShader::CreateViewport(VkViewport viewport, VkRect2D scissor)
		{
			VkPipelineViewportStateCreateInfo viewportState = {};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;

			return viewportState;
		}

		VkPipelineRasterizationStateCreateInfo VulkanShader::CreateRasterizer()
		{
			VkPipelineRasterizationStateCreateInfo rasterizer = {};
			rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable = VK_FALSE;

			rasterizer.rasterizerDiscardEnable = VK_FALSE;

			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;

			rasterizer.lineWidth = 1.0f;

			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

			rasterizer.depthBiasEnable = VK_FALSE;
			rasterizer.depthBiasConstantFactor = 0.0f; // Optional
			rasterizer.depthBiasClamp = 0.0f; // Optional
			rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

			return rasterizer;
		}

		VkPipelineMultisampleStateCreateInfo VulkanShader::CreateMultismaple()
		{
			VkPipelineMultisampleStateCreateInfo multisampling = {};
			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			multisampling.minSampleShading = 1.0f; // Optional
			multisampling.pSampleMask = nullptr; // Optional
			multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
			multisampling.alphaToOneEnable = VK_FALSE; // Optional

			return multisampling;
		}

		VkPipelineColorBlendAttachmentState VulkanShader::CreateColourBlendAttch()
		{
			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_FALSE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

			return colorBlendAttachment;
		}

		VkPipelineColorBlendStateCreateInfo VulkanShader::CreateColourBlendState(VkPipelineColorBlendAttachmentState colourBlendAttch)
		{
			VkPipelineColorBlendStateCreateInfo colorBlending = {};
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colourBlendAttch;
			colorBlending.blendConstants[0] = 0.0f; // Optional
			colorBlending.blendConstants[1] = 0.0f; // Optional
			colorBlending.blendConstants[2] = 0.0f; // Optional
			colorBlending.blendConstants[3] = 0.0f; // Optional

			return colorBlending;
		}

		void VulkanShader::CreateGraphicsPipeline(VkPipelineShaderStageCreateInfo shaderStages[], VkPipelineVertexInputStateCreateInfo vertexInputInfo,
			VkPipelineInputAssemblyStateCreateInfo inputAssembly, VkPipelineViewportStateCreateInfo viewportState,
			VkPipelineRasterizationStateCreateInfo rasterizer, VkPipelineMultisampleStateCreateInfo multisampleState,
			VkPipelineColorBlendStateCreateInfo colourBlendState)
		{
			VkGraphicsPipelineCreateInfo pipelineInfo = {};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shaderStages;
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState = &multisampleState;
			pipelineInfo.pDepthStencilState = nullptr; // Optional
			pipelineInfo.pColorBlendState = &colourBlendState;
			pipelineInfo.pDynamicState = nullptr; // Optional

			pipelineInfo.layout = m_pipelineLayout;
			pipelineInfo.renderPass = *m_vulkanContext->GetVulkanSwapchain()->GetRenderPass();
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
			pipelineInfo.basePipelineIndex = -1; // Optional

			if (vkCreateGraphicsPipelines(*m_vulkanContext->GetVulkanDevice()->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS)
			{
				EN_CORE_ERROR("Vulkan Shader: Graphics pipeline was not created!");
			}
		}
	}
}