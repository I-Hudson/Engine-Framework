#include "Renderer/Shader.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <Platform\DirectX\DirectXShader.h>
#include "Platform/Vulkan/VulkanShader.h"

#include <fstream>

namespace Framework
{
	namespace Renderer
	{
		std::shared_ptr<Shader> Shader::Create(const std::string& a_filePath)
		{
			switch (Renderer::GetAPI())
			{
			case RendererAPI::API::None: exit(0);
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(a_filePath);
			}
			printf("No shader was created!");
			return nullptr;
		}
		std::shared_ptr<Shader> Shader::Create(const std::string& a_name, const std::string& a_vertexSource, const std::string& a_fragSource)
		{
			switch (Renderer::GetAPI())
			{
			case RendererAPI::API::None: exit(0);
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(a_name, a_vertexSource, a_fragSource);
			case RendererAPI::API::DirectX: return std::make_shared<DirectXShader>(a_name, a_vertexSource, a_fragSource);
			case RendererAPI::API::Vulkan: return std::make_shared<Vulkan::VulkanShader>(a_name, a_vertexSource, a_fragSource);
			}
			printf("No shader was created!");
			return nullptr;
		}

		std::string Shader::ReadFromFile(const std::string& a_filePath)
		{
			std::string result;
			std::ifstream in(a_filePath, std::ios::in | std::ios::binary);
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
				std::string type = a_source.substr(begin, eol - begin);

				size_t nextLinePos = a_source.find_first_not_of("\r\n", eol);
				pos = a_source.find(typeToken, nextLinePos);
				shaderSources[ShaderTypeFromString(type)] = a_source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? a_source.size() - 1 : nextLinePos));
			}

			return shaderSources;
		}

		void ShaderLibrary::Add(const std::string& a_name, const std::shared_ptr<Shader>& a_shader)
		{
			if (Exists(a_name))
			{
				printf("Shader allready exists: %s", a_name.c_str());
				return;
			}
			m_shaders[a_name] = a_shader;
		}

		void ShaderLibrary::Add(const std::shared_ptr<Shader>& a_shader)
		{
			auto& name = a_shader->GetName();
			Add(name, a_shader);
		}

		std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& a_filePath)
		{
			auto shader = Shader::Create(a_filePath);
			Add(shader);
			return shader;
		}

		std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& a_name, const std::string& a_filePath)
		{
			auto shader = Shader::Create(a_filePath);
			Add(shader);
			return shader;
		}

		std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& a_name, const std::string& a_filePathVertex, const std::string& a_filePathFrag)
		{
			auto shader = Shader::Create(a_name, a_filePathVertex, a_filePathFrag);
			Add(shader);
			return shader;
		}

		std::shared_ptr<Shader> ShaderLibrary::GetShader(const std::string& a_name)
		{
			bool exists = Exists(a_name);
			return m_shaders[a_name];
		}

		void ShaderLibrary::ReleaseAll()
		{
			for (auto& kv : m_shaders)
			{
				kv.second->Release();
			}
			m_shaders.clear();
		}

		bool ShaderLibrary::Exists(const std::string& a_name)
		{
			return m_shaders.find(a_name) != m_shaders.end();
		}
	}
}