#pragma once

//-------------------------------------------------------------------------------
// Interface for each render API platform
//-------------------------------------------------------------------------------

#include <glm/glm.hpp>
#include <memory>
#include "VertexArray.h"

namespace Framework
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
			DirectX = 2
		};

		//Define all render API calls needed. These are overridden later by the specific API
		virtual void SetClearColor(const glm::vec4& aColor) = 0;
		virtual void Clear() = 0;

		//Draw the vertex array data
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& aVertexArray) = 0;

		//Return the API
		inline static API GetAPI() { return sAPI; }

	private:
		static API sAPI;
	};
}