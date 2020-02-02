#pragma once

#include "Events/Event.h"
#include <Renderer\Material.h>

namespace Framework
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Close";
			return ss.str();
		}
	};

	class WindowResizeEvent : public Event
	{
	public:

		WindowResizeEvent(unsigned int a_width, unsigned int a_height)
		: m_width(a_width), m_height(a_height)
		{}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window Resize Event: " << m_width <<", " << m_height << "\n";
			return ss.str();
		}
	private:
		unsigned int m_width, m_height;
	};

	class RendererMaterialSelected : public Event
	{
	public:
		RendererMaterialSelected(Framework::Renderer::Material* seltectedMaterial) 
			: material(seltectedMaterial)
		{ }

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

			std::string ToString() const override
		{
			std::stringstream ss;
			ss << "New Material Selected";
			return ss.str();
		}

	private:
		Framework::Renderer::Material* material;
	};

	class VulkanRecreateShaders : public Event
	{
	public:
		VulkanRecreateShaders()
		{ }

		EVENT_CLASS_TYPE(VulkanShaderRecreate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

			std::string ToString() const override
		{
			std::stringstream ss;
			ss << "";
			return ss.str();
		}
	};
}