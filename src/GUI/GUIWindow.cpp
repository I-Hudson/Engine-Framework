#include "GUI/GUIWindow.h"

namespace Framework
{
	namespace GUI
	{
		GUIWindow::GUIWindow()
		{
		}

		GUIWindow::GUIWindow(const glm::vec2& position, const glm::vec2& size)
			: GUIPanel(position, size)
		{

		}

		GUIWindow::GUIWindow(const ImVec2& position, const ImVec2& size)
			: GUIPanel(position, size)
		{

		}

		GUIWindow::~GUIWindow()
		{
		}

		void GUIWindow::OnUpdate()
		{
			OnPreUpdate();
			ImGui::Begin("TEST WINDOW");
			ImGui::End();
		}

		void GUIWindow::OnDraw()
		{
		}

		void GUIWindow::SetType()
		{
			m_type = GUITypes::GUI_WINDOW;
		}
	}
}