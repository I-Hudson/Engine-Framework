#include "Window.h"

#include "Platform/Windows/WindowsWindow.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW
#endif

namespace Framework
{
	Window* Window::Create(const WindowProps& aProps)
	{
		return DBG_NEW WindowsWindow(aProps);
	}
}