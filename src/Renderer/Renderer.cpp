#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Application.h"
#include <iostream>

namespace Framework
{
	namespace Renderer
	{
		Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData();
		//std::shared_ptr<VertexArray> Renderer::m_batchArray = nullptr;
		//Vertex* Renderer::m_batchVertex = new Vertex[650000];
		//unsigned int* Renderer::m_batchIndices = new unsigned int[650000];

		//unsigned int Renderer::m_batchVertexCount = 0;
		//unsigned int Renderer::m_batchIndicesCount = 0;
		//
		//bool Renderer::m_init = false;

		static int renderCalls = 0;

		void Renderer::Begin(Camera& a_camera)
		{
			renderCalls = 0;
			m_sceneData->ProjectionViewMatrix = a_camera.GetProjViewMatrix();

			RenderCommand::BeginRender();

			//m_batchVertexCount = 0;
			//m_batchIndicesCount = 0;
			//
			//if (!m_init)
			//{
			//	m_init = true;
			//
			//	std::shared_ptr<OpenGLVertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>(m_batchVertex, sizeof(Vertex) * 65000);
			//	BufferLayout layout =
			//	{
			//		{ShaderDataType::Float4, "inPosition"},
			//		{ShaderDataType::Float4, "inColor", true},
			//		{ShaderDataType::Float4, "inNormal", true},
			//		{ShaderDataType::Float2, "inUV", true},
			//	};
			//	vertexBuffer->SetLayout(layout);
			//
			//	std::shared_ptr<OpenGLIndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>(m_batchIndices, 65000);
			//
			//	m_batchArray = std::make_shared<OpenGLVertexArray>();
			//	m_batchArray->AddVertexBuffer(vertexBuffer);
			//	m_batchArray->AddIndexBuffer(indexBuffer);
			//}
		}

		void Renderer::EndScene()
		{
			//std::cout << "Number of render calls: " << renderCalls << "\n";
			RenderCommand::EndRender();
		}

		void Renderer::Destroy()
		{
			//delete[] m_batchVertex;
			//delete[] m_batchIndices;
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

			//int startIndex = m_batchVertexCount;
			//
			//for (size_t i = 0; i < vBuffer->GetCount(); ++i)
			//{
			//	Vertex v = *vBuffer->GetVertex(i);
			//	m_batchVertex[m_batchVertexCount] = v;
			//	m_batchVertexCount++;
			//}
			//
			//for (size_t i = 0; i < iBuffer->GetCount(); ++i)
			//{
			//	int newIndex = startIndex + iBuffer->GetIndice(i);
			//	m_batchIndices[m_batchIndicesCount] = newIndex;
			//	m_batchIndicesCount++;
			//}
		}

		void Renderer::Submit(const std::shared_ptr<Shader> a_shader, const std::shared_ptr<VertexArray> a_vertexArray, const glm::mat4& a_transform)
		{
			if (a_shader != nullptr)
			{
				a_shader->Bind();
				a_shader->UploadUniformMat4("u_ProjectionView", m_sceneData->ProjectionViewMatrix);
				a_shader->UploadUniformMat4("u_ObjectMatrix", a_transform);
			}

			if (a_vertexArray != nullptr)
			{
				a_vertexArray->Bind();
			}
			
			RenderCommand::DrawIndexed(a_vertexArray);
			++renderCalls;

			if (a_shader != nullptr)
			{
				a_shader->Unbind();
			}

			if (a_vertexArray != nullptr)
			{
				a_vertexArray->Unbind();
			}
		}

		void Renderer::Submit(Material* a_material, const std::shared_ptr<VertexArray> a_vertexArray, std::vector<std::string> a_textures, const glm::mat4& a_transform)
		{
			a_material->GetShader()->Bind();
			a_material->GetShader()->UploadUniformMat4("u_ProjectionView", m_sceneData->ProjectionViewMatrix);
			a_material->GetShader()->UploadUniformMat4("u_ObjectMatrix", a_transform);
			a_material->GetShader()->UploadUniformVec3("u_ViewPos", m_sceneData->ProjectionViewMatrix[3].xyz);
			a_material->GetShader()->UploadUniformVec3("u_DirLight0", *m_sceneData->m_dirLights[0]);
			a_material->GetShader()->UploadUniformVec3("u_AmbiantLight", m_sceneData->m_ambiantLight);
			a_material->GetShader()->UploadUniformFloat("u_AmbiantLightInten", m_sceneData->m_ambiantLightIntenstiy);

			a_material->SetUniforms();

			a_vertexArray->Bind();
			RenderCommand::DrawIndexed(a_vertexArray);
			++renderCalls;

			a_material->GetShader()->Unbind();
			a_vertexArray->Unbind();
			glActiveTexture(GL_TEXTURE0);
		}

		void Renderer::SubmitBatched(const std::shared_ptr<Shader> a_shader, const glm::mat4& a_transform)
		{
			//if (m_batchVertexCount > 0 && m_batchIndicesCount > 0)
			//{
			//	a_shader->Bind();
			//	a_shader->UploadUniformMat4("u_ProjectionView", m_sceneData->ProjectionViewMatrix);
			//	a_shader->UploadUniformMat4("u_ObjectMatrix", a_transform);
			//	
			//	m_batchArray->Bind();
			//
			//	m_batchArray->SetSubVertexData(m_batchVertex, m_batchVertexCount * sizeof(Vertex));
			//	m_batchArray->SetSubIndexData(m_batchIndices, m_batchIndicesCount);
			//
			//	RenderCommand::DrawIndexed(m_batchArray);
			//	++renderCalls;
			//
			//	a_shader->Unbind();
			//	m_batchArray->Unbind();
			//}
		}
	}
}