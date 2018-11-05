#include "Importer.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#include "Application.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma	comment (lib, "DevIL/libx86/ILU.lib")
#pragma	comment (lib, "DevIL/libx86/ILUT.lib")

Mesh::~Mesh()
{
}

GLuint Importer::LoadImageFile(const char * file)
{
	if (!is_devil_init)
	{
		ilInit();

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			LOG("Importer - GLEW initialization error: %s", glewGetErrorString(err));
			exit(-1);
		}
		LOG("Importer - GLEW intialized successfully. Current GLEW version: %s", glewGetString(GLEW_VERSION));

		is_devil_init = true;
	}

	ILuint imageID = 0;
	GLuint textureID;

	// Safe
	ILboolean success;
	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	success = ilLoadImage(file);
	if (success)
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (!success)
		{
			error = ilGetError();
			LOG("Importer - Image conversion failed! IL error: %i - %s", error, iluErrorString(error));
			return -1;
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

		image_dimensions = ImVec2(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
	}
	else
	{
		error = ilGetError();
		LOG("Importer - Image loading failed! IL error: %i - %s", error, iluErrorString(error));
		return -1;
	}

	LOG("Importer - Texture for mesh applied: %s", file);

	return textureID;
}

void myCallback(const char *msg, char *userData)
{
	LOG("Assimp - %s", msg);
}

void Importer::SetCallback()
{
	struct aiLogStream stream;
	stream.callback = myCallback;
	aiAttachLogStream(&stream);
}

void Importer::CleanCallback()
{
	aiDetachAllLogStreams();
}

std::vector<Mesh*> Importer::CreateMesh(const char * path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	std::vector<Mesh*> ret;

	if (scene != nullptr && scene->HasMeshes())
	{

		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh* mesh = new Mesh;
			
			// VERTEX
			mesh->vertex.reserve(scene->mMeshes[i]->mNumVertices);
			memcpy(mesh->vertex.data(), scene->mMeshes[i]->mVertices, sizeof(float3)*scene->mMeshes[i]->mNumVertices);

			if (mesh->vertex.empty() != false)
			{
				glGenBuffers(1, (GLuint*) &(mesh->buffer_vertex));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->buffer_vertex);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * scene->mMeshes[i]->mNumVertices, mesh->vertex.data(), GL_STATIC_DRAW);

				LOG("Importer - Loading %i vertex succesful!", scene->mMeshes[i]->mNumVertices);
			}
			else
			{
				LOG("Importer - WARNING, the scene has 0 vertex!");
			}

			if (scene->mMeshes[i]->HasNormals())
			{
				mesh->normals.reserve(scene->mMeshes[i]->mNumVertices);
				memcpy(mesh->normals.data(), scene->mMeshes[i]->mNormals, sizeof(float3)*scene->mMeshes[i]->mNumVertices);
				glGenBuffers(1, (GLuint*) &(mesh->buffer_normals));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->buffer_normals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * scene->mMeshes[i]->mNumVertices, mesh->normals.data(), GL_STATIC_DRAW);
			}

			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				float* uv = new float[scene->mMeshes[i]->mNumVertices * 2]; //BE
																			//memcpy(uv, scene->mMeshes[i]->mTextureCoords, sizeof(float)*scene->mMeshes[i]->mNumVertices);
				for (int j = 0; j < scene->mMeshes[i]->mNumVertices; ++j)
				{
					uv[j * 2] = scene->mMeshes[i]->mTextureCoords[0][j].x;
					uv[j * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][j].y;
				}

				glGenBuffers(1, (GLuint*) &(mesh->buffer_uv));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->buffer_uv);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * scene->mMeshes[i]->mNumVertices * 2, uv, GL_STATIC_DRAW);

				delete[] uv;
			}

			// INDEX
			uint* index = new uint[scene->mMeshes[i]->mNumFaces * 3];
			mesh->num_index = scene->mMeshes[i]->mNumFaces * 3;
			for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
			{
				if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
				{
					LOG("Importer - WARNING, geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, sizeof(uint) * 3);
				}
			}

			glGenBuffers(1, (GLuint*) &(mesh->buffer_index));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffer_index);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * scene->mMeshes[i]->mNumFaces * 3, index, GL_STATIC_DRAW);
			LOG("Importer - Loading %i index succesful!", (uint)scene->mMeshes[i]->mNumFaces * 3);

			delete[] index;
			
			mesh->mesh_aabb.SetNegativeInfinity();
			mesh->mesh_aabb.Enclose(mesh->vertex.data(), scene->mMeshes[i]->mNumVertices);

			// MATERIALS
			if (scene->HasMaterials())
			{
				if (scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE) > 0)
				{
					aiString texturePath; // Path to the texture, from FBX directory
					scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texturePath);
					std::string texName = texturePath.C_Str();
					std::string strPath = path;

					for (uint i = strlen(path); i >= 0; i--)
					{
						if (path[i] == '\\')
						{
							strPath.resize(i + 1);
							strPath += texName;
							break;
						}
					}
					if (strPath.c_str() != NULL)
					{
						if (FileExists(strPath.c_str()))
						{
							mesh->buffer_texture = LoadImageFile(strPath.c_str());
						}
					}
					else
					{
						LOG("Importer - ERROR Image not loaded");
					}

					if (mesh->buffer_texture == -1)
					{
						LOG("Importer - ERROR Creating buffer for Texture");
					}
				}
			}

			// ROOT TRANSFORM
			aiVector3D translation = { 0.f, 0.f, 0.f };
			aiVector3D scaling = { 0.f, 0.f, 0.f };
			aiQuaternion rotation = { 0.f, 0.f, 0.f };

			if (scene->mRootNode != nullptr)
				scene->mRootNode->mTransformation.Decompose(scaling, rotation, translation);

			mesh->transform.position = float3(translation.x, translation.y, translation.z);
			mesh->transform.scale = float3(scaling.x, scaling.y, scaling.z);
			mesh->transform.rotation = Quat(rotation.x, rotation.y, rotation.z, rotation.w);
			
			ret.push_back(mesh);
		}

		aiReleaseImport(scene);
	}
	else
		LOG("Importer - Error loading scene %s", path);

	return ret;
}

FileExtensions Importer::GetExtension(const char *path)
{
	char* ptr = &(char)path[strlen(path)];

	for (; *ptr != '.' && ptr != path; ptr--)
	{
		// You found a secret :D
	}

	ptr++;

	std::string supportedImageFormats("bmp dcx dds hdr icns ico cur iff gif jpg jpe jpeg jp2 lbm png PNG raw tif tga");

	if (supportedImageFormats.find(ptr) != std::string::npos)
	{
		return FileExtensions::Image;
	}

	std::string supported3DScenesFormats("FBX fbx OBJ obj");

	if (supported3DScenesFormats.find(ptr) != std::string::npos)
	{
		return FileExtensions::Scene3D;
	}

	return FileExtensions::Unsupported;
}

bool Importer::FileExists(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void Importer::FindAndReplace(std::string & source, std::string const & to_find, std::string const & replace)
{
	for (std::string::size_type i = 0; (i = source.find(to_find, i)) != std::string::npos;)
	{
		source.replace(i, to_find.length(), replace);
		i += replace.length();
	}
}