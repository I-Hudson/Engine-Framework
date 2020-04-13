#pragma once

///
// Base framework class for all OpenGL projects. This is the main application.
///

#include <memory>
#include <functional>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "Camera.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Log.h"
#include "FWTime.h"
#include "Shape/Cube.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Layer/LayerStack.h"
#include "GUI/GUIManager.h"

namespace Framework
{
	//event callback
	using EventCallbackFn = std::function<void(Event&)>;

	class Application
	{
	public:

		Application();
		~Application();

		bool CreateApp(const int& a_width, const int& a_height, const char* a_title, const bool& a_runDemo);
		void RunApp(const int& a_width, const int& a_height, const char* a_title, const bool& a_runDemo);
		void DestroyApp();

		static void FireEvent(Event& appEvent) { static_cast<WindowsWindow*>(Get().m_window)->FireEvent(appEvent); }

		void PushLayer(Layer* aLayer);
		void PushOverlay(Layer* aLayer);

		void OnEvent(Event& a_event);

		inline static Application& Get()
		{
			return *s_instance;
		}

		inline Window* GetWindow() { return m_window; }

		Renderer::TextureLibrary& GetTextureLibrary() { return m_textureLibrary; }
		Renderer::ShaderLibrary& GetShaderLibrary() { return m_shaderLibrary; }
		GUI::GUIManager& GetGUIManager() { return m_guiManager; }

	protected:

		virtual bool OnCreate() { return false; }
		virtual void Update() { }
		virtual void Draw() { }
		virtual void Destroy() { }

		bool KeyDown(const int& aKeyCode);

		bool m_isRunning;

		Renderer::ShaderLibrary m_shaderLibrary;
		Renderer::TextureLibrary m_textureLibrary;
		Camera m_mainCamera;

		GUI::GUIManager m_guiManager;

		LayerStack m_layerStack;

		Window* m_window;

	private:
		void OnImGuiCreate(); 
		void OnImGuiDestroy();

		static Application* s_instance;
	};
}