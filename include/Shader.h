#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Framework
{
	class Shader
	{
	public:

		Shader(const std::string& a_shaderFile);
		Shader(const std::string& a_vertexSrc, const std::string& a_fragSrc);
		~Shader();

		//Bind and unbind the shader
		void Bind() const;
		void Unbind() const;

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

	private:
		std::string ReadFromFile(const std::string& a_filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& a_source);
		void Compile(const std::unordered_map<GLenum, std::string>& a_shaderSources);

		uint32_t m_ID;
	};
}