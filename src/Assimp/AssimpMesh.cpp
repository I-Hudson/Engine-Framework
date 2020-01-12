#include "Assimp/AssimpMesh.h"
#include "Log.h"

#include "Application.h"
#include "Shader.h"
#include "Renderer.h"

AssimpMesh::AssimpMesh()
{
}

AssimpMesh::~AssimpMesh()
{
}

AssimpMesh::AssimpMesh(std::vector<glm::vec4> aVertices, std::vector<unsigned int> aIndices, std::vector<glm::vec2> aUV)
{
	std::vector <AssimpVertex> vertices;
	vertices.reserve(aVertices.size());

	for (unsigned int i = 0; i < aVertices.size(); ++i)
	{
		AssimpVertex vertex;
		glm::vec4 vector;

		//position
		vector = aVertices[i];
		vertex.Position = vector;

		//colour
		vector = glm::vec4(1, 1, 1, 1);
		vertex.Color = vector;

		//normal
		vector = glm::vec4(1, 1, 1, 1);
		vertex.Normal = vector;

		//tangent
		//vector = glm::vec4(1, 1, 1, 1);
		//vertex.mTangent = vector;

		//bitangent
		//vector = glm::vec4(1, 1, 1, 1);
		//vertex.mBitangent = vector;

		//uv
		glm::vec2 uv = glm::vec2(aUV[i].x, aUV[i].y);
		vertex.UV = uv;

		vertices.push_back(vertex);

	}

	this->mVertices = vertices;
	this->mIndices = aIndices;

	setupMesh();
}

AssimpMesh::AssimpMesh(std::vector<AssimpVertex> aVertices, std::vector<unsigned int> aIndices, std::vector<std::string> aTextures)
{
	this->mVertices = aVertices;
	this->mIndices = aIndices;
	this->mTextures = aTextures;

	//set the vertex buffers
	setupMesh();
}

AssimpMesh::AssimpMesh(std::vector<AssimpVertex> aVertices, std::vector<unsigned int> aIndices)
{
	std::vector <AssimpVertex> vertices;
	vertices.reserve(aVertices.size());

	for (unsigned int i = 0; i < aVertices.size(); ++i)
	{
		//push back new vertex
		AssimpVertex vertex;	
		vertex = aVertices[i];
		vertices.push_back(vertex);
	}

	this->mVertices = vertices;
	this->mIndices = aIndices;

	//set the vertex buffers
	setupMesh();
}

void AssimpMesh::Draw()
{
	//unsigned int diffuseNr = 1;
	//unsigned int specularNr = 1;
	//unsigned int normalNr = 1;
	//unsigned int heightNr = 1;
	//if (aBindTextures)
	//{
		//for (unsigned int i = 0; i < mTextures.size(); ++i)
		//{
	//		//active proper texture unit before binding
	//		glActiveTexture(GL_TEXTURE0 + i);
	//
	//		std::string number;
	//		std::string name = mTextures[i].mType;
	//		if (name == "texture_diffuse")
	//		{
	//			number = std::to_string(diffuseNr++);
	//		}
	//		else if (name == "texture_specular")
	//		{
	//			number = std::to_string(specularNr++); // transfer unsigned int to stream
	//		}
	//		else if (name == "texture_normal")
	//		{
	//			number = std::to_string(normalNr++); // transfer unsigned int to stream
	//		}
	//		else if (name == "texture_height")
	//		{
	//			number = std::to_string(heightNr++); // transfer unsigned int to stream
	//		}
	//
	//		if (aShader != nullptr)
	//		{
	//			//bind texture
	//			unsigned int loc = glGetUniformLocation(aShader->getShader(), (name).c_str());
	//			glUniform1i(loc, i);
	//			glBindTexture(GL_TEXTURE_2D, mTextures[i].mTextureID);
	//		}
		//}
	//}
	//
	////Draw Mesh
	//glBindVertexArray(mVAO);
	//glDrawElements(aDrawMode, (GLsizei)mIndices.size(), GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
	//
	//// always good practice to set everything back to defaults once configured.
	//glActiveTexture(GL_TEXTURE0);

	Framework::Renderer::Submit(Framework::Application::Get().GetShaderLibrary().GetShader("demoShader"), m_vertexArray, mTextures);
}

void AssimpMesh::setupMesh()
{
	std::shared_ptr<Framework::VertexBuffer> vb;
	vb.reset(Framework::VertexBuffer::Create(&mVertices[0], sizeof(AssimpVertex) * mVertices.size()));
	
	Framework::BufferLayout layout =
	{
			{Framework::ShaderDataType::Float4, "inPosition"},
			{Framework::ShaderDataType::Float4, "inColor", true},
			{Framework::ShaderDataType::Float4, "inNormal", true},
			{Framework::ShaderDataType::Float2, "inUV", true},
	};
	vb->SetLayout(layout);

	std::shared_ptr<Framework::IndexBuffer> ib;
	ib.reset(Framework::IndexBuffer::Create(&mIndices[0], mIndices.size()));

	m_vertexArray.reset(Framework::VertexArray::Create());
	m_vertexArray->AddVertexBuffer(vb);
	m_vertexArray->AddIndexBuffer(ib);
}

void AssimpMesh::unloadMesh()
{
	//delete buffers
	m_vertexArray.reset();
	
	//delete all textures
	mTextures.clear();
}
