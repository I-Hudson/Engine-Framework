#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include <iostream>

namespace Framework
{
	Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData();
	std::unique_ptr<VertexArray> Renderer::m_batchArray = nullptr;
	Vertex* Renderer::m_batchVertex = new Vertex[65000];
	unsigned int* Renderer::m_batchIndices = new unsigned int[65000];

	int Renderer::m_vertexIndex = 0;
	int Renderer::m_indiceIndex = 0;

	unsigned int Renderer::m_batchCount = 0;

	static int renderCalls = 0;
	int Renderer::m_vertexBufferIndex = 0;

	void Renderer::Begin(Camera& a_camera)
	{
		renderCalls = 0;
		m_sceneData->ProjectionViewMatrix = a_camera.GetProjViewMatrix();

		//delete[] m_batchVertex;
		//delete[] m_batchIndices;

		m_vertexIndex = 0;
		m_indiceIndex = 0;
		m_batchCount = 0;
	}

	void Renderer::EndScene()
	{
		std::cout << "Number of render calls: " << renderCalls << "\n";
	}

	void Renderer::SubmitBatch(const std::shared_ptr<Shader> a_shader, const std::shared_ptr<VertexArray> a_vertexArray, const glm::mat4& a_transform)
	{
		memcpy(m_batchVertex + m_vertexIndex, a_vertexArray->GetVertexBuffers()[0]->GetData(), a_vertexArray->GetVertexBuffers()[0]->GetBufferSize());
		for (size_t i = 0; i < 48; ++i)
		{
			Vertex v = m_batchVertex[i];
			//std::cout << v.Position.x << "-" << v.Position.y << "-" << v.Position.z << "\n";
		}

		memcpy(m_batchIndices + m_indiceIndex, a_vertexArray->GetIndexBuffer()->GetData(), a_vertexArray->GetIndexBuffer()->GetCount() * sizeof(unsigned int));
		for (int i = 36 * m_batchCount; i < 36 * (m_batchCount + 1); ++i)
		{
			(m_batchIndices)[i] += 24 * m_batchCount;
		}

		++m_batchCount;
		m_vertexIndex += 24;
		m_indiceIndex += a_vertexArray->GetIndexBuffer()->GetCount();
	}

	void Renderer::Submit(const std::shared_ptr<Shader> a_shader, const std::shared_ptr<VertexArray> a_vertexArray, const glm::mat4& a_transform)
	{
		a_shader->Bind();
		a_shader->UploadUniformMat4("u_ProjectionView", m_sceneData->ProjectionViewMatrix);
		a_shader->UploadUniformMat4("u_ObjectMatrix", a_transform);

		//a_vertexArray->Bind();
		RenderCommand::DrawIndexed(a_vertexArray);
		++renderCalls;

		a_shader->Unbind();
		a_vertexArray->Unbind();
	}

	void Renderer::Submit(const std::shared_ptr<Shader> a_shader, const std::shared_ptr<VertexArray> a_vertexArray, std::shared_ptr<Texture> a_texture, const glm::mat4 & a_transform)
	{
		a_shader->Bind();
		a_shader->UploadUniformMat4("u_ProjectionView", m_sceneData->ProjectionViewMatrix);
		a_shader->UploadUniformMat4("u_ObjectMatrix", a_transform);
		a_shader->UploadTexture("texture1", a_texture);

		a_vertexArray->Bind();
		RenderCommand::DrawIndexed(a_vertexArray);
		++renderCalls;

		a_shader->Unbind();
		a_vertexArray->Unbind();
	}

	void Renderer::SubmitBatched(const std::shared_ptr<Shader> a_shader, const glm::mat4& a_transform)
	{
		return;
		a_shader->Bind();
		a_shader->UploadUniformMat4("u_ProjectionView", m_sceneData->ProjectionViewMatrix);
		a_shader->UploadUniformMat4("u_ObjectMatrix", a_transform);
		
		std::shared_ptr<OpenGLVertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>(m_batchVertex, sizeof(Vertex) * m_vertexIndex);
		BufferLayout layout =
		{
			{ShaderDataType::Float4, "inPosition"},
			{ShaderDataType::Float4, "inColor", true},
			{ShaderDataType::Float4, "inNormal", true},
			{ShaderDataType::Float2, "inUV", true},
		};
		vertexBuffer->SetLayout(layout);
		
		std::shared_ptr<OpenGLIndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>(m_batchIndices, m_indiceIndex);
		
		std::shared_ptr<OpenGLVertexArray> vertexArray = std::make_shared<OpenGLVertexArray>();
		vertexArray->AddVertexBuffer(vertexBuffer);
		vertexArray->AddIndexBuffer(indexBuffer);
		
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
		++renderCalls;
		
		a_shader->Unbind();
		vertexArray->Unbind();
	}
}