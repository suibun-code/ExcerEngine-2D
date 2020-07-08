#pragma once

#include "ShaderUtil.h"
#include "Texture2D.h"
#include "glm/glm/glm.hpp"

class XSprite
{
private:
	ShaderUtil shaderUtil;

	GLint posAttrib = 0;
	GLint colAttrib = 0;
	GLint texAttrib = 0;
	GLint alphaUni = 0;
	GLint timeUni = 0;

	GLuint gVAO = 0;
	GLuint gVBO = 0;
	GLuint gIBO = 0;
	GLuint cVBO = 0;
	GLuint tVBO = 0;

	void initRenderData();

protected:

public:
	XSprite(ShaderUtil shaderUtil);
	~XSprite();

	void DrawSprite(Texture2D& texture, glm::vec2 position = glm::vec2(0.f, 0.f), glm::vec2 size = glm::vec2(10.f, 10.f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
};

