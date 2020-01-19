#pragma once

#include "Renderer/Material.h"

namespace Framework
{
	class OpenGLMaterial : public Renderer::Material
	{
	public:
		OpenGLMaterial();
		~OpenGLMaterial();

	protected:
		virtual void ProcessShaderUniforms() override;

	};
}