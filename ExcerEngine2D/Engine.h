#pragma once

#define FPS 144

#include <fstream>
#include <iostream>

//SDL
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

//GLEW
#include "glew.h"

//SDL OPENGL
#include "SDL_opengl.h"

//IMGUI
#include "imgui.h"
#include "imgui_sdl.h"

#include "FSM.h"
#include "ConsoleIO.h"
#include "AudioManager.h"
#include "ShaderUtil.h"

class Engine
{
private:
	//engine instance
	static Engine* instance;

	//GLEW / OpenGL
	ShaderUtil shaderUtil;
	SDL_GLContext gContext = NULL; //OpenGL context

	GLint gVertexPos2DLocation = -1;
	GLuint gVAO = 0;
	GLuint gVAO2 = 0;
	GLuint gVBO = 0;
	GLuint gIBO = 0;
	GLuint cVBO = 0;
	GLuint tVBO = 0;
	GLuint tex = 0;

	//boolean checks
	bool m_b_gameInstanceEnabled = true;
	bool m_b_running = false;
	bool m_b_gotTick = false;
	bool m_b_LMBState = false;
	bool m_b_musicPlaying = false;

	//time
	Uint64 currentTime = SDL_GetPerformanceCounter();
	Uint64 lastTime = 0;
	float deltaTime = 0;
	float framesPerSecond = 0;

	//unused old variables for capping fps
	int m_tick = 0;
	int frame = 0;
	bool cap = true;

	//mouse position
	Sint32 m_i_mousePosX = 0, m_i_mousePosY = 0;

	//window dimensions
	int window_width = 1280, window_height = 720;

	SDL_Window* SDL_m_Window = nullptr; //window
	SDL_Renderer* SDL_m_Renderer = nullptr; //renderer
	SDL_Surface* SDL_m_surface = nullptr; //surface

	const uint8_t* m_keyStates = nullptr; //key states

	StateMachine* m_p_FSM = nullptr; //state machine
	AudioManager* m_p_AM = nullptr; //audio manager

public:
	const char* version = "v0.1.1";

	static Engine* singleton_instance()
	{
		if (instance == nullptr)
			instance = new Engine();
		return instance;
	}

	Engine();
	~Engine();

	bool init_all(const char* title, const int xpos, const int ypos,
		const int width, const int height, const int flags);

	//GLEW
	bool initGL(); //initializes matrices and clear color
	void printProgramLog(GLuint program);
	void printShaderLog(GLuint shader);
	void handleKeys(unsigned char key, int x, int y); //input handler

	bool is_running() { return m_b_running; }
	Sint32 get_mouse_posX() { return m_i_mousePosX; }
	Sint32 get_mouse_posY() { return m_i_mousePosY; }
	bool LM_state() { return m_b_LMBState; }
	void set_LMB_state(bool state) { m_b_LMBState = state; }
	bool get_LMB_state() { return m_b_LMBState; }
	float get_delta_time() { return deltaTime; }
	float getFramesPerSecond() { return framesPerSecond; }
	int get_window_width() { return window_width; }
	int get_window_height() { return window_height; }
	void get_logical_size() { SDL_RenderGetLogicalSize(SDL_m_Renderer, &window_width, &window_height); }
	SDL_Window *get_window() { return SDL_m_Window; }
	SDL_Renderer *get_renderer() { return SDL_m_Renderer; }
	StateMachine *get_FSM() { return m_p_FSM; }
	AudioManager *get_AM() { return m_p_AM; }
	void wait_for(int time);
	int round64(int num);
	bool tick();
	void update(float deltaTime);
	void render();
	void handle_events();
	void quit();
	void clean();

	bool key_down(SDL_Scancode k);
};

