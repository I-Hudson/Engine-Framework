#pragma once

#include "imgui.h"
#include "GUITypes.h"
#include <glm/glm.hpp>

namespace Framework
{
	namespace GUI
	{
		class GUIManager;

		class GUIPanel
		{
		public:
			GUIPanel();
			GUIPanel(const glm::vec2& position, const glm::vec2& size);
			GUIPanel(const ImVec2& position, const ImVec2& size);
			~GUIPanel();

			virtual void Init(GUIManager* guiManager);
			virtual void OnUpdate() = 0;
			virtual void OnDraw() = 0;

			void SetPosition(const ImVec2& position);
			void SetSize(const ImVec2& size);

			GUITypes GetType() const { return m_type; }

		protected:
			void OnPreUpdate();

			GUIManager* GetGuiManager() const;

		private:
			virtual void SetType();

		protected:
			ImVec2 m_position;
			ImVec2 m_size;
			GUITypes m_type;
			GUIManager* m_guiManager;
		};
	}
}