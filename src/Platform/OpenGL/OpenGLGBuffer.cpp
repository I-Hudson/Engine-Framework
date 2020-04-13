#include "Platform/OpenGL/OpenGLGBuffer.h"

#include "glad/glad.h"
#include "Log.h"

namespace Framework
{
	OpenGLGBuffer::OpenGLGBuffer()
	{
	}

	OpenGLGBuffer::~OpenGLGBuffer()
	{
	}

	void OpenGLGBuffer::Init()
	{
		glGenFramebuffers(1, &m_ID);
	}

	void OpenGLGBuffer::Free()
	{

	}

	void OpenGLGBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glDrawBuffers(m_drawBuffers.size(), m_drawBuffers.data());
	}

	void OpenGLGBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLGBuffer::AddAttachment(int aLevel, int aInternalFormat,
										int aWidth, int aHeight,
										int aBorder, int aFormat,
										int aType, int aAttachment,
										std::string aName)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		OpenGLGBufferAttachment* newAttach = new OpenGLGBufferAttachment();
		newAttach->Name = aName;
		newAttach->Width = aWidth;
		newAttach->Height = aHeight;
		newAttach->Format = aFormat;
		newAttach->Attachement = aAttachment;
		newAttach->Type = aType;
		
		glGenTextures(1, &newAttach->Texture);
		glBindTexture(GL_TEXTURE_2D, newAttach->Texture);
		glTexImage2D(GL_TEXTURE_2D, aLevel, aInternalFormat, newAttach->Width, newAttach->Height, aBorder, newAttach->Format, newAttach->Type, 0);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		if (aAttachment == GL_DEPTH_ATTACHMENT)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		
		glFramebufferTexture(GL_FRAMEBUFFER, newAttach->Attachement, newAttach->Texture, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		m_attachments.push_back(newAttach);
		
		if (newAttach->Attachement != GL_DEPTH_STENCIL_ATTACHMENT &&
			newAttach->Attachement != GL_DEPTH_ATTACHMENT)
		{
			m_drawBuffers.push_back(newAttach->Attachement);
		}
		
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			EN_ERROR("Framebuffer not created correctly!");
		}

		Unbind();
	}
}