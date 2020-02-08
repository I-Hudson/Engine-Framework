#include "Renderer/Renderer.h"

#include "Platform/Vulkan/VulkanRendererAPI.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Application.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Framework
{
	namespace Renderer
	{
		Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData();

		static int renderCalls = 0;

		void Renderer::Begin(Camera& a_camera)
		{
			renderCalls = 0;
			m_sceneData->ProjectionViewMatrix = a_camera.GetProjViewMatrix();
			m_sceneData->ProjectionMatrix = a_camera.GetProjMatrix();
			m_sceneData->ViewMatrix = a_camera.GetViewMatrix();
			m_sceneData->m_renderQueue.clear();

			RenderCommand::BeginRender();
		}

		void Renderer::EndScene()
		{
			Render();
			static_cast<Vulkan::VulkanRendererAPI*>(RenderCommand::GetAPI())->GetNextFrameRender();

			//std::cout << "Number of render calls: " << renderCalls << "\n";
			RenderCommand::EndRender();
		}

		void Renderer::Destroy()
		{
			delete m_sceneData;
		}

		void Renderer::SetAmbiantLightColour(const glm::vec3& lightColour)
		{
			m_sceneData->m_ambiantLight = lightColour;
		}

		void Renderer::SetAmbiantLightIntensity(const float& lightInten)
		{
			m_sceneData->m_ambiantLightIntenstiy = lightInten;
		}

		void Renderer::AddDirLight(glm::vec3* dirLight)
		{
			m_sceneData->m_dirLights.push_back(dirLight);
		}

		void Renderer::SubmitBatch(const std::shared_ptr<Shader> a_shader, const std::shared_ptr<VertexArray> a_vertexArray, const glm::mat4& a_transform)
		{
			std::shared_ptr<VertexBuffer> vBuffer = a_vertexArray->GetVertexBuffers()[0];
			std::shared_ptr<IndexBuffer> iBuffer = a_vertexArray->GetIndexBuffer();
		}

		void Renderer::Submit(const std::shared_ptr<Shader> a_shader, const std::shared_ptr<VertexArray> a_vertexArray, const glm::mat4& a_transform)
		{
			//RenderRequest newRequest;
			//newRequest.Material = nullptr;
			//newRequest.VertexArray = &*a_vertexArray;
			//newRequest.Textures = nullptr;
			//newRequest.Transform = a_transform;
			//
			//m_sceneData->m_renderQueue.push_back(newRequest);
		}

		void Renderer::Submit(Material* a_material, const std::shared_ptr<VertexArray> a_vertexArray, std::vector<std::string> a_textures, const glm::mat4& a_transform)
		{
			RenderRequest newRequest;
			newRequest.Material = a_material;
			newRequest.VertexArray = &*a_vertexArray;
			newRequest.Textures = a_textures;
			newRequest.Transform = a_transform;

			m_sceneData->m_renderQueue.push_back(newRequest);
		}

		void Renderer::SubmitBatched(const std::shared_ptr<Shader> a_shader, const glm::mat4& a_transform)
		{
		}

		void Renderer::Render()
		{
			for (size_t i = 0; i < m_sceneData->m_renderQueue.size(); ++i)
			{
				m_sceneData->m_renderQueue[i].Material->GetShader()->Bind();
				m_sceneData->m_renderQueue[i].Material->SetMat4("u_Projection", m_sceneData->ProjectionMatrix);
				m_sceneData->m_renderQueue[i].Material->SetMat4("u_View", m_sceneData->ViewMatrix);
				m_sceneData->m_renderQueue[i].Material->SetMat4("u_ObjectMatrix", m_sceneData->m_renderQueue[i].Transform);
				m_sceneData->m_renderQueue[i].Material->SetVec3("u_ViewPos", m_sceneData->ProjectionViewMatrix[3].xyz);
				for (size_t i = 0; i < m_sceneData->m_dirLights.size(); i++)
				{
					m_sceneData->m_renderQueue[i].Material->SetVec3("u_DirLight0", *m_sceneData->m_dirLights[i]);
				}
				m_sceneData->m_renderQueue[i].Material->SetVec3("u_AmbiantLight", m_sceneData->m_ambiantLight);
				m_sceneData->m_renderQueue[i].Material->SetFloat("u_AmbiantLightInten", m_sceneData->m_ambiantLightIntenstiy);

				m_sceneData->m_renderQueue[i].Material->SetUniforms();

				m_sceneData->m_renderQueue[i].VertexArray->Bind();
				RenderCommand::DrawIndexed(m_sceneData->m_renderQueue[i].VertexArray);
				++renderCalls;

				m_sceneData->m_renderQueue[i].Material->GetShader()->Unbind();
				m_sceneData->m_renderQueue[i].VertexArray->Unbind();
			}
		}
	}
}