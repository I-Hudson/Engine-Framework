#include "GraphicsContext.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/DirectX/DirectXContext.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

namespace Framework
{
	GraphicsContext* GraphicsContext::Create(const int& a_width, const int& a_height, const std::string& a_title, 
																const bool& a_fullscreen, void* a_window, Window::WindowData* a_windowData)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: return nullptr;
			case RendererAPI::API::OpenGL: return DBG_NEW OpenGLContext(a_width, a_height, a_title, a_fullscreen, a_windowData);
			//case RendererAPI::API::DirectX: return new DirectXContext(a_width, a_height, a_title, a_fullscreen, a_window);
		}
		return nullptr;
	}
}