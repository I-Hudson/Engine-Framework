#pragma once

///
// Base framework class for all OpenGL projects. This is the main application.
///

#include <memory>

#include "Camera.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Log.h"
#include "FWTime.h"
#include "Shape/Cube.h"
#include "Shader.h"
#include "Texture.h"
#include "Platform/OpenGL/OpenGLContext.h"

struct GLFWwindow;

namespace Framework
{
	class Application
	{
	public:
		Application();
		~Application();

		bool CreateApp(const int& a_width, const int& a_height, const char* a_title, const bool& a_runDemo);
		void RunApp(const int& a_width, const int& a_height, const char* a_title, const bool& a_runDemo);
		void DestroyApp();

		inline Application& Get() { return *sInstance; }

	protected:

		virtual bool OnCreate() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void Destroy() = 0;

		bool m_isRunning;

		ShaderLibrary m_shaderLibrary;
		TextureLibrary m_textureLibrary;
		std::shared_ptr<Camera> m_mainCamera;
		std::shared_ptr<Cube>m_demoCube;

		std::shared_ptr<GraphicsContext> m_context;

	private:
		static Application* sInstance;
	};
}