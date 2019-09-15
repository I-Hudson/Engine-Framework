#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Log.h"

namespace Framework
{
	class Texture
	{
	public:
		virtual ~Texture() = default;


		virtual void Load(const std::string& a_filePath) = 0;
		virtual void Release() = 0;
		virtual const std::string& GetName() = 0;

		static std::shared_ptr<Texture> Create(const std::string& a_name, const std::string& a_filePath);
	};


	class TextureLibrary
	{
	public:
		void Add(const std::string& a_name, const std::shared_ptr<Texture>& a_texture);
		std::shared_ptr<Texture> GetTexture(const std::string& a_name);

		std::shared_ptr<Texture> Load(const std::string a_name, std::string& a_filePath);

		void Release(const std::string a_name);
		void ReleaseAll();

		bool Exists(const std::string& a_name);

	private:
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
	};
}