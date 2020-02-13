#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <array>

namespace Framework
{
	OpenGLShader::OpenGLShader(const std::string& a_shaderFile)
		: m_ID(0)
	{
		std::string sources = ReadFromFile(a_shaderFile);
		auto shaderSources = PreProcess(sources);
		Compile(shaderSources);

		//resources/shaders/demoShader.glsl
		auto lastSlash = a_shaderFile.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = a_shaderFile.rfind('.');
		auto count = lastDot == std::string::npos ? a_shaderFile.size() - lastSlash : lastDot - lastSlash;
		m_name = a_shaderFile.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& a_name, const std::string& a_vertexSrc, const std::string& a_fragSrc)
		: m_ID(0), m_name(a_name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = a_vertexSrc;
		sources[GL_FRAGMENT_SHADER] = a_fragSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ID);
	}

	void OpenGLShader::Bind(Renderer::GBuffer* gBuffer) const
	{
		glUseProgram(m_ID);
	}

	void OpenGLShader::Unbind(Renderer::GBuffer* gBuffer) const
	{
		glUseProgram(0);
	}

	const std::string& OpenGLShader::GetName()
	{
		return m_name;
	}

	//BASE TYPES
	void OpenGLShader::UploadUniformBool(const std::string& a_name, const bool& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniform1i(loc, a_value);
	}
	void OpenGLShader::UploadUniformInt(const std::string& a_name, const int& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniform1i(loc, a_value);
	}
	void OpenGLShader::UploadUniformFloat(const std::string& a_name, const float& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniform1f(loc, a_value);
	}

	//VECTOR
	void OpenGLShader::UploadUniformVec2(const std::string& a_name, const glm::vec2& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniform2fv(loc, 1, glm::value_ptr(a_value));
	}
	void OpenGLShader::UploadUniformVec3(const std::string& a_name, const glm::vec3& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniform3fv(loc, 1, glm::value_ptr(a_value));
	}
	void OpenGLShader::UploadUniformVec4(const std::string& a_name, const glm::vec4& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniform4fv(loc, 1, glm::value_ptr(a_value));
	}

	//MATRIX
	void OpenGLShader::UploadUniformMat2(const std::string& a_name, const glm::mat2& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(a_value));
	}
	void OpenGLShader::UploadUniformMat3(const std::string& a_name, const glm::mat3& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(a_value));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& a_name, const glm::mat4& a_value)
	{
		unsigned int loc = glGetUniformLocation(m_ID, a_name.c_str());
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(a_value));
	}

	void OpenGLShader::UploadTexture(const std::string& a_name, const std::shared_ptr<Renderer::Texture> a_texture, const uint8_t& a_textureUint)
	{
		glActiveTexture(GL_TEXTURE0 + a_textureUint);
		UploadUniformInt(a_name, a_textureUint);
		a_texture->Bind();
	}

	void OpenGLShader::Release()
	{
		glDeleteProgram(m_ID);
	}

	uint32_t OpenGLShader::ShaderTypeFromString(const std::string& a_type)
	{
		if (a_type == "vertex") return GL_VERTEX_SHADER;
		if (a_type == "fragment") return GL_FRAGMENT_SHADER;

		return 0;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& a_shaderSources)
	{
		unsigned int program = glCreateProgram();
		if (a_shaderSources.size() > 2)
		{
			printf("2 shaders are supported. No more than that!");
			exit(0);
		}
		//Stack allocated array. Vector is heap allocated
		std::array<GLenum, 2> shaderIDs;
		int glShaderIDIndex = 0;

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
			shaderIDs[glShaderIDIndex++]= shader;
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