#include "glm/glm.hpp"
#include "Material.h"
#include"Engine.h"
#include <sstream> 

TakeOne::Material::Material(Program* pProgram)
        : mProgram(pProgram)
{
}

void TakeOne::Material::SetProgram(Program* pProgram)
{
    mProgram = pProgram;
}

void TakeOne::Material::SetTexture(Texture&& pTexture)
{
    mTextures.push_back(std::move(pTexture));
}

void TakeOne::Material::Use()
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

void TakeOne::Material::Apply()
{
	std::stringstream vertex,fragment, texture;
	vertex   << "data/shaders/" << mMaterialFormat.programName << "/vertex.glsl";
	fragment << "data/shaders/" << mMaterialFormat.programName << "/fragment.glsl";
	texture << "data/" << mMaterialFormat.textureName;

	const std::string vertex_shader = vertex.str();
	const std::string fragment_shader = fragment.str();

	Program::ProgramArgs args(vertex_shader, fragment_shader);
	std::string resource_key = vertex_shader + fragment_shader;

	mProgram = Engine::GetInstance()->GetResoruceManager().Load<Program>(resource_key.c_str(), &args); 

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

void TakeOne::Material::Reload()
{
    assert(mProgram != nullptr && "The program from material must be set!");

    mProgram->Reload();
}

int TakeOne::Material::GetTexturesCount()
{
    return mTextures.size();
}

void TakeOne::Material::SetFormatUsed(std::vector<unsigned int>&& pFormatsUsed)
{
	mFormatsUsed = std::move(pFormatsUsed);
}

void TakeOne::Material::SetMaterialFormat(MaterialFormat&& pMaterialFormat)
{
	mMaterialFormat = std::move(pMaterialFormat);
}


TakeOne::Material::MaterialFormat::MaterialFormat()
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
