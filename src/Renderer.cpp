#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include <iostream>

namespace Framework
{
	Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData();
	std::unique_ptr<VertexArray> Renderer::m_batchArray = nullptr;

	static int renderCalls = 0;
	int Renderer::m_vertexBufferIndex = 0;

	void Renderer::Begin(Camera& a_camera)
	{
		renderCalls = 0;
		m_sceneData->ProjectionViewMatrix = a_camera.GetProjViewMatrix();

		//if (m_batchArray == nullptr)
		//{
		//	Renderer::m_batchArray = std::make_unique<OpenGLVertexArray>();
		//
		//	BufferLayout layout =
		//	{
		//		{ShaderDataType::Float4, "inPosition"},
		//		{ShaderDataType::Float4, "inColor", true},
		//		{ShaderDataType::Float4, "inNormal", true},
		//		{ShaderDataType::Float2, "inUV", true},
		//	};
		//
		//	uint32_t size = (uint32_t)4096 * layout.GetStride();
		//	float* vertices = new float[size];
		//
		//	memset(vertices, 0, size);
		//	std::shared_ptr<OpenGLVertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>(vertices, size);
		//	vertexBuffer->SetLayout(layout);
		//
		//	size = (uint32_t)4096 * 36;
		//	unsigned int* indices = new unsigned int[size];
		//	std::shared_ptr<OpenGLIndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>(indices, size);
		//
		//	m_batchArray->AddVertexBuffer(vertexBuffer);
		//	m_batchArray->AddIndexBuffer(indexBuffer);
		//
		//	delete[] vertices;
		//	delete[] indices;
		//
		//	m_vertexBufferIndex = 0;
		//}
		//else
		//{
		//	float vertices[] =
		//	{
		//		1.0f, 1.0f, 1.0f, 1.0f
		//	};
		//	m_batchArray->GetVertexBuffers()[0]->SetData(vertices, 0, 2);
		//}
	}

	void Renderer::EndScene()
	{
		std::cout << "Number of render calls: " << renderCalls << "\n";
	}

	void Renderer::SubmitBatch(const std::shared_ptr<Shader> a_shader, const std::shared_ptr<VertexArray> a_vertexArray, const glm::mat4& a_transform)
	{
		//float* data = nullptr;
		//a_vertexArray->GetVertexBuffers()[0]->GetData(data, 0, 1344);
		//delete[] data;
		//
		//m_batchArray->GetVertexBuffers()[0]->SetData(data, m_vertexBufferIndex, m_vertexBufferIndex + a_vertexArray->GetVertexBuffers()[0]->GetBufferSize());
		//
		//m_vertexBufferIndex += a_vertexArray->GetVertexBuffers()[0]->GetBufferSize();


	}

	void Renderer::Submit(const std::shared_ptr<Shader> a_shader, const std::shared_ptr<VertexArray> a_vertexArray, const glm::mat4& a_transform)
	{
		a_shader->Bind();
		a_shader->UploadUniformMat4("u_ProjectionView", m_sceneData->ProjectionViewMatrix);
		a_shader->UploadUniformMat4("u_ObjectMatrix", a_transform);

		a_vertexArray->Bind();
		RenderCommand::DrawIndexed(a_vertexArray);
		++renderCalls;

		a_shader->Unbind();
		a_vertexArray->Unbind();
	}
}