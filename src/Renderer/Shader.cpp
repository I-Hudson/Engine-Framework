#include "Renderer/Shader.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <Platform\DirectX\DirectXShader.h>

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
			}
			printf("No shader was created!");
			return nullptr;
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