#pragma once

#include "Renderer/Shader.h"

namespace Framework
{
	class OpenGLShader : public Renderer::Shader
	{
	public:
		OpenGLShader(const std::string& a_shaderFile);
		OpenGLShader(const std::string& a_name, const std::string& a_vertexSrc, const std::string& a_fragSrc);
		~OpenGLShader() override;

		//Bind and unbind the shader
		void Bind(Renderer::GBuffer* gBuffer) const override;
		void Unbind(Renderer::GBuffer* gBuffer) const override;

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

		uint32_t m_ID;
		std::string m_name;
	};
};
