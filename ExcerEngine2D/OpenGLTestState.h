#pragma once

#include "FSM.h"
#include "Engine.h"
#include "ShaderUtil.h"

class OpenGLTestState : public State
{
private:
	ShaderUtil shaderUtil;

	GLint gVertexPos2DLocation = -1;
	GLuint gVAO = 0;
	GLuint gVAO2 = 0;
	GLuint gVBO = 0;
	GLuint gIBO = 0;
	GLuint cVBO = 0;
	GLuint tVBO = 0;
	GLuint tex = 0;

public:
	OpenGLTestState() : State("OpenGLTest") {}
	void enter();
	void update(float deltaTime);
	void render();
	void resume() {}
	void exit();
};

