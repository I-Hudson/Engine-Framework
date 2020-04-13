#pragma once

#include "GUI/GUIPanel.h"

namespace Framework
{
	namespace GUI
	{
		class GUIDebug : public GUIPanel
		{
		public:
			GUIDebug();
			GUIDebug(const glm::vec2& position, const glm::vec2& size);
			GUIDebug(const ImVec2& position, const ImVec2& size);
			~GUIDebug();

			virtual void OnUpdate() override;
			virtual void OnDraw() override;

		private:
			virtual void SetType() override;
		};
	}
}