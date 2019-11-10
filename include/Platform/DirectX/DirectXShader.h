#pragma once

#include "Platform/DirectX/DirectXContext.h"

#include "Shader.h"
#include "Application.h"

namespace Framework
{
	class DirectXShader : public Shader
	{
	public:
		DirectXShader(const std::string& a_shaderFile);
		DirectXShader(const std::string& a_name, const std::string& a_vertexSrc, const std::string& a_fragSrc);
		~DirectXShader() override;

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

		void UploadTexture(const std::string& a_name, const std::shared_ptr<Texture> a_texture) override;

		virtual void Release() override;

	private:
		std::string ReadFromFile(const std::string& a_filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& a_source);
		void Compile(const std::unordered_map<GLenum, std::string>& a_shaderSources);

		uint32_t m_textureIndex;
		uint32_t m_ID;
		std::string m_name;

		// Depth buffer.
		Microsoft::WRL::ComPtr<ID3D12Resource> m_depthBuffer;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DSVHeap;

		Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
	};
}
