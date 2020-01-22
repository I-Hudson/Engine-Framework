#pragma once

#include "Renderer/Shader.h"
#include "Platform/Vulkan/VulkanContext.h"


namespace Framework
{
	namespace Vulkan
	{
		class VulkanShader : public Renderer::Shader
		{
		public:
			VulkanShader(const std::string& a_shaderFile);
			VulkanShader(const std::string& a_name, const std::string& a_vertexSrc, const std::string& a_fragSrc);
			~VulkanShader() override;

			//Bind and unbind the shader
			void Bind() const override;
			void Unbind() const override;

			virtual const std::string& GetName() override;

			//Upload uniforms to the GPU

			//BASE TYPES
			void UploadUniformBool(const std::string& a_name, const bool& a_value);
			void UploadUniformInt(const std::string& a_name, const int& a_value);
			void UploadUniformFloat(const std::string& a_name, const float& a_value);
			//void UploadUniformString(const std::string& a_name, const std::string& a_value);

			//VECTOR
			void UploadUniformVec2(const std::string& a_name, const glm::vec2& a_value);
			void UploadUniformVec3(const std::string& a_name, const glm::vec3& a_value);
			void UploadUniformVec4(const std::string& a_name, const glm::vec4& a_value);

			//MATRIX														   
			void UploadUniformMat2(const std::string& a_name, const glm::mat2& a_value);
			void UploadUniformMat3(const std::string& a_name, const glm::mat3& a_value);
			void UploadUniformMat4(const std::string& a_name, const glm::mat4& a_value);

			void UploadTexture(const std::string& a_name, const std::shared_ptr<Renderer::Texture> a_texture, const uint8_t& a_textureUint = 0) override;

			virtual const unsigned int& GetProgramId() override { return m_ID; }

			virtual void Release() override;

		protected:
			virtual uint32_t ShaderTypeFromString(const std::string& a_type) override;

		private:
			void Compile(const std::unordered_map<GLenum, std::string>& a_shaderSources);
			VkShaderModule CreateShaderModule(const std::string& byteData);

			VkPipelineViewportStateCreateInfo CreateViewport(VkViewport viewport, VkRect2D scissor);
			VkPipelineRasterizationStateCreateInfo CreateRasterizer();
			VkPipelineMultisampleStateCreateInfo CreateMultismaple();
			VkPipelineColorBlendAttachmentState CreateColourBlendAttch();
			VkPipelineColorBlendStateCreateInfo CreateColourBlendState(VkPipelineColorBlendAttachmentState colourBlendAttch);

			void CreateRenderPass();
			void CreateGraphicsPipeline(VkPipelineShaderStageCreateInfo shaderStages[], VkPipelineVertexInputStateCreateInfo vertexInputInfo, VkPipelineInputAssemblyStateCreateInfo inputAssembly,
										VkPipelineViewportStateCreateInfo viewportState, VkPipelineRasterizationStateCreateInfo rasterizer, VkPipelineMultisampleStateCreateInfo multisampleState, 
										VkPipelineColorBlendStateCreateInfo colourBlendState);
			void CreateFramebuffers();

			VulkanContext* m_vulkanContext;

			VkPipeline m_graphicsPipeline;
			//@TODO: Move the render pass object to VulkanContext not the shader. 
			// this object would live in a "GBuffer" class. Has not need for shaders, describes framebuffers
			// properties, how many color bits/depth, samples.
			VkRenderPass m_renderPass;
			VkPipelineLayout m_pipelineLayout;

			std::vector<VkFramebuffer> m_swapChainFramebuffers;

			uint32_t m_ID;
			std::string m_name;
		};
	}
}
