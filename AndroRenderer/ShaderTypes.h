#pragma once

#include <windows.h>
#include <GL\glew.h>

#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../AndroUtils/AndroUtils.h"



using std::string;
using std::ifstream;
using std::map;
using std::vector;


namespace andro
{
class GLSLProgram
{
public:
    struct GLSLShader
    {
        bssU32 id;
        string filename;
        string source;
    };

    GLSLProgram(const string& vertexShader, const string& fragmentShader);
    virtual ~GLSLProgram()
    {

    }

    void unload();
    bssBool initialize();
	void linkProgram();
    bssU32 getUniformLocation(const string& name);
    bssU32 getAttribLocation(const string& name);
    void sendUniform(const string& name, const int id);
    void sendUniform4x4(const string& name, const bssFloat* matrix, bssBool transpose=false);
    void sendUniform3x3(const string& name, const bssFloat* matrix, bssBool transpose=false);
    void sendUniform(const string& name, const bssFloat red, const bssFloat green,
                    const bssFloat blue, const bssFloat alpha);
    void sendUniform(const string& name, const bssFloat x, const bssFloat y,
                     const bssFloat z);
    void sendUniform(const string& name, const bssFloat scalar);
    void bindAttrib(bssU32 index, const string& attribName);
    void bindShader();
   
private:
    string readFile(const string& filename);
    bssBool compileShader(const GLSLShader& shader);
    void outputShaderLog(bssU32 shaderID);
    void outputProgramLog(bssU32 programID);

    GLSLShader m_vertexShader;
    GLSLShader m_fragmentShader;
    bssU32 m_programID;

    map<string, bssU32> m_uniformMap;
    map<string, bssU32> m_attribMap;
};

class AbstractProgramType: public GLSLProgram
{
public:
	AbstractProgramType(const string& vertexShader, const string& fragmentShader): GLSLProgram(vertexShader, fragmentShader){};
	virtual ~AbstractProgramType(){};

	virtual void BindAttributeLocations() = 0;
	virtual void UpdateUniforms() = 0;
	virtual void EnableVertexAttributes() = 0;
	virtual void DisableVertexAttributes() = 0;
};

class ProgramType_MV_PR_TEX: public AbstractProgramType
{
public:
	ProgramType_MV_PR_TEX(const string& vertexShader, const string& fragmentShader);
	~ProgramType_MV_PR_TEX();

	void BindAttributeLocations();
	void UpdateUniforms();
	void EnableVertexAttributes();
	void DisableVertexAttributes();

};

}