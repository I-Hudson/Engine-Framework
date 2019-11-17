#include "Shape/Cube.h"

#include "Platform/DirectX/DirectXBuffer.h"
#include "Platform/DirectX/DirectXVertexArray.h"

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

	void Cube::Translate(const glm::vec3& a_vec)
	{
		//for (size_t i = 0; i < m_vertexArray->GetVertexBuffers()[0]->GetBufferSize() / sizeof(Vertex); i++)
		//{
		//	m_vertexArray->GetVertexBuffers()[0]->GetData()[i].Position += glm::vec4(a_vec, 1.0f);
		//}

		Shape::Translate(a_vec);
	}

	void Cube::Create(const float& a_size)
	{
		// Setup the vertex buffer
		Vertex vertices[] =
		{
			//FRONT
			Vertex({-a_size, -a_size, a_size, 1.0f}, {1.0f, 1.0f, 1.0f , 1.0f}, {0, 0, 1, 0 }, { 0, 0 }), //0
			Vertex({-a_size, a_size, a_size, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0,0,1,0}, {0, 1}),
			Vertex({a_size, a_size, a_size, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 1, 0}, {1, 1}),
			Vertex({a_size, -a_size, a_size, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 1, 0}, {1, 0}),

			//RIGHT																	    
			Vertex(a_size, -a_size, a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, 1,0,0,0, 0, 0), //4
			Vertex(a_size, a_size, a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1,0,0,0, 0, 1),
			Vertex(a_size, a_size, -a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, 1,0,0,0, 1, 1),
			Vertex(a_size, -a_size, -a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1,0,0,0, 1, 0),

			//BACK
			Vertex(-a_size, -a_size, -a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,0,-1,0, 1, 0), //8
			Vertex(-a_size, a_size, -a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, 0,0,-1,0, 1, 1),
			Vertex(a_size, a_size, -a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,0,-1,0, 0, 1),
			Vertex(a_size, -a_size, -a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, 0,0,-1,0, 0, 0),

			//LEFT
			Vertex(-a_size, -a_size, -a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1,0,0,0, 0, 0), //12
			Vertex(-a_size, a_size, -a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, -1,0,0,0, 0, 1),
			Vertex(-a_size, a_size, a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1,0,0,0, 1, 1),
			Vertex(-a_size, -a_size, a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, -1,0,0,0, 1, 0),

			//TOP
			Vertex(-a_size, a_size, a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,1,0,0, 0, 0), //16
			Vertex(-a_size, a_size, -a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, 0,1,0,0, 0, 1),
			Vertex(a_size, a_size, -a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,1,0,0, 1, 1),
			Vertex(a_size, a_size, a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,1,0,0, 1, 0),

			//BOTTOM
			Vertex(-a_size, -a_size, a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,-1,0,0, 0, 0), //20
			Vertex(-a_size, -a_size, -a_size, 1.0f,1.0f, 1.0f, 1.0f, 1.0f, 0,-1,0,0, 0, 1),
			Vertex(a_size, -a_size, -a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,-1,0,0, 1, 1),
			Vertex(a_size, -a_size, a_size, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0,-1,0,0, 1, 0)
		};
			// Setup the vertex buffer
		//	VertexDX vertices[] =
		//	{
		//	//FRONT
		//	VertexDX(-a_size, -a_size, a_size, 1.0f, 1.0f, 1.0f), //0
		//	VertexDX(-a_size, a_size, a_size, 1.0f,1.0f, 1.0f),
		//	VertexDX(a_size, a_size, a_size, 1.0f, 1.0f, 1.0f),
		//	VertexDX(a_size, -a_size, a_size, 1.0f,1.0f, 1.0f),
		//
		//	//RIGHT																	    
		//	VertexDX(a_size, -a_size, a_size, 1.0f,1.0f, 1.0f), //4
		//	VertexDX(a_size, a_size, a_size, 1.0f, 1.0f, 1.0f),
		//	VertexDX(a_size, a_size, -a_size, 1.0f,1.0f, 1.0f),
		//	VertexDX(a_size, -a_size, -a_size, 1.0f, 1.0f, 1.0f),
		//
		//	//BACK
		//	VertexDX(-a_size, -a_size, -a_size, 1.0f, 1.0f, 1.0f), //8
		//	VertexDX(-a_size, a_size, -a_size, 1.0f,1.0f, 1.0f),
		//	VertexDX(a_size, a_size, -a_size, 1.0f, 1.0f, 1.0f),
		//	VertexDX(a_size, -a_size, -a_size, 1.0f,1.0f, 1.0f),
		//
		//	//LEFT
		//	VertexDX(-a_size, -a_size, -a_size, 1.0f, 1.0f, 1.0f), //12
		//	VertexDX(-a_size, a_size, -a_size, 1.0f,1.0f, 1.0f),
		//	VertexDX(-a_size, a_size, a_size, 1.0f, 1.0f, 1.0f),
		//	VertexDX(-a_size, -a_size, a_size, 1.0f,1.0f, 1.0f),
		//
		//	//TOP
		//	VertexDX(-a_size, a_size, a_size, 1.0f, 1.0f, 1.0f), //16
		//	VertexDX(-a_size, a_size, -a_size, 1.0f,1.0f, 1.0f),
		//	VertexDX(a_size, a_size, -a_size, 1.0f, 1.0f, 1.0f),
		//	VertexDX(a_size, a_size, a_size, 1.0f, 1.0f, 1.0f),
		//
		//	//BOTTOM
		//	VertexDX(-a_size, -a_size, a_size, 1.0f, 1.0f, 1.0f), //20
		//	VertexDX(-a_size, -a_size, -a_size, 1.0f,1.0f, 1.0f),
		//	VertexDX(a_size, -a_size, -a_size, 1.0f, 1.0f, 1.0f),
		//	VertexDX(a_size, -a_size, a_size, 1.0f, 1.0f, 1.0f)
		//};
		std::shared_ptr<OpenGLVertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>(vertices, sizeof(vertices));
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