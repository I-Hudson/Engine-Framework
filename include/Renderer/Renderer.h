#pragma once

#include "Renderer/RenderCommand.h"
#include "Renderer/Shader.h"
#include "Camera.h"
#include "Renderer/Texture.h"
#include "Renderer/Material.h"
#include "Renderer/GBuffer.h"

namespace Framework
{
	namespace Renderer
	{
		struct RenderRequest
		{
			Material* Material;
			VertexArray* VertexArray;
			std::vector<std::string> Textures;
			glm::mat4 Transform;
		};

		///
		//RENDERER. RENDER OBJECTS TO THE SCREEN.
		///
		class Renderer
		{
		public:
			//Make scene ready to rendering
			static void Begin(Camera& a_camera);
			//Scene has finished rendering
			static void EndScene();

			static void Destroy();

			static void SetAmbiantLightColour(const glm::vec3& lightColour);
			static void SetAmbiantLightIntensity(const float& lightInten);
			static void AddDirLight(glm::vec3* dirLight);

			static void Submit(Material* a_material, const std::shared_ptr<VertexArray> a_vertexArray, std::vector<std::string> a_textures, const glm::mat4& a_transform = glm::mat4(1.0f));

			static void Render();

			//Get the API in use
			inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		private:
			//Scene data needed
			struct SceneData
			{
				glm::vec3 m_ambiantLight;
				float m_ambiantLightIntenstiy;
				std::vector<glm::vec3*> m_dirLights;
				glm::mat4 ProjectionViewMatrix;

				glm::mat4 ProjectionMatrix;
				glm::mat4 ViewMatrix;
				
				std::vector<RenderRequest> m_renderQueue;
			};

			static GBuffer* m_gBuffer;

			//Pointer to static scene data
			static SceneData* m_sceneData;
		};
	}
}