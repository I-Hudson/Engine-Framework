#pragma once

#include "Renderer/Shader.h"
#include "Platform/Vulkan/VulkanContext.h"

#include <memory>
#include <functional>
#include <algorithm>

namespace Framework
{
	namespace Vulkan
	{
		struct UniformVertexObject
		{
			glm::mat4 u_ProjectionView;
			glm::mat4 u_ObjectMatrix;
		};

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
			virtual void UploadUniformBool(const std::string& a_name, const bool& a_value) override;
			virtual void UploadUniformInt(const std::string& a_name, const int& a_value) override;
			virtual void UploadUniformFloat(const std::string& a_name, const float& a_value) override;

			//VECTOR
			virtual void UploadUniformVec2(const std::string& a_name, const glm::vec2& a_value) override;
			virtual void UploadUniformVec3(const std::string& a_name, const glm::vec3& a_value) override;
			virtual void UploadUniformVec4(const std::string& a_name, const glm::vec4& a_value) override;

			//MATRIX														   
			virtual void UploadUniformMat2(const std::string& a_name, const glm::mat2& a_value) override;
			virtual void UploadUniformMat3(const std::string& a_name, const glm::mat3& a_value) override;
			virtual void UploadUniformMat4(const std::string& a_name, const glm::mat4& a_value) override;

			void UploadTexture(const std::string& a_name, const std::shared_ptr<Renderer::Texture> a_texture, const uint8_t& a_textureUint = 0) override;

			virtual void OnEvent(Event& event) override;

			virtual const unsigned int& GetProgramId() override { return m_ID; }

			virtual void Release() override;

			void SetSwapChainCallback(std::function<void()> function);

			VkDescriptorSetLayout GetDescriptorSetLayout() { return m_descriptorLayout; }
			VkPipelineLayout GetPipelineLayout() { return m_pipelineLayout; }

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
			VkDescriptorSetLayout m_descriptorLayout;
			VkPipelineLayout m_pipelineLayout;

			std::unordered_map<unsigned int, std::string> m_shaderSources;

			uint32_t m_ID;
			std::string m_name;
		
			std::function<void()> OnSwapChainRecreate;
		};
	}
}
