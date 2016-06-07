#pragma once

#include "Mesh.h"
#include "Material.h"
#include <string>
#include <fstream>
#include <vector>
#include "../AndroUtils/Utils/Resource.h"
#include "../AndroUtils/Utils/Octree.h"

#define ITEM_OPAQUE		  0x0000001
#define ITEM_TRANSLUCENT  0x0000002
#define ITEM_POLYGON	  0x0000004


namespace TakeTwo
{
    class RenderObject : public andro::Resource
    {
		friend class  andro::ResourceManager;

    public:
        void Render() const;

        Mesh& GetMesh() const { return *mMesh; }
        Material& GetMaterial() const { return *mMaterial; }

		andro::OctreeNode<andro::Triangle*>* m_octree; //TODO

		inline void SetFlags(unsigned int flags) { mFlags = flags; }
		inline unsigned int GetFlags() const { return mFlags; }

    private:
		RenderObject(const std::string& pResource_key, void *args);

		bool LoadModel(const char* pModelName);
		bool BuildModelFromOctree(andro::OctreeNode<andro::Triangle*>* pOctree);
		bool LoadMaterial(const Material::MaterialFormat& pMaterialFormat);

		void fillVerticesFromOctree(std::vector<TakeTwo::Vertex>& vertices, std::vector<unsigned int>& indices, andro::OctreeNode<andro::Triangle*>* node, int step);


        std::unique_ptr<Mesh> mMesh;
        std::unique_ptr<Material> mMaterial;
		unsigned int mFlags;

	public:
		struct RenderObjectArgs //used for ResourceManager
		{
			const Material::MaterialFormat* pMaterialFormat;
			const char* pModelName; //use this to load from file
			andro::OctreeNode<andro::Triangle*>* pOctree; //use this to create from octree
		};

    };
}
