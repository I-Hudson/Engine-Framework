#pragma once

#include "GUI/GUIPanel.h"

namespace Framework
{
	namespace GUI
	{
		class GUIMaterialPanel : public GUIPanel
		{
		public:
			GUIMaterialPanel();
			GUIMaterialPanel(const glm::vec2& position, const glm::vec2& size);
			GUIMaterialPanel(const ImVec2& position, const ImVec2& size);
			~GUIMaterialPanel();

			virtual void OnUpdate() override;
			virtual void OnDraw() override;

		private:
			virtual void SetType() override;
		};
	}
}