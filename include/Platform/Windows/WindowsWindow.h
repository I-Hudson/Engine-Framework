#pragma once

#include "GraphicsContext.h"
#include "Window.h"

namespace Framework
{
	class WindowsWindow : public Window
	{
	public:
		//construcotr
		WindowsWindow(const WindowProps& a_props);
		//destructor
		virtual ~WindowsWindow();

		//update
		void OnUpdate() override;

		//width
		inline unsigned int GetWidth() const override { return mData.Width; };
		//height
		inline unsigned int GetHeight() const override { return mData.Height; };

		//set event callback
		inline void SetEventCallback(const EventCallbackFn& aCallback) override { mData.EventCallback = aCallback; };
		GraphicsContext* GetContext() { return m_context; }

		//set vsync
		void SetVSync(bool aEnabled) override;
		//is vsync on or off
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return mWindow; };

	private:
		//init func
		virtual void Init(const WindowProps& aProps);
		//shutdown
		virtual void Shutdown();

		//glfww window pointer
		void* mWindow;
		GraphicsContext* m_context;

		//window data
		struct WindowData
		{
			//title
			std::string Title;
			//width, height
			unsigned int Width, Height;
			//vsync
			bool VSync;

			//event callback
			EventCallbackFn EventCallback;
		};

		//Window data
		WindowData mData;
	};
}