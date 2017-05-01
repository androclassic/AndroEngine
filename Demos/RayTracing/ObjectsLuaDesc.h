#pragma once
#include "Material.h"
#include "AndroUtils/Utils/Octree.h"
#include "AndroUtils/Utils/Ray.h"
#include "AndroUtils/Utils/Shapes.h"
#include "AndroUtils/Introspection/LuaState.h"

//////////////////////////////////////////////////////////////////////////////////////


enum ObjectType
{
	OBJ_Box,
	OBJ_Sphere,
	OBJ_RectObjectXY,
	OBJ_RectObjectYZ,
	OBJ_RectObjectXZ,
	OBJ_BoxFromPlanes,
	ObjectType_SIZE
};

enum TextureType
{
	Tex_Constant,
	Tex_Noise,
	TextureType_SIZE
};

enum MaterialType
{
	M_Lambertian,
	M_Dielectric,
	M_Metal,
	M_LIGHT,
	M_Isotropic,
	MaterialType_SIZE
};


struct ObjectDesc
{
	ObjectType  m_type;
	TextureType m_texture;
	MaterialType m_material;
	andro::Vector3 m_colour;
	andro::Vector3 m_position;
	andro::Vector3 m_size;

	float m_roughness;
	bool  m_flipNormals;

	static void FromLua(lua_State * L, int index, Variable * ref)
	{

		ASSERT(lua_istable(L, index));
		ObjectDesc* ref_objectDesc = new ((ObjectDesc*)ref->GetVoidPtr())ObjectDesc();

		lua_getfield(L, index, "Type");
		lua_getfield(L, index, "Material");
		lua_getfield(L, index, "Texture");
		lua_getfield(L, index, "Roughness");
		lua_getfield(L, index, "FlipNormals");
		lua_getfield(L, index, "Size");
		lua_getfield(L, index, "Position");
		lua_getfield(L, index, "Colour");


// colour
		ASSERT(lua_istable(L, -1));
		lua_getfield(L, -1, "r");
		float r = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "g");
		float g = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "b");
		float b = lua_tonumber(L, -1);
		lua_pop(L, 1);
		lua_pop(L, 1);

// position

		ASSERT(lua_istable(L, -1));
		lua_getfield(L, -1, "x");
		float x = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "y");
		float y = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, -1, "z");
		float z = lua_tonumber(L, -1);
		lua_pop(L, 1);
		lua_pop(L, 1);

// size

		if(lua_istable(L, -1))
		{
			lua_getfield(L, -1, "x");
			float x = lua_tonumber(L, -1);
			lua_pop(L, 1);

			lua_getfield(L, -1, "y");
			float y = lua_tonumber(L, -1);
			lua_pop(L, 1);

			lua_getfield(L, -1, "z");
			float z = lua_tonumber(L, -1);
			lua_pop(L, 1);
			lua_pop(L, 1);
			ref_objectDesc->m_size = andro::Vector3(x, y, z);
		}
		else
		{
			float size = lua_tonumber(L, -1);
			ref_objectDesc->m_size = andro::Vector3(size, size, size);
			lua_pop(L, 1);

		}			



		ref_objectDesc->m_colour = andro::Vector3(r, g, b);
		ref_objectDesc->m_position = andro::Vector3(x, y, z);

		ref_objectDesc->m_flipNormals = lua_toboolean(L, -1);
		lua_pop(L, 1);

		ref_objectDesc->m_roughness = lua_tonumber(L, -1);
		lua_pop(L, 1);


		int texture = lua_tointeger(L, -1);
		lua_pop(L, 1);

		int material = lua_tointeger(L, -1);
		lua_pop(L, 1);

		int type = lua_tointeger(L, -1);
		lua_pop(L, 1);


		ASSERT(type < ObjectType::ObjectType_SIZE);
		ASSERT(material < MaterialType::MaterialType_SIZE);
		ASSERT(texture < TextureType::TextureType_SIZE);

		ref_objectDesc->m_type = (ObjectType)type;
		ref_objectDesc->m_material = (MaterialType)material;
		ref_objectDesc->m_texture = (TextureType)texture;


	}

	static void ToLua(lua_State * L, Variable & var)
	{
		ASSERT(false); //not implemented
	}
};

class Object;
extern DEVICE_HOST Object* CreateFromObjectDesc(ObjectDesc& desc);
