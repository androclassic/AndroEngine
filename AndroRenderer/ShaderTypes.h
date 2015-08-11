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
        unsigned int id;
        string filename;
        string source;
    };

    GLSLProgram(const string& vertexShader, const string& fragmentShader);
    virtual ~GLSLProgram()
    {

    }

    void unload();
    bool initialize();
	void linkProgram();
    unsigned int getUniformLocation(const string& name);
    unsigned int getAttribLocation(const string& name);
    void sendUniform(const string& name, const int id);
    void sendUniform4x4(const string& name, const float* matrix, bool transpose=false);
    void sendUniform3x3(const string& name, const float* matrix, bool transpose=false);
    void sendUniform(const string& name, const float red, const float green,
                    const float blue, const float alpha);
    void sendUniform(const string& name, const float x, const float y,
                     const float z);
    void sendUniform(const string& name, const float scalar);
    void bindAttrib(unsigned int index, const string& attribName);
    void bindShader();
   
private:
    string readFile(const string& filename);
    bool compileShader(const GLSLShader& shader);
    void outputShaderLog(unsigned int shaderID);
    void outputProgramLog(unsigned int programID);

    GLSLShader m_vertexShader;
    GLSLShader m_fragmentShader;
    unsigned int m_programID;

    map<string, unsigned int> m_uniformMap;
    map<string, unsigned int> m_attribMap;
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