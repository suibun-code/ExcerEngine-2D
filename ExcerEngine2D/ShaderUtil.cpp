#include "ShaderUtil.h"

#include <iostream>
#include <fstream>


unsigned int ShaderUtil::get_compiled_shader(unsigned int shader_type, const std::string& shader_source)
{
	unsigned int shader_id = glCreateShader(shader_type);

	const char* c_source = shader_source.c_str();
	glShaderSource(shader_id, 1, &c_source, nullptr);
	glCompileShader(shader_id);

	GLint result;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

		GLchar* strInfoLog = new GLchar[length + 1];
		glGetShaderInfoLog(shader_id, length, &length, strInfoLog);

		fprintf(stderr, "Compilation error in shader: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	return shader_id;
}

bool ShaderUtil::load_shaders(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
	std::ifstream is_vs(vertexShaderFile);
	const std::string f_vs((std::istreambuf_iterator<char>(is_vs)), std::istreambuf_iterator<char>());

	std::ifstream is_fs(fragmentShaderFile);
	const std::string f_fs((std::istreambuf_iterator<char>(is_fs)), std::istreambuf_iterator<char>());

	gProgramID = glCreateProgram();

	unsigned int vs = get_compiled_shader(GL_VERTEX_SHADER, f_vs);
	unsigned int fs = get_compiled_shader(GL_FRAGMENT_SHADER, f_fs);

	glAttachShader(gProgramID, vs);
	glAttachShader(gProgramID, fs);

	glLinkProgram(gProgramID);
	glValidateProgram(gProgramID);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return true;
}

void ShaderUtil::use_shaders()
{
	glUseProgram(gProgramID);
}

GLuint ShaderUtil::get_shaders()
{
	return gProgramID;
}

void ShaderUtil::delete_shaders()
{
	glDeleteProgram(gProgramID);
}

