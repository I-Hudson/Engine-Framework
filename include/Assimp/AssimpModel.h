#pragma once

#include "Assimp/AssimpMesh.h"
#include "Renderer/Texture.h"
#include "Renderer/Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stbi/stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class AssimpModel
{
public:
	/*
		Functions
	*/
	AssimpModel() {}
	~AssimpModel();
	AssimpModel(const char* aPath, const bool aLoadAtRuntime = false);
	AssimpModel(AssimpMesh aPath);
	AssimpModel(std::vector<AssimpVertex> aVertices, std::vector<unsigned int> aIndices);

	//draw the mesh
	void Draw();

	//destroy the model
	void Destroy();
	//get mesh count
	int getMeshCount() const { return (int)mMeshes.size(); }
	//get mesh
	AssimpMesh getMeshIndex(int a_i) const { return mMeshes[a_i]; }
	//get file name of model
	std::string* getModelFileName();

private:
	/*
		Model Data
	*/
	std::vector<AssimpMesh> mMeshes;
	std::string mDirectory;
	std::vector<std::string> mLoadedTextures;
	std::string mModelName;

	/*
		Functions
	*/
	void loadModel(const char* aPath, const bool aLoadAtRuntime);
	void processNode(aiNode* aNode, const aiScene* aScene);
	AssimpMesh processMesh(aiMesh* aMesh, const aiScene* aScene);
	std::vector<std::string> loadMaterialTextures(aiMaterial* aMaterial, aiTextureType aType,
		std::string aName);
	//load texture from file
	std::string TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
	Framework::Renderer::TextureType GetTextureType(aiTextureType type);
};