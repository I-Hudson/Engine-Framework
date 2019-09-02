#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

namespace Framework
{
	Shader::Shader(const std::string& a_shaderFile)
	{
		std::string sources = ReadFromFile(a_shaderFile);
		auto shaderSources = PreProcess(sources);
		Compile(shaderSources);
	}

	Shader::Shader(const std::string& a_vertexSrc, const std::string& a_fragSrc)
		: m_ID(0)
	{

	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	//BASE TYPES
	void Shader::UploadUniformBool(const std::string& a_name, const bool& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniform1i(loc, a_value);
	}
	void Shader::UploadUniformInt(const std::string& a_name, const int& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniform1i(loc, a_value);
	}
	void Shader::UploadUniformFloat(const std::string& a_name, const float& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniform1f(loc, a_value);
	}

	//VECTOR
	void Shader::UploadUniformVec2(const std::string& a_name, const glm::vec2& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniform2fv(loc, 1, glm::value_ptr(a_value));
	}
	void Shader::UploadUniformVec3(const std::string& a_name, const glm::vec3& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniform3fv(loc, 1, glm::value_ptr(a_value));
	}
	void Shader::UploadUniformVec4(const std::string& a_name, const glm::vec4& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniform4fv(loc, 1, glm::value_ptr(a_value));
	}

	//MATRIX
	void Shader::UploadUniformMat2(const std::string& a_name, const glm::mat2& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(a_value));
	}
	void Shader::UploadUniformMat3(const std::string& a_name, const glm::mat3& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(a_value));
	}
	void Shader::UploadUniformMat4(const std::string& a_name, const glm::mat4& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(a_value));
	}

	static GLenum ShaderTypeFromString(const std::string& a_type)
	{
		if (a_type == "vertex") return GL_VERTEX_SHADER;
		if (a_type == "fragment") return GL_FRAGMENT_SHADER;

		return 0;
	}

	std::string Shader::ReadFromFile(const std::string& a_filePath)
	{
		std::string result;
		std::ifstream in(a_filePath, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		return result;
	}
	std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& a_source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = a_source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			//eol = end of line
			size_t eol = a_source.find_first_of("\r\n", pos);
			size_t begin = pos + typeTokenLength + 1;
			std::string type= a_source.substr(begin, eol - begin);

			size_t nextLinePos = a_source.find_first_not_of("\r\n", eol);
			pos = a_source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = a_source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? a_source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}
	void Shader::Compile(const std::unordered_map<GLenum, std::string>& a_shaderSources)
	{
		unsigned int program = glCreateProgram();
		std::vector<GLenum> shaderIDs;
		shaderIDs.reserve(a_shaderSources.size());

		for (auto& kv : a_shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			unsigned int shader = glCreateShader(type);
			
			const char* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			//Compile shader 
			glCompileShader(shader);

			//Check the shader has compiled correctly
			int success = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (success == GL_FALSE)
			{
				int maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());

				glDeleteShader(shader);

				printf("%s", infoLog.data());
				return;
			}
			glAttachShader(program, shader);
			shaderIDs.push_back(shader);
		}

		//Link program
		glLinkProgram(program);

		//Check program link
		unsigned int success = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)& success);
		if (success == GL_FALSE)
		{
			int maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

			glDeleteProgram(program);
			m_ID = 0;

			for (auto& shaderID : shaderIDs)
			{
				glDeleteShader(shaderID);
			}

			printf("%s", infoLog.data());
		}

		for (auto& shaderID : shaderIDs)
		{
			glDetachShader(program, shaderID);
			glDeleteShader(shaderID);
		}
		m_ID = program;
	}
}