#pragma once

#include "RenderCommand.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

namespace Framework
{
	struct RenderRequest
	{

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

		static void AddDirLight(glm::vec3* dirLight);

		static void SubmitBatch(const std::shared_ptr<Shader> a_shader, const std::shared_ptr<VertexArray> a_vertexArray, const glm::mat4& a_transform = glm::mat4(1.0f));
		//Submit a shader and vertex array to render
		static void Submit(const std::shared_ptr<Shader> a_shader, const std::shared_ptr<VertexArray> a_vertexArray, const glm::mat4& a_transform = glm::mat4(1.0f));
		static void Submit(const std::shared_ptr<Shader> a_shader, const std::shared_ptr<VertexArray> a_vertexArray, std::vector<std::string> a_textures, const glm::mat4& a_transform = glm::mat4(1.0f));
		static void SubmitBatched(const std::shared_ptr<Shader>, const glm::mat4& a_transform = glm::mat4(1.0f));

		//Get the API in use
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		//Scene data needed
		struct SceneData
		{
			std::vector<glm::vec3*> m_dirLights;
			glm::mat4 ProjectionViewMatrix;
		};

		//Pointer to static scene data
		static SceneData* m_sceneData;

		//static Vertex* m_batchVertex;
		//static unsigned int m_batchVertexCount;
		//
		//static unsigned int* m_batchIndices;
		//static unsigned int m_batchIndicesCount;
		//
		//
		//static bool m_init;
		//static std::shared_ptr<VertexArray> m_batchArray;

	};
}