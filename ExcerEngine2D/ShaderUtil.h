#pragma once

#include "glew.h"

#include <string>
#include <fstream>

class ShaderUtil
{
private:
	GLuint gProgramID;
	GLuint GetCompiledShader(GLuint shader_type, const std::string& shader_source);

public:
	ShaderUtil();
	~ShaderUtil();

	//load vertex and fragment shader from file
	bool Load(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

	//use program
	void Use();

	//delete program
	void Delete();

};

