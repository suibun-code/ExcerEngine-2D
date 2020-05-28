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
	GLuint textures[2] = { NULL };

public:
	OpenGLTestState() : State("OpenGLTest") {}
	void enter();
	void update(float deltaTime);
	void render();
	void handle_state_events(const SDL_Event* event);
	void resume() {}
	void exit();
};

