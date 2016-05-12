#include "glm/glm.hpp"
#include "Material.h"
#include"Engine.h"
#include <sstream> 

TakeTwo::Material::Material(Program* pProgram)
        : mProgram(pProgram)
{
	mFormatsUsed.resize(MaterialFormatEnum::Count);
}


void TakeTwo::Material::SetProgram(Program* pProgram)
{
    mProgram = pProgram;
}

void TakeTwo::Material::SetTexture(Texture&& pTexture)
{
    mTextures.push_back(std::move(pTexture));
}

void TakeTwo::Material::Use()
{
    assert(mProgram != nullptr && "The program from material must be set!");
	mProgram->Use();


    for(const auto& texture:mTextures)
        texture.Bind();

    //send the uniforms to the shader
    for (auto& kv : mShaderParams)
    {
        kv.second->SendToShader();
    }
}

void TakeTwo::Material::Apply()
{
	std::stringstream texture;
	texture << "data/" << mMaterialFormat.textureName;

	mProgram = EffectLibrary::GetInstance()->GetEffect(mMaterialFormat.programName);

	SetTexture(Texture(texture.str(), Texture::INVERT_Y | Texture::COMPRESS_TO_DXT | Texture::TEXTURE_REPEATS | Texture::MIPMAPS));


	if (mFormatsUsed[COLOR_AMBIENT])
		SetShaderParam("u_color_ambient", mMaterialFormat.color_ambient);

	if (mFormatsUsed[COLOR_DIFFUSE])
		SetShaderParam("u_color_diffuse", mMaterialFormat.color_diffuse);

	if (mFormatsUsed[COLOR_EMISSIVE])
		SetShaderParam("u_color_emissive", mMaterialFormat.color_emissive);

	if (mFormatsUsed[COLOR_REFLECTIVE])
		SetShaderParam("u_color_reflective", mMaterialFormat.color_reflective);

	if (mFormatsUsed[COLOR_SPECULAR])
		SetShaderParam("u_color_specular", mMaterialFormat.color_specular);

	if (mFormatsUsed[COLOR_TRANSPARENT])
		SetShaderParam("u_color_transparent", mMaterialFormat.color_transparent);

	if (mFormatsUsed[OPACITY])
		SetShaderParam("u_opacity", mMaterialFormat.opacity);

	if (mFormatsUsed[REFLECTIVITY])
		SetShaderParam("u_reflectivity", mMaterialFormat.reflectivity);

	if (mFormatsUsed[REFRACTIVITY])
		SetShaderParam("u_refractivity", mMaterialFormat.refractivity);

	if (mFormatsUsed[SHININESS])
		SetShaderParam("u_shininess", mMaterialFormat.shininess);

	if (mFormatsUsed[SHININESS_STRENGTH])
		SetShaderParam("u_shininess_strength", mMaterialFormat.shininess_strength);

	SetShaderParam("u_textures_count", GetTexturesCount());


}


int TakeTwo::Material::GetTexturesCount()
{
    return mTextures.size();
}

void TakeTwo::Material::SetFormatUsed(std::vector<unsigned int>&& pFormatsUsed)
{
	mFormatsUsed = std::move(pFormatsUsed);
}

void TakeTwo::Material::SetMaterialFormat(MaterialFormat&& pMaterialFormat)
{
	mMaterialFormat = std::move(pMaterialFormat);
}


TakeTwo::Material::MaterialFormat::MaterialFormat()
{
	//default values TODO
	color_ambient = glm::vec4(1, 0, 1, 1);
	color_diffuse = glm::vec4(1, 0, 1, 1);
	color_emissive = glm::vec4(1, 0, 1, 1);
	color_reflective = glm::vec4(1, 0, 1, 1);
	opacity = 1;
	shininess_strength = 1;
	shininess = 1;
	textureName = "default.jpg";
	programName = "SimpleColor";
}

void TakeTwo::Material::MaterialFormat::MaterialFromLua(lua_State * L, int index, Variable * ref)
{
	ASSERT(lua_istable(L, index));

	TakeTwo::Material::MaterialFormat temp;
	TakeTwo::Material::MaterialFormat* ref_mat = new ((TakeTwo::Material::MaterialFormat*)ref->GetVoidPtr())TakeTwo::Material::MaterialFormat();

	lua_getfield(L, index, "Texture");
	lua_getfield(L, index, "Shader");

	const char *texture = luaL_checkstring(L, -2);
	const char *shader = luaL_checkstring(L, -1);

	ref_mat->programName = shader;
	ref_mat->textureName = texture;
}

void TakeTwo::Material::MaterialFormat::MaterialToLua(lua_State * L, Variable & var)
{
	ASSERT(false); //not implemented
}

