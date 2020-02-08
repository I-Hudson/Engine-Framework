#include "Assimp/AssimpModel.h"
#include "Log.h"

#include "Application.h"

//#include <thread>

AssimpModel::~AssimpModel()
{
}

AssimpModel::AssimpModel(const char* aPath, const bool aLoadAtRuntime)
{
	//std::thread t1(&AssimpModel::loadModel, aPath);
	//load model
	loadModel(aPath, aLoadAtRuntime);
	//t1.join();
}

AssimpModel::AssimpModel(AssimpMesh aPath)
{
	//add a new mesh
	mMeshes.push_back(aPath);
}

AssimpModel::AssimpModel(std::vector<AssimpVertex> aVertices, std::vector<unsigned int> aIndices)
{
	//add a new mesh
	AssimpMesh mesh(aVertices, aIndices);
	mMeshes.push_back(mesh);
}

void AssimpModel::Draw()
{
	//draw all meshes
	for (unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i].Draw();
	}
}

void AssimpModel::Destroy()
{
	//destroy all meshes
	for (unsigned int i = 0; i < mMeshes.size(); i++)
	{
		mMeshes[i].unloadMesh();
	}
	EN_TRACE("Model Has Been Unloaded");
}

std::string* AssimpModel::getModelFileName()
{
	//get model file name
	return &mDirectory;
}

void AssimpModel::loadModel(const char* aPath, const bool aLoadAtRuntime)
{
	//get the assimp importer, load the model from file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(aPath, aiProcess_Triangulate |
													aiProcess_FlipUVs | 
													aiProcess_CalcTangentSpace |
													aiProcess_GenUVCoords);

	if (!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	if (!aLoadAtRuntime)
	{
		mDirectory = std::string(aPath).substr(0, std::string(aPath).find_last_of('/'));
	}
	else
	{
		mDirectory = std::string(aPath).substr(0, std::string(aPath).find_last_of('\\'));
	}
	//process scene root node
	processNode(scene->mRootNode, scene);
}

void AssimpModel::processNode(aiNode* aNode, const aiScene* aScene)
{
	//process the node's meshes 
	for (unsigned int i = 0; i < aNode->mNumMeshes; i++)
	{
		aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
		mMeshes.push_back(processMesh(mesh, aScene));
	}
	//process the node's children 
	for (unsigned int i = 0; i < aNode->mNumChildren; i++)
	{
		processNode(aNode->mChildren[i], aScene);
	}
}

AssimpMesh AssimpModel::processMesh(aiMesh* aMesh, const aiScene* aScene)
{
	std::vector<AssimpVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::string> textures;

	/*
		Vertices
	*/
	for (unsigned int i = 0; i < aMesh->mNumVertices; ++i)
	{
		AssimpVertex vertex;

		//process vertex
		glm::vec4 position;
		position.x = aMesh->mVertices[i].x;
		position.y = aMesh->mVertices[i].y;
		position.z = aMesh->mVertices[i].z;
		position.w = 1;
		vertex.Position = position;

		glm::vec4 color = glm::vec4();
		color.r = 1.0f;
		color.g = 1.0f;
		color.b = 1.0f;
		color.a = 1.0f;
		vertex.Color = color;

		glm::vec4 normal = glm::vec4();
		if (aMesh->mNormals != nullptr)
		{
			normal.x = aMesh->mNormals[i].x;
			normal.y = aMesh->mNormals[i].y;
			normal.z = aMesh->mNormals[i].z;
			normal.w = 0;
		}
		vertex.Normal = normal;

		//glm::vec4 tangent = glm::vec4();
		//if(aMesh->mTangents != nullptr)
		//{
		//	tangent.x = aMesh->mTangents[i].x;
		//	tangent.y = aMesh->mTangents[i].y;
		//	tangent.z = aMesh->mTangents[i].z;
		//	tangent.w = 0;
		//}
		//vertex.mTangent = tangent;
		//
		//glm::vec4 bitangent = glm::vec4();
		//if (aMesh->mBitangents != nullptr)
		//{
		//	bitangent.x = aMesh->mBitangents[i].x;
		//	bitangent.y = aMesh->mBitangents[i].y;
		//	bitangent.z = aMesh->mBitangents[i].z;
		//	bitangent.w = 0;
		//}
		//vertex.mBitangent = bitangent;

		if (aMesh->mTextureCoords[0])
		{
			glm::vec2 texCoord;
			texCoord.x = aMesh->mTextureCoords[0][i].x;
			texCoord.y = aMesh->mTextureCoords[0][i].y;
			vertex.UV = texCoord;
		}
		else
		{
			vertex.UV = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	/*
		Indices
	*/
	for (unsigned int i = 0; i < aMesh->mNumFaces; ++i)
	{
		aiFace face = aMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	/*
		Material
	*/
	if (aMesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = aScene->mMaterials[aMesh->mMaterialIndex];
		std::vector<std::string> diffuseMaps = loadMaterialTextures(material, 
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<std::string> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<std::string> normalMaps = loadMaterialTextures(material,
			aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<std::string> heightMap = loadMaterialTextures(material,
			aiTextureType_HEIGHT, "texture_height");
		textures.insert(textures.end(), heightMap.begin(), heightMap.end());
	}

	return AssimpMesh(vertices, indices, textures);
}

std::vector<std::string> AssimpModel::loadMaterialTextures(aiMaterial* aMaterial, aiTextureType aType, std::string aName)
{
	//load a texture
	std::vector<std::string> textures;
	for (unsigned int i = 0; i < aMaterial->GetTextureCount(aType); ++i)
	{
		aiString str;
		//get texture index and string
		aMaterial->GetTexture(aType, i, &str);
		bool skip = false;
		//if we have already loaded the texture, push it back and set skip to true
		for (unsigned int j = 0; j < mLoadedTextures.size(); ++j)
		{
			if (std::strcmp(mLoadedTextures[j].c_str(), str.C_Str()) == 0)
			{
				mLoadedTextures.push_back(mLoadedTextures[j]);
				skip = true;
				break;
			}
		}
		//if texture has been loaded skip
		if (!skip)
		{
			//load texture from file
			std::string texture;
			texture = TextureFromFile(str.C_Str(), mDirectory);
			//Framework::Application::Get().GetTextureLibrary().Load(texture, texture, GetTextureType(aType));
			textures.push_back(texture);
		}
	}
	return textures;
}

std::string AssimpModel::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	//texture filename
	int lastSlash = (int)std::string(path).find_last_of('\\');
	int pathLength = (int)std::string(path).length();

	std::string filename = std::string(path).substr(lastSlash + 1, pathLength - lastSlash); //std::string(path);
	filename = directory + '/' + filename;
	return filename;
}

Framework::Renderer::TextureType AssimpModel::GetTextureType(aiTextureType type)
{
	switch (type)
	{
	case aiTextureType_DIFFUSE:
		return Framework::Renderer::TextureType::DIFFUSE;
	case aiTextureType_SPECULAR:
		return Framework::Renderer::TextureType::SPECULAR;
	case aiTextureType_AMBIENT:
		return Framework::Renderer::TextureType::AMBIENT;
	case aiTextureType_EMISSIVE:
		return Framework::Renderer::TextureType::EMISSIVE;
	case aiTextureType_HEIGHT:
		return Framework::Renderer::TextureType::HEIGHT;
	case aiTextureType_NORMALS:
		return Framework::Renderer::TextureType::NORMALS;
	case aiTextureType_SHININESS:
		return Framework::Renderer::TextureType::SHININESS;
	case aiTextureType_OPACITY:
		return Framework::Renderer::TextureType::OPACITY;
	case aiTextureType_DISPLACEMENT:
		return Framework::Renderer::TextureType::DISPLACEMENT;
	case aiTextureType_LIGHTMAP:
		return Framework::Renderer::TextureType::LIGHTMAP;
	case aiTextureType_REFLECTION:
		return Framework::Renderer::TextureType::REFLECTION;
	default:
		break;
	}
	return Framework::Renderer::TextureType::UNKNOWN;
}
