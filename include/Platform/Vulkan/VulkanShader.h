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

			virtual void OnEvent(Event& event) override;

			virtual const unsigned int& GetProgramId() override { return m_ID; }

			virtual void Release() override;

		protected:
			virtual uint32_t ShaderTypeFromString(const std::string& a_type) override;

		private:
			void Compile(const std::unordered_map<unsigned int, std::string>& a_shaderSources);
			VkShaderModule CreateShaderModule(const std::string& byteData);
			void CreateShaderFromCachedSources();
			std::array<VkVertexInputAttributeDescription, 4> SetAttributeDescriptions(VkPipelineVertexInputStateCreateInfo* info);

			VkPipelineViewportStateCreateInfo CreateViewport(VkViewport viewport, VkRect2D scissor);
			VkPipelineRasterizationStateCreateInfo CreateRasterizer();
			VkPipelineMultisampleStateCreateInfo CreateMultismaple();
			VkPipelineColorBlendAttachmentState CreateColourBlendAttch();
			VkPipelineColorBlendStateCreateInfo CreateColourBlendState(VkPipelineColorBlendAttachmentState colourBlendAttch);

			void CreateGraphicsPipeline(VkPipelineShaderStageCreateInfo shaderStages[], VkPipelineVertexInputStateCreateInfo vertexInputInfo, VkPipelineInputAssemblyStateCreateInfo inputAssembly,
										VkPipelineViewportStateCreateInfo viewportState, VkPipelineRasterizationStateCreateInfo rasterizer, VkPipelineMultisampleStateCreateInfo multisampleState, 
										VkPipelineColorBlendStateCreateInfo colourBlendState);
		
		private:
			VulkanContext* m_vulkanContext;

			VkPipeline m_graphicsPipeline;
			VkPipelineLayout m_pipelineLayout;

			std::unordered_map<unsigned int, std::string> m_shaderSources;

			uint32_t m_ID;
			std::string m_name;
		};
	}
}
