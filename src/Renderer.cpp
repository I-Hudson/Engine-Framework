#include "Renderer.h"

namespace Framework
{
	Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData();


	void Renderer::Begin(Camera& a_camera)
	{
		m_sceneData->ProjectionViewMatrix = a_camera.GetProjViewMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader> a_shader, const std::shared_ptr<VertexArray> a_vertexArray, const glm::mat4& a_transform)
	{
		a_shader->Bind();
		a_shader->UploadUniformMat4("u_ProjectionView", m_sceneData->ProjectionViewMatrix);
		a_shader->UploadUniformMat4("u_ObjectMatrix", a_transform);

		a_vertexArray->Bind();
		RenderCommand::DrawIndexed(a_vertexArray);
	}
}