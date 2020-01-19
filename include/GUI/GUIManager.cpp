#include "GUIManager.h"

namespace Framework
{
	namespace GUI
	{
		GUIManager::GUIManager()
		{
		}

		GUIManager::~GUIManager()
		{
		}

		void GUIManager::OnUpdate()
		{
			for (auto it : m_panels)
			{
				it->OnUpdate();
			}
		}

		void GUIManager::OnDraw()
		{
			for (auto it : m_panels)
			{
				it->OnDraw();
			}
		}

		void GUIManager::Remove(GUIPanel* panel)
		{
		}

		void GUIManager::Destroy()
		{
			for (auto it : m_panels)
			{
				delete it;
			}
			m_panels.clear();
			m_panelBits.reset();
		}
	}
}