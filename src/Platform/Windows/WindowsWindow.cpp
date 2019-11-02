#include "Platform/Windows/WindowsWindow.h"

#include "Renderer.h"
#include "Platform/DirectX/DirectXContext.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Framework
{
	WindowsWindow::WindowsWindow(const WindowProps& a_props)
	{
		Init(a_props);
	}

	WindowsWindow::~WindowsWindow()
	{
	}

	void WindowsWindow::OnUpdate()
	{
		m_context->SwapBuffers();
	}

	void WindowsWindow::Destroy()
	{
		m_context->Destroy();
	}

	void WindowsWindow::SetVSync(bool aEnabled)
	{
	}

	bool WindowsWindow::IsVSync() const
	{
		return false;
	}

	void WindowsWindow::Init(const WindowProps& aProps)
	{
		m_context = GraphicsContext::Create(aProps.Width, aProps.Height, aProps.Title, false, nullptr);
	}

	void WindowsWindow::Shutdown()
	{
	}
}