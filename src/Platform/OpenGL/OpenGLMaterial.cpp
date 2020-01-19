#include "Platform/OpenGL/OpenGLMaterial.h"

namespace Framework
{
	OpenGLMaterial::OpenGLMaterial()
	{
	}

	OpenGLMaterial::~OpenGLMaterial()
	{
	}

	void OpenGLMaterial::ProcessShaderUniforms()
	{
		int uniformCount = 0;
		glGetProgramiv(m_shader->GetProgramId(), GL_ACTIVE_UNIFORMS, &uniformCount);

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 16; // maximum name length
		GLchar name[bufSize]; // variable name in GLSL
		GLsizei length; // name length
		for (size_t i = 0; i < uniformCount; i++)
		{
			glGetActiveUniform(m_shader->GetProgramId(), i, bufSize, &length, &size, &type, name);
			printf("Uniform #%d Type: %u Name: %s\n", i, type, name);

			switch (type)
			{
				case GL_FLOAT:
				SetFloat(name, 0.0f);
					break;
				case GL_INT:
					SetInt(name, 0);
					break;
				case GL_BOOL:
					SetBool(name, false);
					break;

				case GL_FLOAT_VEC2:
					SetVec2(name, glm::vec2());
					break;
				case GL_FLOAT_VEC3:
					SetVec3(name, glm::vec3());
					break;
				case GL_FLOAT_VEC4:
					SetVec4(name, glm::vec4());
					break;

				case GL_FLOAT_MAT2:
					SetMat2(name, glm::mat2());
					break;
				case GL_FLOAT_MAT3:
					SetMat3(name, glm::mat3());
					break;
				case GL_FLOAT_MAT4:
					SetMat4(name, glm::mat4());
					break;

				case GL_SAMPLER_2D:
					SetTexture(name, nullptr, 0);
					break;
			default:
				break;
			}
		}
	}
}