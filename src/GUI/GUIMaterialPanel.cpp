#include "GUI/GUIMaterialPanel.h"

namespace Framework
{
	namespace GUI
	{
		GUIMaterialPanel::GUIMaterialPanel()
		{
		}

		GUIMaterialPanel::GUIMaterialPanel(const glm::vec2& position, const glm::vec2& size)
			: GUIPanel(position, size)
		{
		}

		GUIMaterialPanel::GUIMaterialPanel(const ImVec2& position, const ImVec2& size)
			: GUIPanel(position, size)
		{
		}

		GUIMaterialPanel::~GUIMaterialPanel()
		{
		}

		void GUIMaterialPanel::OnUpdate()
		{
			OnPreUpdate();
			ImGui::Begin("Material Window");

			//ImGui::LabelText("Texture", "");
			ImGui::Image((void*)(intptr_t)1, ImVec2(m_size.x * 0.4f, m_size.x * 0.4f));
			{
			
			}

			ImGui::End();
		}
		void GUIMaterialPanel::OnDraw()
		{
		}

		void GUIMaterialPanel::SetType()
		{
			m_type = GUITypes::GUI_MATERIAL_PANEL;
		}
	}
}