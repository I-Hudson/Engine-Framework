#pragma once

#include "Renderer/Shader.h"
#include <map>

namespace Framework
{
	namespace Renderer
	{
		struct UniformValue
		{
			//type
			ShaderDataType Type;

			union
			{
				bool Bool;
				int Int;
				float Float;

				glm::vec2 Vec2;
				glm::vec3 Vec3;
				glm::vec4 Vec4;

				glm::mat2 Mat2;
				glm::mat3 Mat3;
				glm::mat4 Mat4;
			};

			UniformValue() {}
			~UniformValue() {}
		};

		struct UniformTextureValue
		{
			//type
			ShaderDataType Type;
			int TextureUnit;
			std::shared_ptr<Texture> Texture;

			UniformTextureValue() {}
			~UniformTextureValue() {}
		};

		class Material
		{
		public:
			Material();
			~Material();

			void Release();
			virtual void OnRelease() { }

			void SetShader(std::shared_ptr<Shader> shader);
			virtual void OnSetShader() { }

			void SetBool(const std::string& name, const bool& value);
			void SetInt(const std::string& name, const int& value);
			void SetFloat(const std::string& name, const float& value);

			void SetVec2(const std::string& name, const glm::vec2& value);
			void SetVec3(const std::string& name, const glm::vec3& value);
			void SetVec4(const std::string& name, const glm::vec4& value);

			void SetMat2(const std::string& name, const glm::mat2& value);
			void SetMat3(const std::string& name, const glm::mat3& value);
			void SetMat4(const std::string& name, const glm::mat4& value);

			void SetTexture(const std::string& name, const std::shared_ptr<Texture> a_texture, const uint8_t& a_textureUint = 0);

			virtual void SetUniforms(GBuffer* gBuffer);

			std::shared_ptr<Shader> GetShader() const { return m_shader; }
			std::map<std::string, UniformValue> GetUniforms() const { return m_uniforms; };

			static Material* Create();

		protected:
			virtual void ProcessShaderUniforms() = 0;

		private:
			bool VerifyShader();

		protected:
			std::shared_ptr<Shader> m_shader;
			std::map<std::string, UniformValue> m_uniforms;
			std::map<std::string, UniformTextureValue> m_uniformsTexture;
		};
	}
}