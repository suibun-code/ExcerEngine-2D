#pragma once

#include "glew.h"

#include <string>

class ShaderUtil
{
private:
	unsigned int gProgramID = NULL;
	unsigned int get_compiled_shader(unsigned int shader_type, const std::string& shader_source);

public:
	ShaderUtil() {}
	~ShaderUtil() {}

	//load vertex and fragment shader from file
	bool load_shaders(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

	//use program
	ShaderUtil& use_shaders();

	//get program
	GLuint get_shaders();

	//delete program
	void delete_shaders();

};

