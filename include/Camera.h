#pragma once

#include <glm/glm.hpp>

namespace Framework
{
	///
	// BASE CAMERA
	///	
	class Camera
	{
	public:
		Camera();
		~Camera();

		//Set the view matrix for the camera
		void SetViewMatrix(const glm::mat4& a_value);
		//Set the projection matrix for the camera
		void SetProjMatrix(const glm::mat4& a_value);
		void SetProjMatrix(const float& a_fov, const float& a_aspect, const float& a_near, const float& a_far);

		//Set the position for the camera
		void SetPosition(const glm::vec3& a_value);

		void Update(float a_deltaTime);

		//Get the projection view matrix
		const glm::mat4& GetProjViewMatrix();

	private:

		void SetProjectionViewMatrix();

		glm::mat4 m_projectionViewMatrix;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;

	};
}