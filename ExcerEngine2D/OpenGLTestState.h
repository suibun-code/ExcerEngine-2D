#pragma once

#include <chrono>

#include "FSM.h"
#include "Engine.h"
#include "ShaderUtil.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

class OpenGLTestState : public State
{
private:
	ShaderUtil shaderUtil;

	//GL ints
	GLint gVertexPos2DLocation = -1;
	GLuint gVAO = 0;
	GLuint gVAO2 = 0;
	GLuint gVBO = 0;
	GLuint gIBO = 0;
	GLuint cVBO = 0;
	GLuint tVBO = 0;
	GLuint textures[2] = { NULL };

	//GL matrices
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 MVP;


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

