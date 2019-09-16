#include "Platform/OpenGL/OpenGLTexture.h"

#include <glad/glad.h>

namespace Framework
{
	OpenGLTexture::OpenGLTexture(const std::string& a_name, const std::string& a_shaderFile)
		: m_ID(0), m_name(a_name)
	{
	}

	OpenGLTexture::~OpenGLTexture()
	{
	}

	void OpenGLTexture::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void OpenGLTexture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture::Load(const std::string& a_filePath)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char* data = nullptr;

		data = stbi_load(a_filePath.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);
	}

	void OpenGLTexture::Release()
	{
		glDeleteTextures(1, &m_ID);
	}

	const std::string& OpenGLTexture::GetName()
	{
		return m_name;
	}
}