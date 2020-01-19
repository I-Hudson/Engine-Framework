#pragma once

#include "GUI/GUIPanel.h"
#include <glm/glm.hpp>

#include <vector>
#include <bitset>

namespace Framework
{
	namespace GUI
	{
		class GUIManager
		{
		public:
			GUIManager();
			~GUIManager();

			void OnUpdate();
			void OnDraw();

			template <typename T, typename... TArgs>
			GUIPanel* Add(TArgs&&... aArgs)
			{
				// check
				if (true)
				{
					T* panel(new T(std::forward<TArgs>(aArgs)...));
					panel->Init(this);
					m_panels.push_back(panel);

					m_panelBits[panel->GetType()] = true;

					return panel;
				}
				return nullptr;
			}

			GUIPanel* Get();
			void Remove(GUIPanel* panel);

			void Destroy();

		private:
			std::bitset<32> m_panelBits;
			std::vector<GUIPanel*> m_panels;
		};
	}
}