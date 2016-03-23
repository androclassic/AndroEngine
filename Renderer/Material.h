#pragma once

#include "ShaderParam.h"
#include "Program.h"
#include "Texture.h"
#include "GL/glew.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"

#include"../../AndroUtils/Introspection/LuaState.h"


namespace TakeTwo
{
    class Material
    {
    public:
        Material(Program* = nullptr);

        template<typename T>
        void SetShaderParam(const std::string& pName, T pValue, int pCount = 1);
        void SetTexture(Texture&& pTexture);
        void SetProgram(Program* pProgram);
        void Use();
        void Reload();
		void Apply();

        int GetTexturesCount();
		void SetFormatUsed(std::vector<unsigned int>&& pFormatsUsed);
		const std::vector<unsigned int>& GetFormatUsed() { return mFormatsUsed; }

		struct MaterialFormat
		{
			MaterialFormat();
			
			glm::vec4 color_ambient;
			glm::vec4 color_diffuse;
			glm::vec4 color_emissive;
			glm::vec4 color_reflective;
			glm::vec4 color_specular;
			glm::vec4 color_transparent;

			float opacity;
			float reflectivity;
			float refractivity;
			float shininess;
			float shininess_strength;

			std::string textureName;
			std::string programName;

			static void MaterialFromLua(lua_State *L, int index, Variable *ref);
			static void MaterialToLua(lua_State *L, Variable& var);
		};

		enum MaterialFormatEnum
		{
			COLOR_AMBIENT,
			COLOR_DIFFUSE,
			COLOR_EMISSIVE,
			COLOR_REFLECTIVE,
			COLOR_SPECULAR,
			COLOR_TRANSPARENT,

			OPACITY,
			REFLECTIVITY,
			REFRACTIVITY,
			SHININESS,
			SHININESS_STRENGTH,

			Count
		};

		void SetMaterialFormat(MaterialFormat&& pMaterialFormat);
	private:

		Program* mProgram;
        std::vector<Texture> mTextures;
        std::unordered_map<std::string, std::unique_ptr<ShaderParamBase>> mShaderParams;
		std::vector<unsigned int> mFormatsUsed;
		MaterialFormat mMaterialFormat;


    };
}

#include "template_impl/Material.hpp"
