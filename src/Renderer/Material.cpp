#include "Renderer/Material.h"
#include "Application.h"

#include "Platform/OpenGL/OpenGLMaterial.h"
#include "Platform/Vulkan/VulkanMaterial.h"

namespace Framework
{
	namespace Renderer
	{
		Material::Material()
		{
		}

		Material::~Material()
		{
		}

		void Material::Release()
		{
			m_uniforms.clear();
			m_uniformsTexture.clear();

			OnRelease();
		}

		void Material::SetShader(std::shared_ptr<Shader> shader)
		{
			m_shader = shader;

			OnSetShader();
			//ProcessShaderUniforms();
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

		void Material::SetUniforms(GBuffer* gBuffer)
		{
			for (auto it : m_uniforms)
			{
				switch (it.second.Type)
				{
				case ShaderDataType::Bool:
					m_shader->UploadUniformBool(it.first, it.second.Bool);
					break;
				case ShaderDataType::Int:
					m_shader->UploadUniformInt(it.first, it.second.Int);
					break;
				case ShaderDataType::Float:
					m_shader->UploadUniformFloat(it.first, it.second.Float);
					break;

				case ShaderDataType::Float2:
					m_shader->UploadUniformVec2(it.first, it.second.Vec2);
					break;
				case ShaderDataType::Float3:
					m_shader->UploadUniformVec3(it.first, it.second.Vec3);
					break;
				case ShaderDataType::Float4:
					m_shader->UploadUniformVec4(it.first, it.second.Vec4);
					break;

				case ShaderDataType::Mat2:
					m_shader->UploadUniformMat2(it.first, it.second.Mat2);
					break;
				case ShaderDataType::Mat3:
					m_shader->UploadUniformMat3(it.first, it.second.Mat3);
					break;
				case ShaderDataType::Mat4:
					m_shader->UploadUniformMat4(it.first, it.second.Mat4);
					break;

				default:
					break;
				}
			}

			for (auto it : m_uniformsTexture)
			{
				if (it.second.Texture != nullptr)
				{
					m_shader->UploadTexture(it.first, it.second.Texture, it.second.TextureUnit);
				}
			}
		}

		Material* Material::Create()
		{
			switch (Renderer::GetAPI())
			{
			case RendererAPI::API::OpenGL:
				return new OpenGLMaterial();
			case RendererAPI::API::Vulkan:
				return new Vulkan::VulkanMaterial();
			}
			EN_CORE_ERROR("No material exists for {0}", Renderer::GetAPI());
			return nullptr;
		}


		bool Material::VerifyShader()
		{
			return m_shader != nullptr;
		}
	}
}