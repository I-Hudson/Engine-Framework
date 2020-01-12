#pragma once

#include <string>
#include "Events/Event.h"

class Layer
{
public:
	Layer(const std::string& aName = "Layer");
	virtual ~Layer();

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	virtual void OnImGuiRender() {}
	virtual void OnEvent(Framework::Event& aEvent) {}

	inline const std::string& GetName() const {return mDebugName; }

protected:
	std::string mDebugName;
};
