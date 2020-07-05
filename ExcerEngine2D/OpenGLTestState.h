#pragma once

#include <chrono>

#include "FSM.h"
#include "Engine.h"
#include "ShaderUtil.h"
#include "OGLSprite.h"

class OpenGLTestState : public State
{
private:
	ShaderUtil shaderUtil;

	//sprites
	std::vector<OGLSprite*> allSprites;

	//time
	float time = 0.f;
	std::chrono::steady_clock::time_point t_now;
	std::chrono::steady_clock::time_point t_start;
	GLint monoAlpha = 0;
	GLint uniTime = 0;

public:
	OpenGLTestState() : State("OpenGLTest") {}
	void enter();
	void update(float deltaTime);
	void render();
	void handle_state_events(const SDL_Event* event);
	void resume() {}
	void exit();

	//**TEST**
	void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation);
	void transformObject(float scale);
	void transformObject(glm::vec3 rotationAxis, float rotationAngle);
	void transformObject(glm::vec3 translation);
};

