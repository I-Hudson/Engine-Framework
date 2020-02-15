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
		GBuffer* Renderer::m_gBuffer = nullptr;

		static int renderCalls = 0;

		void Renderer::Begin(Camera& a_camera)
		{
			if (m_gBuffer == nullptr)
			{
				m_gBuffer = GBuffer::Create();
			}

			renderCalls = 0;
			m_sceneData->ProjectionViewMatrix = a_camera.GetProjViewMatrix();
			m_sceneData->ProjectionMatrix = a_camera.GetProjMatrix();
			m_sceneData->ViewMatrix = a_camera.GetViewMatrix();
			m_sceneData->m_renderQueue.clear();

			RenderCommand::BeginRender(m_gBuffer);
		}

		void Renderer::EndScene()
		{
			Render();
			//std::cout << "Number of render calls: " << renderCalls << "\n";
			RenderCommand::EndRender(m_gBuffer);
		}

		void Renderer::Destroy()
		{
			delete m_sceneData;
			m_gBuffer->Free();
			delete m_gBuffer;
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

		void Renderer::Submit(Material* a_material, const std::shared_ptr<VertexArray> a_vertexArray, std::vector<std::string> a_textures, const glm::mat4& a_transform)
		{
			RenderRequest newRequest;
			newRequest.Material = a_material;
			newRequest.VertexArray = &*a_vertexArray;
			newRequest.Textures = a_textures;
			newRequest.Transform = a_transform;

			m_sceneData->m_renderQueue.push_back(newRequest);
		}

		void Renderer::Render()
		{
			for (size_t i = 0; i < m_sceneData->m_renderQueue.size(); ++i)
			{
				m_sceneData->m_renderQueue[i].Material->GetShader()->Bind(m_gBuffer);
				m_sceneData->m_renderQueue[i].Material->SetMat4("u_Projection", m_sceneData->ProjectionMatrix);
				m_sceneData->m_renderQueue[i].Material->SetMat4("u_View", m_sceneData->ViewMatrix);
				m_sceneData->m_renderQueue[i].Material->SetMat4("u_ObjectMatrix", m_sceneData->m_renderQueue[i].Transform);
				m_sceneData->m_renderQueue[i].Material->SetVec4("u_ViewPos", m_sceneData->ProjectionViewMatrix[3]);
				for (size_t i = 0; i < m_sceneData->m_dirLights.size(); i++)
				{
					m_sceneData->m_renderQueue[i].Material->SetVec4("u_DirLight", glm::vec4(*m_sceneData->m_dirLights[i], 1.0));
				}
				m_sceneData->m_renderQueue[i].Material->SetVec4("u_AmbiantLight", glm::vec4(m_sceneData->m_ambiantLight, 1.0f));
				m_sceneData->m_renderQueue[i].Material->SetFloat("u_AmbiantLightInten", m_sceneData->m_ambiantLightIntenstiy);

				m_sceneData->m_renderQueue[i].Material->SetUniforms(m_gBuffer);

				m_sceneData->m_renderQueue[i].VertexArray->Bind();
				RenderCommand::DrawIndexed(m_sceneData->m_renderQueue[i].VertexArray, m_gBuffer);
				++renderCalls;

				m_sceneData->m_renderQueue[i].Material->GetShader()->Unbind(m_gBuffer);
				m_sceneData->m_renderQueue[i].VertexArray->Unbind();
			}
		}
	}
}