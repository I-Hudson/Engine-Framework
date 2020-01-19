#pragma once

#include "imgui.h"
#include <glm/glm.hpp>
#include "GUI/GUIPanel.h"

namespace Framework
{
	namespace GUI
	{
		class GUIWindow : public GUIPanel
		{
		public:
			GUIWindow();
			GUIWindow(const glm::vec2& position, const glm::vec2& size);
			GUIWindow(const ImVec2& position, const ImVec2& size);
			~GUIWindow();

			virtual void OnUpdate() override;
			virtual void OnDraw() override;

		private:
			virtual void SetType() override;
		};
	}
}