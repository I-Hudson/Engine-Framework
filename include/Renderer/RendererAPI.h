#pragma once

//-------------------------------------------------------------------------------
// Interface for each render API platform
//-------------------------------------------------------------------------------

#include <glm/glm.hpp>
#include <memory>
#include "Renderer/VertexArray.h"
#include "GraphicsContext.h"

namespace Framework
{
	namespace Renderer
	{
		class RendererAPI
		{
		public:
			enum class API
			{
				None = 0,
				OpenGL = 1,
				DirectX = 2,
				Vulkan = 3
			};

			//Define all render API calls needed. These are overridden later by the specific API
			virtual void BeginRender() = 0;
			virtual void EndRender() = 0;
			virtual void SetClearColor(const glm::vec4& aColor) = 0;
			virtual void Clear() = 0;
			virtual void SetVSync(bool a_state) = 0;
			virtual void SetDepthTest(bool a_state) = 0;
			virtual void SetCullFace(bool a_state) = 0;

			virtual void SetContext(GraphicsContext* context) = 0;

			//Draw the vertex array data
			virtual void DrawIndexed(const VertexArray* aVertexArray) = 0;

			//Return the API
			inline static API GetAPI() { return sAPI; }

		private:
			static API sAPI;
		};
	}
}