#pragma once

#include "Mesh.h"
#include "Material.h"
#include <string>
#include <fstream>
#include <vector>
#include "../AndroUtils/Utils/Resource.h"
#include "Octree.h"

namespace TakeTwo
{
    class RenderObject : public andro::Resource
    {
		friend class  andro::ResourceManager;

    public:
        RenderObject(const Material::MaterialFormat& pMaterialFormat, const char* pModelName);

		bool Load(const Material::MaterialFormat& pMaterialFormat, const char* pModelName);
        void Render() const;

        Mesh& GetMesh() const { return *mMesh; }
        Material& GetMaterial() const { return *mMaterial; }


    private:
		RenderObject(const std::string& pResource_key, void *args);

		bool LoadModel(const char* pModelName);
		bool LoadMaterial(const Material::MaterialFormat& pMaterialFormat);

		void fillVerticesFromOctree(std::vector<TakeTwo::Vertex>& vertices, std::vector<unsigned int>& indices, andro::OctreeNode<andro::Triangle*>* node, int step);


        std::unique_ptr<Mesh> mMesh;
        std::unique_ptr<Material> mMaterial;
		
		andro::OctreeNode<andro::Triangle*>* m_octree;
		std::unique_ptr<Mesh> mMeshOctree;

	public:
		struct RenderObjectArgs //used for ResourceManager
		{
			RenderObjectArgs(const Material::MaterialFormat& pMF, const char* pMN) : pMaterialFormat(pMF), pModelName(pMN)
			{}

			const Material::MaterialFormat& pMaterialFormat;
			const char* pModelName;
		};

    };
}
