#pragma once

#include <glm/glm.hpp>

#include <glm/gtx/transform.hpp>
#include "VertexArray.h"

namespace Framework
{
	class Shape
	{
	public:
		Shape();
		~Shape();

		virtual void Translate(const glm::vec3& a_vec);
		virtual void Rotate(float a_angle, const glm::vec3& a_axis);
		virtual const glm::mat4 GetTransform() { return m_objectMatrix; }

		std::shared_ptr<VertexArray> GetVertexArray() { return m_vertexArray; }

	protected:
		std::shared_ptr<VertexArray> m_vertexArray;
		glm::mat4 m_objectMatrix;
	};
}