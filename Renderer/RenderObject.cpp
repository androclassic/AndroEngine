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
	, m_octree(NULL)
	, Resource(pModelName, NULL)
{
	Load(pMaterialFormat, pModelName);
}

TakeTwo::RenderObject::RenderObject(const std::string & pResource_key, void * pArgs)
	: mMesh(new Mesh)
	, mMaterial(new Material())
	, mMeshOctree(new Mesh)
	, m_octree(NULL)
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
	std::vector<unsigned int> attribsUsed2;
	attribsUsed.push_back(1);
	attribsUsed.push_back(1);
	attribsUsed.push_back(1);
	attribsUsed.push_back(1);
	attribsUsed2 = attribsUsed;

	//Read vertices and indices
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	andro::BoundingBox bbx;
	bbx.max = andro::Vector3(-999, -999, -999);
	bbx.min = andro::Vector3(999, 999, 999);

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

			bbx.min.x = std::min(shape.mesh.positions[i    ], bbx.min.x);
			bbx.min.y = std::min(shape.mesh.positions[i + 1], bbx.min.y);
			bbx.min.z = std::min(shape.mesh.positions[i + 2], bbx.min.z);

			bbx.max.x = std::max(shape.mesh.positions[i],     bbx.max.x);
			bbx.max.y = std::max(shape.mesh.positions[i + 1], bbx.max.y);
			bbx.max.z = std::max(shape.mesh.positions[i + 2], bbx.max.z);

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
	mMesh->SetupFaces();


	//build octree 
	std::vector<andro::Triangle*> triangles;
	triangles.resize(mMesh->m_faces.size());
	for (unsigned int i = 0; i < triangles.size(); i++)
		triangles[i] = &mMesh->m_faces[i];

	const int step = 5;
	m_octree = andro::BuildOctree<andro::Triangle*>(triangles, bbx, step, [](const andro::BoundingBox& box, andro::Triangle* t)	{ return TriangleBoxOverlap(box, *t); });


	//build octree mesh
	std::vector<Vertex> vertices_octree;
	std::vector<unsigned int> indices_octree;

	fillVerticesFromOctree(vertices_octree, indices_octree, m_octree, step);
	mMeshOctree->SetVertices(std::move(vertices_octree));
	mMeshOctree->SetAttribsUsed(std::move(attribsUsed2));
	mMeshOctree->SetIndices(std::move(indices_octree));
	mMeshOctree->Setup();

	return true;
}



bool TakeTwo::RenderObject::LoadMaterial(const Material::MaterialFormat& pMaterialFormat)
{
	//TODO COMPLETE LOAD MATERIAL

	std::vector<unsigned int> formatUsed(Material::MaterialFormatEnum::Count);
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

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	mMeshOctree->Render();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}


void TakeTwo::RenderObject::fillVerticesFromOctree(std::vector<TakeTwo::Vertex>& vertices, std::vector<unsigned int>& indices, andro::OctreeNode<andro::Triangle*>* node, int step)
{
	if (node == NULL)
		return;

	step--;
	bool is_leaf = step == 0;

	if (is_leaf)
	{


		andro::Vector3 points[8];
		unsigned int cube_indices[36];
		andro::CreateCubeModel(node->m_bounds, points, cube_indices);

		for (int i = 0; i < 8; i++)
		{
			TakeTwo::Vertex v;
			v.position.x = points[i].x;
			v.position.y = points[i].y;
			v.position.z = points[i].z;
			vertices.push_back(v);
		}

		int base = vertices.size();
		for (int i = 0; i < 36; i++)
			indices.push_back(base + cube_indices[i]);
	}
	for (int i = 0; i < 8; i++)
		fillVerticesFromOctree(vertices, indices, node->m_children[i], step);

}




