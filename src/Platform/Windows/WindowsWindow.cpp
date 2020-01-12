#include "Platform/Windows/WindowsWindow.h"

#include "Renderer.h"
#include "Platform/DirectX/DirectXContext.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Framework
{
	WindowsWindow::WindowsWindow(const WindowProps& a_props)
		: m_context(nullptr)
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
		if (m_context != nullptr)
		{
			m_context->Destroy();
			delete m_context;
		}
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
		mData = WindowData();
		mData.Title = aProps.Title;
		mData.Width = aProps.Width;
		mData.Height = aProps.Height;

		m_context = GraphicsContext::Create(aProps.Width, aProps.Height, aProps.Title, false, nullptr, &mData);
	}

	void WindowsWindow::Shutdown()
	{
	}
}