#pragma once

#include "Texture.h"

namespace Framework
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const std::string& a_name, const std::string& a_textureFile, const TextureType& type);
		~OpenGLTexture() override;

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Load(const std::string& a_filePath, const TextureType& type) override;
		virtual void Release() override;
		virtual const std::string& GetName() override;

	private:
		std::string m_name;
	};
}