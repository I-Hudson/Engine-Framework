#pragma once

#include <string>
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>

#include "Renderer/Texture.h"
#include "Renderer/Buffer.h"

namespace Framework
{
	namespace Renderer
	{
		class Shader
		{
		public:
			virtual ~Shader() = default;

			//Bind and unbind the shader
			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			//BASE TYPES
			virtual void UploadUniformBool(const std::string& a_name, const bool& a_value) = 0;
			virtual void UploadUniformInt(const std::string& a_name, const int& a_value) = 0;
			virtual void UploadUniformFloat(const std::string& a_name, const float& a_value) = 0;
			//void UploadUniformString(const std::string& a_name, const std::string& a_value);

			//VECTOR
			virtual void UploadUniformVec2(const std::string& a_name, const glm::vec2& a_value) = 0;
			virtual void UploadUniformVec3(const std::string& a_name, const glm::vec3& a_value) = 0;
			virtual void UploadUniformVec4(const std::string& a_name, const glm::vec4& a_value) = 0;

			//MATRIX														   
			virtual void UploadUniformMat2(const std::string& a_name, const glm::mat2& a_value) = 0;
			virtual void UploadUniformMat3(const std::string& a_name, const glm::mat3& a_value) = 0;
			virtual void UploadUniformMat4(const std::string& a_name, const glm::mat4& a_value) = 0;

			virtual void UploadTexture(const std::string& a_name, const std::shared_ptr<Texture> a_texture, const uint8_t& a_textureUint = 0) = 0;

			virtual const std::string& GetName() = 0;
			virtual const unsigned int& GetProgramId() = 0;

			virtual void Release() = 0;

			static std::shared_ptr<Shader> Create(const std::string& a_filePath);
			static std::shared_ptr<Shader> Create(const std::string& a_name, const std::string& a_vertexSource, const std::string& a_fragSource);
		
		protected:
			virtual std::string ReadFromFile(const std::string& a_filePath);
			virtual std::unordered_map<GLenum, std::string> PreProcess(const std::string& a_source);
			virtual uint32_t ShaderTypeFromString(const std::string& a_type) = 0;
		};

		class ShaderLibrary
		{
		public:
			void Add(const std::string& a_name, const std::shared_ptr<Shader>& a_shader);
			void Add(const std::shared_ptr<Shader>& a_shader);

			std::shared_ptr<Shader> Load(const std::string& a_filePath);
			std::shared_ptr<Shader> Load(const std::string& a_name, const std::string& a_filePath);
			std::shared_ptr<Shader> Load(const std::string& a_name, const std::string& a_filePathVertex, const std::string& a_filePathFrag);

			std::shared_ptr<Shader> GetShader(const std::string& a_name);

			void ReleaseAll();

			bool Exists(const std::string& a_name);
		private:
			std::map<std::string, std::shared_ptr<Shader>> m_shaders;
		};
	}
}