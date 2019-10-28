#include "Platform/Windows/WindowsWindow.h"

#include "Renderer.h"
#include "Platform/DirectX/DirectXContext.h"

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

	void WindowsWindow::SetVSync(bool aEnabled)
	{
	}

	bool WindowsWindow::IsVSync() const
	{
		return false;
	}

	void WindowsWindow::Init(const WindowProps& aProps)
	{
	}

	void WindowsWindow::Shutdown()
	{
	}
}