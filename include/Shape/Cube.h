#pragma once

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>

// DirectX 12 specific headers.
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Shape/Shape.h"

namespace Framework
{
	class Cube : public Shape
	{
	public:
		Cube();
		Cube(const float& a_size);
		~Cube();

		virtual void Translate(const glm::vec3& a_vec) override;

		void Create(const float& a_size);
	};
}