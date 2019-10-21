#pragma once

#include "Events/Event.h"

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
}