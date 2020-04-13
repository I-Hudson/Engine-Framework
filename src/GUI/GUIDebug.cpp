#include "GUI/GUIDebug.h"
#include "FWTime.h"

namespace Framework
{
	namespace GUI
	{
		GUIDebug::GUIDebug()
		{
		}

		GUIDebug::GUIDebug(const glm::vec2& position, const glm::vec2& size)
			: GUIPanel(position, size)
		{
		}

		GUIDebug::GUIDebug(const ImVec2& position, const ImVec2& size)
			: GUIPanel(position, size)
		{
		}

		GUIDebug::~GUIDebug()
		{
		}

		void GUIDebug::OnUpdate()
		{
			OnPreUpdate();
			ImGui::Begin("Debug Window");

			ImGui::Text("FPS: %f", 1.0f / Time::GetDeltaTime());

			ImGui::End();
		}

		void GUIDebug::OnDraw()
		{
		}

		void GUIDebug::SetType()
		{
			m_type = GUITypes::GUI_DEBUG;
		}
	}
}
