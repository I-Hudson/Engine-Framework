#pragma once

#include "Renderer/RendererAPI.h"

namespace Framework
{
	namespace Renderer
	{
		class GBuffer;

		///
		// DEFINE ALL RENDER COMMANDS THAT CAN BE CALLED
		///
		class RenderCommand
		{
		public:

			inline static void Destroy()
			{
				if (sRendererAPI != nullptr)
				{
					delete sRendererAPI;
				}
			}

			static void Create();

			inline static void BeginRender(GBuffer* gBuffer)
			{
				sRendererAPI->BeginRender(gBuffer);
			}

			inline static void EndRender(GBuffer* gBuffer)
			{
				sRendererAPI->EndRender(gBuffer);
			}

			static void SetGraphicsContext(Framework::Renderer::GraphicsContext* context)
			{
				sRendererAPI->SetContext(context);
			}

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

			inline static void SetDepthTest(bool a_state)
			{
				sRendererAPI->SetDepthTest(a_state);
			}

			inline static void SetCullFace(bool a_state)
			{
				sRendererAPI->SetCullFace(a_state);
			}

			//Draw the vertex array data
			inline static void DrawIndexed(const VertexArray* aVertexArray, GBuffer* gBuffer)
			{
				sRendererAPI->DrawIndexed(aVertexArray, gBuffer);
			}

			inline static RendererAPI* GetAPI()
			{
				return sRendererAPI;
			}

		private:
			static RendererAPI* sRendererAPI;
		};
	}
}