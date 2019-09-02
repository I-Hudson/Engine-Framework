#pragma once

///
// Base framework class for all OpenGL projects. This is the main application.
///

#include <memory>

#include "Camera.h"
#include "Renderer.h"
#include "Shader.h"
#include "Time.h"
#include "Shape/Cube.h"

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

		std::shared_ptr<Camera> m_mainCamera;
		std::shared_ptr<Shader> m_demoShader;
		std::shared_ptr<Cube>m_demoCube;

		GLFWwindow* m_window;

	private:
		static Application* sInstance;
	};
}