#include "Material.h"

#include "Application.h"

namespace Framework
{
	Material::Material()
	{
		SetShader(Application::Get().GetShaderLibrary().GetShader("DefaultShader"));
	}

	Material::~Material()
	{
	}

	void Material::SetShader(std::shared_ptr<Shader> shader)
	{
		m_shader = shader;
	}

	void Material::SetBool(const std::string& name, const bool& value)
	{
		m_uniforms[name].Type = ShaderDataType::Bool;
		m_uniforms[name].Bool = value;
	}

	void Material::SetInt(const std::string& name, const int& value)
	{
		m_uniforms[name].Type = ShaderDataType::Bool;
		m_uniforms[name].Int = value;
	}

	void Material::SetFloat(const std::string& name, const float& value)
	{
		m_uniforms[name].Type = ShaderDataType::Float;
		m_uniforms[name].Float = value;
	}

	void Material::SetVec2(const std::string& name, const glm::vec2& value)
	{
		m_uniforms[name].Type = ShaderDataType::Float2;
		m_uniforms[name].Vec2 = value;
	}

	void Material::SetVec3(const std::string& name, const glm::vec3& value)
	{
		m_uniforms[name].Type = ShaderDataType::Float3;
		m_uniforms[name].Vec3 = value;
	}

	void Material::SetVec4(const std::string& name, const glm::vec4& value)
	{
		m_uniforms[name].Type = ShaderDataType::Float4;
		m_uniforms[name].Vec4 = value;
	}

	void Material::SetMat2(const std::string& name, const glm::mat2& value)
	{
		m_uniforms[name].Type = ShaderDataType::Mat3;
		m_uniforms[name].Mat2 = value;
	}

	void Material::SetMat3(const std::string& name, const glm::mat3& value)
	{
		m_uniforms[name].Type = ShaderDataType::Mat3;
		m_uniforms[name].Mat3 = value;
	}

	void Material::SetMat4(const std::string& name, const glm::mat4& value)
	{
		m_uniforms[name].Type = ShaderDataType::Mat4;
		m_uniforms[name].Mat4 = value;
	}

	void Material::SetTexture(const std::string& name, const std::shared_ptr<Texture> a_texture, const uint8_t& a_textureUint)
	{
		m_uniformsTexture[name].Type = ShaderDataType::Texture;
		m_uniformsTexture[name].TextureUnit = a_textureUint;
		m_uniformsTexture[name].Texture = a_texture;
	}

	void Material::SetUniforms()
	{
		for (auto it : m_uniforms)
		{
			switch (it.second.Type)
			{
			case ShaderDataType::Bool:
				m_shader->UploadUniformBool(it.first, it.second.Bool);
					break;
			case ShaderDataType::Int:
				m_shader->UploadUniformInt	(it.first, it.second.Int);
				break;
			case ShaderDataType::Float:
				m_shader->UploadUniformFloat(it.first, it.second.Float);
				break;

			case ShaderDataType::Float2:
				break;
			case ShaderDataType::Float3:
				break;
			case ShaderDataType::Float4:
				break;

			case ShaderDataType::Mat2:
				break;
			case ShaderDataType::Mat3:
				break;
			case ShaderDataType::Mat4:
				break;

			default:
				break;
			}
		}
	}


	bool Material::VerifyShader()
	{
		return m_shader != nullptr;
	}
}