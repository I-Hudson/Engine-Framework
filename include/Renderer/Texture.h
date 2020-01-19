#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Log.h"

#include "stbi/stb_image.h"

namespace Framework
{
	namespace Renderer
	{
		enum TextureType
		{
			DIFFUSE,
			SPECULAR,
			AMBIENT,
			EMISSIVE,
			HEIGHT,
			NORMALS,
			SHININESS,
			OPACITY,
			DISPLACEMENT,
			LIGHTMAP,
			REFLECTION,
			UNKNOWN
		};

		class Texture
		{
		public:
			virtual ~Texture() = default;

			virtual void Bind() = 0;
			virtual void Unbind() = 0;

			virtual void Load(const std::string& a_filePath, const TextureType& type) = 0;
			virtual void Release() = 0;
			virtual const std::string& GetName() = 0;

			TextureType GetType() const { return m_type; }
			uint32_t GetId() const { return m_ID; }

			std::string TextureTypeToString(const TextureType& type = TextureType::UNKNOWN)
			{
				TextureType localType = type;

				if (type == TextureType::UNKNOWN)
				{
					localType = m_type;
				}

				switch (localType)
				{
				case TextureType::DIFFUSE:
					return std::string("diffuse");
				case TextureType::SPECULAR:
					return std::string("specular");
				case TextureType::AMBIENT:
					return std::string("ambient");
				case TextureType::EMISSIVE:
					return std::string("emissive");
				case TextureType::HEIGHT:
					return std::string("height");
				case TextureType::NORMALS:
					return std::string("normals");
				case TextureType::SHININESS:
					return std::string("shininess");
				case TextureType::OPACITY:
					return std::string("opacity");
				case TextureType::DISPLACEMENT:
					return std::string("displacement");
				case TextureType::LIGHTMAP:
					return std::string("lightmap");
				case TextureType::REFLECTION:
					return std::string("reflection");
				default:
					break;
				}
				return std::string("Unknown");
			}

			static std::shared_ptr<Texture> Create(const std::string& a_name, const std::string& a_filePath, const TextureType& type);

		protected:
			uint32_t m_ID;
			TextureType m_type;
		};


		class TextureLibrary
		{
		public:
			void Add(const std::string& a_name, const std::shared_ptr<Texture>& a_texture);
			std::shared_ptr<Texture> GetTexture(const std::string& a_name);

			std::shared_ptr<Texture> Load(const std::string a_name, const std::string& a_filePath, const TextureType& type);

			void Release(const std::string a_name);
			void ReleaseAll();

			bool Exists(const std::string& a_name);

		private:
			std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
		};
	}
}