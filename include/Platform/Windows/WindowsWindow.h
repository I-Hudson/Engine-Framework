#pragma once

#include "Renderer/GraphicsContext.h"
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
		void Destroy() override;

		//width
		inline unsigned int GetWidth() const override { return mData.Width; };
		//height
		inline unsigned int GetHeight() const override { return mData.Height; };

		//set event callback
		inline void SetEventCallback(EventCallbackFn aCallback) override { mData.EventCallback = aCallback; };

		//set vsync
		void SetVSync(bool aEnabled) override;
		//is vsync on or off
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_window; };
		inline virtual Renderer::GraphicsContext* GetGraphicsContext() const override { return m_context; }

		void FireEvent(Event& event) { mData.EventCallback(event); }

	private:
		//init func
		virtual void Init(const WindowProps& aProps);
		//shutdown
		virtual void Shutdown();

		//glfww window pointer
		void* m_window;
		Renderer::GraphicsContext* m_context;

		//Window data
		WindowData mData;
	};
}