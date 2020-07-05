#pragma once

#include <memory>

#include "glew.h"
#include "SDL_opengl.h"
#include "ShaderUtil.h"
#include "FSM.h"
#include "Engine.h"

//GLM
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

class OGLSprite
{
private:
	ShaderUtil shaderUtil;

	GLint gVertexPos2DLocation = -1;
	GLuint gVAO;
	//GLuint gVAO2 = 0;
	GLuint gVBO;
	GLuint gIBO;
	GLuint cVBO;
	GLuint tVBO;
	GLuint textures[2] = { NULL };

	//uniforms
	GLint uniModel = 0;
	GLint uniView = 0;
	GLint uniProj = 0;
	GLint uniMVP = 0;

	//GL matrices
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 MVP;

	GLint posAttrib = 0;
	GLint colAttrib = 0;
	GLint monoAlpha = 0;
	GLint texAttrib = 0;
	GLint uniTime = 0;

protected:

	struct SpriteSize
	{
		GLfloat x = 0;
		GLfloat y = 0;
		GLfloat w = 0;
		GLfloat h = 0;
	};

	struct SpritePosition
	{
		GLfloat xPos = 0;
		GLfloat yPos = 0;
	};

public:
	OGLSprite(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat xPos, GLfloat yPos);

	SpriteSize spriteSize;
	SpritePosition spritePosition;

	GLint get_vertpos2d() { return gVertexPos2DLocation; }
	GLuint get_sprite_VAO() { return gVAO; }

	void destroy_sprite();
};

