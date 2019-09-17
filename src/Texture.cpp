#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Framework
{
	std::shared_ptr<Texture> Texture::Create(const std::string& a_name, const std::string& a_filePath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: exit(0);
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture>(a_name, a_filePath);
		}

		return nullptr;
	}




	void TextureLibrary::Add(const std::string& a_name, const std::shared_ptr<Texture>& a_texture)
	{
		if (Exists(a_name))
		{
			EN_CORE_WARN("Texture {0} already exists", a_name);
		}
		m_textures[a_name] = a_texture;
	}

	std::shared_ptr<Texture> TextureLibrary::GetTexture(const std::string& a_name)
	{
		if (Exists(a_name))
		{
			return m_textures[a_name];
		}
		EN_CORE_WARN("No texture with ID {0} exists", a_name);
		return nullptr;
	}

	std::shared_ptr<Texture> TextureLibrary::Load(const std::string a_name, const std::string& a_filePath)
	{
		auto texture = Texture::Create(a_name, a_filePath);
		Add(a_name, texture);
		return texture;
	}

	void TextureLibrary::Release(const std::string a_name)
	{
		if (Exists(a_name))
		{
			m_textures.find(a_name)->second->Release();
		}
	}

	void TextureLibrary::ReleaseAll()
	{
		for (auto& texture : m_textures)
		{
			texture.second->Release();
		}
	}

	bool TextureLibrary::Exists(const std::string& a_name)
	{
		return m_textures.find(a_name) != m_textures.end();
	}
}