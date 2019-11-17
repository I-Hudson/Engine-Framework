#include "Shape/Shape.h"

namespace Framework
{
	Shape::Shape()
		: m_objectMatrix(glm::mat4())
	{
	}

	Shape::~Shape()
	{
	}

	void Shape::Translate(const glm::vec3& a_vec)
	{
		m_objectMatrix[3] += glm::vec4(a_vec, 0.0f);
	}

	void Shape::Rotate(float a_angle, const glm::vec3& a_axis)
	{
		m_objectMatrix = glm::rotate(m_objectMatrix, a_angle, a_axis);
	}
}