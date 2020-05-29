#pragma once

#include <chrono>

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

	//time
	float time = 0.f;
	std::chrono::steady_clock::time_point t_now;
	std::chrono::steady_clock::time_point t_start;
	GLint posAttrib = 0;
	GLint colAttrib = 0;
	GLint monoAlpha = 0;
	GLint texAttrib = 0;
	GLint uniTime = 0;

public:
	OpenGLTestState() : State("OpenGLTest") {}
	void enter();
	void update(float deltaTime);
	void render();
	void handle_state_events(const SDL_Event* event);
	void resume() {}
	void exit();
};

