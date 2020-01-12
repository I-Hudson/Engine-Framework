#pragma once

///
// Base framework class for all OpenGL projects. This is the main application.
///

#include <memory>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "Camera.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Log.h"
#include "FWTime.h"
#include "Shape/Cube.h"
#include "Shader.h"
#include "Texture.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Layer/LayerStack.h"

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

		void PushLayer(Layer* aLayer);
		void PushOverlay(Layer* aLayer);

		void OnEvent(Event& a_event);

		inline static Application& Get() { return *sInstance; }
		inline static void Reset() { delete sInstance; }
		inline Window* GetWindow() { return m_window; }

		TextureLibrary& GetTextureLibrary() { return m_textureLibrary; }
		ShaderLibrary& GetShaderLibrary() { return m_shaderLibrary; }

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

		LayerStack m_layerStack;

		Window* m_window;

	private:
		void OnImGuiCreate(); 
		void OnImGuiDestroy();

	private:
		static Application* sInstance;
	};
}