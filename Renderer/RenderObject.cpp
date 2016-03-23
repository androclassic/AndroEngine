#include "RenderObject.h"
#include "MaterialHelper.h"
#include "Log.h"
#include <sstream>
#include "Engine.h"
#include "../AndroUtils/Utils/Trace.h"
#include "../AndroUtils/Utils/AndroUtils.h"
#include "../External/tinyOBJ/tiny_obj_loader.h"


TakeTwo::RenderObject::RenderObject(const Material::MaterialFormat& pMaterialFormat, const char* pModelName)
	: mMesh(new Mesh)
	, mMaterial(new Material())
	, Resource(pModelName, NULL)
{
	Load(pMaterialFormat, pModelName);
}

TakeTwo::RenderObject::RenderObject(const std::string & pResource_key, void * pArgs)
	: mMesh(new Mesh)
	, mMaterial(new Material())
	, Resource(pResource_key, NULL)
{
	RenderObjectArgs* args = (RenderObjectArgs*)pArgs;
	Load(args->pMaterialFormat, args->pModelName);
}

bool TakeTwo::RenderObject::LoadModel(const char* pFilename)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(shapes, materials, err, pFilename, NULL, TRUE);

	if (!err.empty()) {
		LOG_MSG(err);
	}

	if (!ret) {
		return false;
	}


	std::vector<unsigned int> attribsUsed;
	attribsUsed.push_back(1);
	attribsUsed.push_back(1);
	attribsUsed.push_back(1);
	attribsUsed.push_back(1);

	//Read vertices and indices
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;


	unsigned int base = 0;
	for (auto shape : shapes)
	{
		bool hasNormals = shape.mesh.normals.size() ? true : false;
		bool hasTextCoord = shape.mesh.texcoords.size() ? true : false;

		if (hasNormals)
			ASSERT(shape.mesh.normals.size() == shape.mesh.positions.size());

		for (int i = 0, n = 0, t = 0; i < shape.mesh.positions.size(); i += 3, t += 2, n += 3)
		{
			Vertex  p;
			p.position = glm::vec3(shape.mesh.positions[i], shape.mesh.positions[i + 1], shape.mesh.positions[i + 2]);
			if (hasNormals)
			{
				p.normal = glm::vec3(shape.mesh.normals[n], shape.mesh.normals[n + 1], shape.mesh.normals[n + 2]);
				//p.color = glm::vec3(shape.mesh.normals[n], shape.mesh.normals[n + 1], shape.mesh.normals[n + 2]);
			}
			if (hasTextCoord)
			{
				p.texCoord = glm::vec2(shape.mesh.texcoords[t], shape.mesh.texcoords[t + 1]);
				p.color = glm::vec3(shape.mesh.texcoords[t], 0, shape.mesh.texcoords[t + 1]);
			}

			vertices.push_back(p);
		}
		for (int i = 0; i < shape.mesh.indices.size(); i++)
		{
			indices.push_back(base + shape.mesh.indices[i]);
		}

		base += (shape.mesh.positions.size() / 3);
	}

	mMesh->SetVertices(std::move(vertices));
	mMesh->SetAttribsUsed(std::move(attribsUsed));
	mMesh->SetIndices(std::move(indices));

	mMesh->Setup();

	return true;
}

bool TakeTwo::RenderObject::LoadMaterial(const Material::MaterialFormat& pMaterialFormat)
{
	//TODO COMPLETE LOAD MATERIAL

	std::vector<unsigned int> formatUsed(11);
	formatUsed[0] = 1;
	formatUsed[1] = 1;
	formatUsed[6] = 1;
	mMaterial->SetFormatUsed(std::move(formatUsed));

	Material::MaterialFormat materialFormat;
	materialFormat.textureName = pMaterialFormat.textureName;
	materialFormat.programName = pMaterialFormat.programName;

	mMaterial->SetMaterialFormat(std::move(materialFormat));
	mMaterial->Apply();

	return true;
}

bool TakeTwo::RenderObject::Load(const Material::MaterialFormat& pMaterialFormat, const char* pModelName)
{

	LoadModel(pModelName);
	LoadMaterial(pMaterialFormat);

	return true;
}

void TakeTwo::RenderObject::Render() const
{
    mMaterial->Use();
    mMesh->Render();
}



