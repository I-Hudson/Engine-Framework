#pragma once

#include <string>
#include "Events/Event.h"
#include <Windows.h>

namespace Framework
{
	class GraphicsContext;
	
	//window props
	struct WindowProps
	{
		//Title, width and height
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		//Constructor
		WindowProps(const std::string& aTitle = "InSight Engine",
			unsigned int aWidth = 1920, unsigned int aHeight = 1080)
			:Title(aTitle), Width(aWidth), Height(aHeight)
		{ }
	};

	//Interface representing a desktop system based window
	class Window
	{
	public:
		//event callback
		using EventCallbackFn = std::function<void(Event&)>;

		//destructor
		virtual ~Window() {}

		//update
		virtual void OnUpdate() {}
		virtual void Destroy() = 0;

		//width
		virtual unsigned int GetWidth() const { return 0; }
		//height
		virtual unsigned int GetHeight() const { return 0; }

		//set the event callback
		virtual void SetEventCallback(const EventCallbackFn& aCallback) {}
		//set vsync
		virtual void SetVSync(bool aEnabled) {}
		//is vsync on or off
		virtual bool IsVSync() const { return false; }

		//get the window pointer
		virtual void* GetNativeWindow() const { return nullptr; }
		virtual std::shared_ptr<GraphicsContext> GetGraphicsContext() const = 0;

		//create window
		static std::shared_ptr<Window> Create(const WindowProps& aProps);
	};
}