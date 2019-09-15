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

	void OpenGLTexture::Load(const std::string& a_filePath)
	{
	}

	void OpenGLTexture::Release()
	{
	}

	const std::string& OpenGLTexture::GetName()
	{
		return m_name;
	}
}