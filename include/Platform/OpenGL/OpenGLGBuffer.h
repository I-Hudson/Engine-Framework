#pragma once

#include "Renderer/GBuffer.h"

#include <glad/glad.h>

#include <string>
#include <vector>

namespace Framework
{
	struct OpenGLGBufferAttachment
	{
		std::string Name;
		int Width;
		int Height;
		GLenum Format;
		GLenum Type;
		GLenum Attachement;
		unsigned int Texture;
	};

	class OpenGLGBuffer : public Renderer::GBuffer
	{
	public:
		OpenGLGBuffer();
		~OpenGLGBuffer();

		virtual void Init() override;
		virtual void Free() override;

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void AddAttachment(int aLevel, int aInternalFormat,
									int aWidth, int aHeight,
									int aBorder, int aFormat,
									int aType, int aAttachment,
									std::string aName) override;

		std::vector<OpenGLGBufferAttachment*> m_attachments;
		std::vector<GLenum> m_drawBuffers;
	};
}
