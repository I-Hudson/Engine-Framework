#pragma once

#include "Shape/Shape.h"

namespace Framework
{
	class Cube : public Shape
	{
	public:
		Cube();
		Cube(const float& a_size);
		~Cube();

		void Create(const float& a_size);
	};
}