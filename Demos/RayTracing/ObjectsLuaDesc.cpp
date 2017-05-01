#include "ObjectsLuaDesc.h"
#include "RayObjects.h"

DEVICE_HOST Object* CreateFromObjectDesc(ObjectDesc& desc)
{
	Object* object = nullptr;
	material* mat = nullptr;
	CTexture* tex = nullptr;

	if (desc.m_material == MaterialType::M_Lambertian)
	{

		if (desc.m_texture == TextureType::Tex_Constant)
			tex = new constant_texture(desc.m_colour);
		else
			tex = new noise_texture(desc.m_colour);

		mat = new Lambertian(tex);
	}
	else if (desc.m_material == MaterialType::M_LIGHT)
	{
		if (desc.m_texture == TextureType::Tex_Constant)
			tex = new constant_texture(desc.m_colour);
		else
			tex = new noise_texture(desc.m_colour);

		mat = new diffuse_light(tex);
	}
	else if (desc.m_material == MaterialType::M_Metal)
	{
		mat = new Metal(desc.m_colour, desc.m_roughness);
	}
	else if (desc.m_material == MaterialType::M_Dielectric)
	{
		mat = new Dielectric(desc.m_roughness); //refractive index
	}
	else
		ASSERT(false);

	//------------------------------------------------------------
	if (desc.m_type == ObjectType::OBJ_Sphere)
	{
		object = new SphereObject(mat, desc.m_position, desc.m_size.x);
	}
	else if (desc.m_type == ObjectType::OBJ_Box)
	{
		object = new BoxObject(mat, desc.m_position, desc.m_size);
	}
	else if (desc.m_type == ObjectType::OBJ_RectObjectXY)
	{
		Vector2 size(desc.m_size.x, desc.m_size.y);
		object = new RectObject(mat, desc.m_position, size, RectObjectType::XY);
	}
	else if (desc.m_type == ObjectType::OBJ_RectObjectYZ)
	{
		Vector2 size(desc.m_size.x, desc.m_size.y);
		object = new RectObject(mat, desc.m_position, size, RectObjectType::YZ);
	}
	else if (desc.m_type == ObjectType::OBJ_RectObjectXZ)
	{
		Vector2 size(desc.m_size.x, desc.m_size.y);
		object = new RectObject(mat, desc.m_position, size, RectObjectType::XZ);
	}
	else
		ASSERT(false);

	return object;
	//return new BoxObject(new Lambertian(new constant_texture(Vector3(1))), Vector3(0.5f, 0, 0), 4);
}