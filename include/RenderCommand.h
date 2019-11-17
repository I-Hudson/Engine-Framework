#pragma once

#include "RendererAPI.h"

namespace Framework
{
	///
	// DEFINE ALL RENDER COMMANDS THAT CAN BE CALLED
	///
	class RenderCommand
	{
	public:
		//Set the clear colour
		inline static void SetClearColor(const glm::vec4& aColor)
		{
			sRendererAPI->SetClearColor(aColor);
		}

		//Clear the screen
		inline static void Clear()
		{
			sRendererAPI->Clear();
		}

		inline static void SetVSync(bool a_state)
		{
			sRendererAPI->SetVSync(a_state);
		}

		//Draw the vertex array data
		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& aVertexArray)
		{
			sRendererAPI->DrawIndexed(aVertexArray);
		}

	private:
		static RendererAPI* sRendererAPI;
	};
}