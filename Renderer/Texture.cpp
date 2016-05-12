#include <GL/glew.h>
#include "Texture.h"
#include "Log.h"
#include "../../External/SOIL/SOIL.h"

std::unordered_map<std::string, std::pair<int, int>> TakeTwo::Texture::mUsedTextures;

TakeTwo::Texture::Texture()
{

}

TakeTwo::Texture::Texture(std::string pTexturePath, unsigned int pTextureFlags)
{
    LoadFromFile(pTexturePath, pTextureFlags);
}


TakeTwo::Texture::Texture(TakeTwo::Texture &&pTexture) :
    mTexturePath(std::move(pTexture.mTexturePath)), mTextureFlags(std::move(pTexture.mTextureFlags)),
    mTextureId(std::move(pTexture.mTextureId))
{
    pTexture.mTexturePath = "";
    pTexture.mTextureFlags = 0;
    pTexture.mTextureId = 0;
}

TakeTwo::Texture& TakeTwo::Texture::operator=(TakeTwo::Texture &&pTexture)
{
    if(this != &pTexture)
    {
        Unload();
        mTexturePath = std::move(pTexture.mTexturePath);
        mTextureFlags = std::move(pTexture.mTextureFlags);
        mTextureId = std::move(pTexture.mTextureId);

        pTexture.mTexturePath = "";
        pTexture.mTextureFlags = 0;
        pTexture.mTextureId = 0;
    }
    return *this;
}

TakeTwo::Texture::~Texture()
{
    Unload();
}

void TakeTwo::Texture::LoadFromFile(std::string pTexturePath, unsigned int pTextureFlags, unsigned int pTextureId)
{
    mTexturePath = pTexturePath;
    mTextureFlags = pTextureFlags;

    Load(pTextureId);
}

void TakeTwo::Texture::LoadFromBuffer(const unsigned char* const pBuffer, int pSize, unsigned int pTextureFlags,  unsigned int pTextureId)
{
    Unload();

    mTextureFlags = pTextureFlags;
    mTextureId = pTextureId;

    mTextureId = SOIL_load_OGL_texture_from_memory
            (
                pBuffer,
                pSize,
                SOIL_LOAD_AUTO,
                mTextureId ? mTextureId : SOIL_CREATE_NEW_ID, //if the texture was already loaded, overwrite it
                mTextureFlags
            );

    if(!mTextureId)
    {
        LOG_MSG(LL"Failed to load texture from memory.");
    }
}

void TakeTwo::Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, mTextureId);
}

void TakeTwo::Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TakeTwo::Texture::Unload()
{
    if (mTextureId!=0){
        Unbind();

        if(mUsedTextures.find(mTexturePath) != mUsedTextures.end() && mUsedTextures[mTexturePath].second > 1)
        {
            mUsedTextures[mTexturePath].second--;
        }
        else
        {
            glDeleteTextures(1, &mTextureId);
            mUsedTextures.erase(mTexturePath);
        }

        mTextureId = 0;
    }
}

void TakeTwo::Texture::Load(unsigned int pTextureId)
{
    //if the user doesn't want a specific texture id and the texture was already loaded
    if(pTextureId == 0 && mUsedTextures.find(mTexturePath) != mUsedTextures.end())
    {
        mTextureId = mUsedTextures[mTexturePath].first;
        mUsedTextures[mTexturePath].second++; //ref count
    }
    else
    {
        Unload();

        mTextureId = pTextureId;

        mTextureId = SOIL_load_OGL_texture
                (
                    mTexturePath.c_str(),
                    SOIL_LOAD_AUTO,
                    mTextureId ? mTextureId : SOIL_CREATE_NEW_ID,
                    mTextureFlags
                );
        if(!mTextureId)
        {
            LOG_MSG(L"Failed to load texture \"%s\".", mTexturePath.c_str());
        }

        mUsedTextures[mTexturePath] = std::make_pair(mTextureId, 1);
    }
}
