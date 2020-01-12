#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "VertexArray.h"
#include "Buffer.h"

struct AssimpVertex : Framework::Vertex
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
	std::shared_ptr<Framework::VertexArray> m_vertexArray;


	/*
		Functions
	*/
	void setupMesh();
};