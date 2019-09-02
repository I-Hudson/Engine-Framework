#include "Shape/Cube.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Framework
{
	Cube::Cube()
	{
		Create(1.0f);
	}

	Cube::Cube(const float& a_size)
	{
		Create(a_size);
	}

	Cube::~Cube()
	{
	}

	void Cube::Create(const float& a_size)
	{
		// Setup the vertex buffer
		float vertices[] =
		{
			//FRONT
			-a_size, -a_size, a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0, 0, 1, 0, 0, 0, //0
			-a_size, a_size, a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, 0,0,1,0, 0, 0,
			a_size, a_size, a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,0,1,0, 0, 0,
			a_size, -a_size, a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, 0,0,1,0, 0, 0,

			//RIGHT																	    
			a_size, -a_size, a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, 1,0,0,0, 0, 0, //4
			a_size, a_size, a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1,0,0,0, 0, 0,
			a_size, a_size, -a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, 1,0,0,0, 0, 0,
			a_size, -a_size, -a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1,0,0,0, 0, 0,

			//BACK
			-a_size, -a_size, -a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,0,-1,0, 0, 0, //8
			-a_size, a_size, -a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, 0,0,-1,0, 0, 0,
			a_size, a_size, -a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,0,-1,0, 0, 0,
			a_size, -a_size, -a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, 0,0,-1,0, 0, 0,

			//LEFT
			-a_size, -a_size, -a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1,0,0,0, 0, 0, //12
			-a_size, a_size, -a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, -1,0,0,0, 0, 0,
			-a_size, a_size, a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1,0,0,0, 0, 0,
			-a_size, -a_size, a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, -1,0,0,0, 0, 0,

			//TOP
			-a_size, a_size, a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,1,0,0, 0, 0, //16
			-a_size, a_size, -a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, 0,1,0,0, 0, 0,
			a_size, a_size, -a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,1,0,0, 0, 0,
			a_size, a_size, a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,1,0,0, 0, 0,

			//BOTTOM
			-a_size, -a_size, a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,-1,0,0, 0, 0, //20
			-a_size, -a_size, -a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, 0,-1,0,0, 0, 0,
			a_size, -a_size, -a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,-1,0,0, 0, 0,
			a_size, -a_size, a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,-1,0,0, 0, 0
		};
		std::shared_ptr<OpenGLVertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>(vertices, (unsigned int)sizeof(vertices));
		BufferLayout layout =
		{
			{ShaderDataType::Float4, "inPosition"},
			{ShaderDataType::Float4, "inColor", true},
			{ShaderDataType::Float4, "inNormal", true},
			{ShaderDataType::Float2, "inUV", true},
		};
		vertexBuffer->SetLayout(layout);

		unsigned int indices[] =
		{
			0,2,1,
			0,3,2,
			//
			4,6,5,
			4,7,6,
			//
			11,9,10,
			11,8,9,
			//
			12,14,13,
			12,15,14,
			//
			16,18,17,
			16,19,18,
			//
			21,23,20,
			21,22,23
		};
		std::shared_ptr<OpenGLIndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

		m_vertexArray = std::make_shared<OpenGLVertexArray>();
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->AddIndexBuffer(indexBuffer);
	}
}