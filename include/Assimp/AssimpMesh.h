#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Renderer/VertexArray.h"
#include "Renderer/Buffer.h"
#include "Renderer/Material.h"

struct AssimpVertex : Framework::Renderer::Vertex
{
};

class AssimpMesh
{
public:
	/*
		MeshData
	*/
	std::vector<AssimpVertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::vector<std::string> mTextures;
	unsigned int mVAO;

	/*
		Functions
	*/
	AssimpMesh();
	~AssimpMesh();

	AssimpMesh(std::vector<glm::vec4> aVertices, std::vector<unsigned int> aIndices, std::vector<glm::vec2> aUV);
	AssimpMesh(std::vector<AssimpVertex> aVertices, std::vector<unsigned int> aIndices, std::vector<std::string> aTextures);
	AssimpMesh(std::vector<AssimpVertex> aVertices, std::vector<unsigned int> aIndices);

	Framework::Renderer::Material* GetMaterial() const { return m_material; }

	//Draw the mesh
	void Draw();
	//unload the mesh
	void unloadMesh();

private:
	/*
		Render Data
	*/
	unsigned int mVBO;
	unsigned int mIBO;
	std::shared_ptr<Framework::Renderer::VertexArray> m_vertexArray;

	Framework::Renderer::Material* m_material;


	/*
		Functions
	*/
	void setupMesh();
};