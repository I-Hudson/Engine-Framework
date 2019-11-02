#include "Window.h"

#include "Platform/Windows/WindowsWindow.h"

namespace Framework
{
	std::shared_ptr<Window> Window::Create(const WindowProps& aProps)
	{
		return std::make_shared<WindowsWindow>(aProps);
	}
}