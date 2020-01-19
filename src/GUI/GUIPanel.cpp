#include "GUI/GUIPanel.h"

namespace Framework
{
	namespace GUI
	{
		GUIPanel::GUIPanel()
			: m_position(ImVec2(0.0f, 0.0f)), m_size(ImVec2(0.0f, 0.0f)), m_guiManager(nullptr)
		{
		}

		GUIPanel::GUIPanel(const glm::vec2& position, const glm::vec2& size)
			: m_position(ImVec2(position.x, position.y)), m_size(ImVec2(size.x, size.y)), m_guiManager(nullptr)
		{
		}

		GUIPanel::GUIPanel(const ImVec2& position, const ImVec2& size)
			: m_position(position), m_size(size), m_guiManager(nullptr)
		{
		}

		GUIPanel::~GUIPanel()
		{
		}

		void GUIPanel::Init(GUIManager* guiManager)
		{
			m_guiManager = guiManager;
			SetType();
		}

		void GUIPanel::SetPosition(const ImVec2& position)
		{
			m_position = position;
		}

		void GUIPanel::SetSize(const ImVec2& size)
		{
			m_size = size;
		}

		void GUIPanel::OnPreUpdate()
		{
			ImGui::SetNextWindowPos(m_position);
			ImGui::SetNextWindowSize(m_size);
		}

		GUIManager* GUIPanel::GetGuiManager() const
		{
			return m_guiManager;
		}

		void GUIPanel::SetType()
		{
			m_type = GUITypes::GUI_PANEL;
		}
	}
}