#include <iostream>
#include <ctime>
#include <chrono>

#include "Engine.h"
#include "TitleState.h"

Engine* Engine::instance = nullptr;

Engine::Engine() {}
Engine::~Engine() {}

bool Engine::init_all(const char* title, const int xpos, const int ypos, const int width, const int height, const int flags)
{
	//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		ConsoleIO::get_instance()->system_ok("SDL Core Initilization", 1);
		SDL_m_Window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if (SDL_m_Window != nullptr)
		{
			ConsoleIO::get_instance()->system_ok("SDL Window Initilization", 1);
			SDL_m_Renderer = SDL_CreateRenderer(SDL_m_Window, -1, SDL_RENDERER_ACCELERATED);
			if (SDL_m_Renderer != nullptr)
			{
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
				ConsoleIO::get_instance()->system_ok("SDL Renderer Initilization", 1);
				SDL_SetRenderDrawColor(SDL_m_Renderer, 255, 0, 0, 255);
				SDL_RenderSetLogicalSize(SDL_m_Renderer, window_width, window_height);
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
				SDL_SetWindowMinimumSize(SDL_m_Window, 640, 360);

				if (IMG_Init(IMG_INIT_PNG) != 0)
				{
					ConsoleIO::get_instance()->system_ok("SDL Image Initilization", 1);
					SDL_m_surface = IMG_Load("res/img/icon.png");
					SDL_SetWindowIcon(SDL_m_Window, SDL_m_surface);
					SDL_FreeSurface(SDL_m_surface);
					if (Mix_Init(MIX_INIT_MP3) != 0)
					{
						Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
						Mix_AllocateChannels(16);
						ConsoleIO::get_instance()->system_ok("SDL Mixer Initilization", 1);

						if (TTF_Init() == 0)
							ConsoleIO::get_instance()->system_ok("SDL Font Initilization", 1);
						else
						{
							ConsoleIO::get_instance()->system_ok("SDL Font Initilization", 0);
							return false;
						}
					}
					else
					{
						ConsoleIO::get_instance()->system_ok("SDL Mixer Initilization", 0);
						return false;
					}
				}
				else
				{
					ConsoleIO::get_instance()->system_ok("SDL Image Initilization", 0);
					return false;
				}
			}
			else
			{
				ConsoleIO::get_instance()->system_ok("SDL Renderer Initilization", 0);
				return false;
			}

			//ImGui
			ImGui::CreateContext();
			ImGuiSDL::Initialize(SDL_m_Renderer, width, height);

		}
		else
		{
			ConsoleIO::get_instance()->system_ok("SDL Window Initilization", 0);
			return false;
		}
	}
	else
	{
		ConsoleIO::get_instance()->system_ok("SDL Core Initilization", 0);
		return false;
	}

	//Create context
	gContext = SDL_GL_CreateContext(SDL_m_Window);

	if (gContext == NULL)
	{
		printf("OpenGL context failed to be created. SDL Error: %s\n", SDL_GetError());
		m_b_running = false;
	}
	else
	{
		//initialize GLEW
		glewExperimental = true;
		GLenum glewError = glewInit();

		if (glewError != GLEW_OK)
		{
			printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		}
	}

	if (!initGL())
	{
		printf("Unable to initialize OpenGL!\n");
	}

	srand((unsigned)time(NULL)); //set random seed

	m_keyStates = SDL_GetKeyboardState(nullptr);
	m_p_FSM = new StateMachine();
	m_p_AM = new AudioManager();
	m_p_AM->set_music_volume(15);
	m_p_AM->load_sound("res/audio/effect/menubtn.wav");
	m_p_FSM->change_state(new TitleState());

	m_b_running = true;
	return true;
}

bool Engine::initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	//generate program
	gProgramID = glCreateProgram();

	//create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//get vertex source
	const GLchar* vertexShaderSource[] =
	{
		"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};

	//set vertex source
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

	//compile vertex source
	glCompileShader(vertexShader);

	//check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);

	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		//printShaderLog(vertexShader);
		success = false;
	}
	else
	{
		//attach vertex shader to program
		glAttachShader(gProgramID, vertexShader);

		//create fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//get fragment source
		const GLchar* fragmentShaderSource[] =
		{
			"#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
		};

		//set fragment source
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		//compile fragment source
		glCompileShader(fragmentShader);

		//check fragment shader for errors
		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{
			printf("Unable to compile fragment shader %d!\n", fragmentShader);
			//printShaderLog(fragmentShader);
			success = false;
		}
		else
		{
			//attach fragment shader to program
			glAttachShader(gProgramID, fragmentShader);

			//link program
			glLinkProgram(gProgramID);

			//check for errors
			GLint programSuccess = GL_TRUE;
			glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				printf("Error linking program %d!\n", gProgramID);
				//printProgramLog(gProgramID);
				success = false;
			}
			else
			{
				//get vertex attribute location
				gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
				if (gVertexPos2DLocation == -1)
				{
					printf("LVertexPos2D is not a valid glsl program variable!\n");
					success = false;
				}
				else
				{
					
				}
			}
		}
	}
	return success;
}

void Engine::handleKeys(unsigned char key, int x, int y)
{
}

void Engine::wait_for(int time)
{
	SDL_Delay(time);
}

int Engine::round64(int num)
{
	int multiple = 64;
	int remainder = num % multiple;

	if (num < multiple)
		return 0;
	if (multiple == 0)
		return num;
	if (remainder == 0)
		return num;

	return num - remainder;
}

bool Engine::tick()
{
	lastTime = currentTime;
	currentTime = SDL_GetPerformanceCounter();
	deltaTime = deltaTime = ((currentTime - lastTime) * 1000 / (float)SDL_GetPerformanceFrequency());
	framesPerSecond = 1000 / deltaTime;

	//auto duration = std::chrono::steady_clock::now().time_since_epoch();
	//auto count = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
	//m_tick = 1000000 / FPS;
	//if (count % m_tick < 325) // Margin of error for modulus.
	//{
	//	if (m_b_gotTick == false) // Drops potential duplicate frames.
	//		m_b_gotTick = true;
	//}
	//else m_b_gotTick = false;

	m_b_gotTick = true;
	return m_b_gotTick;
}

void Engine::update(float deltaTime)
{
	m_p_FSM->update(deltaTime);
}

void Engine::render()
{
	m_p_FSM->render();
}

void Engine::handle_events()
{
	SDL_Event event;

	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_WINDOWEVENT)
		{
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				std::cout << "Window Resized\n";
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				if (!m_p_AM->is_paused())
				{
					m_b_musicPlaying = false;
					m_p_AM->pause_music();
				}
				else
					m_b_musicPlaying = true;
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				if (m_p_AM->is_paused() && m_b_musicPlaying == false)
					m_p_AM->resume_music();
				break;
			}
		}

		switch (event.type)
		{
		case SDL_QUIT:
			m_b_running = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				if (event.button.state == SDL_PRESSED)
					m_b_LMBState = true;
			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
				m_b_LMBState = false;
			break;

		case SDL_MOUSEMOTION:
			m_i_mousePosX = event.motion.x;
			m_i_mousePosY = event.motion.y;
			break;
		}
	}

	m_p_FSM->handle_state_events();
}

void Engine::quit()
{
	m_b_running = false;
}

void Engine::clean()
{
	//cleanup ImGui
	ImGui::DestroyContext();

	SDL_DestroyRenderer(SDL_m_Renderer);
	SDL_DestroyWindow(SDL_m_Window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

bool Engine::key_down(SDL_Scancode k)
{
	if (m_keyStates != nullptr)
	{
		if (m_keyStates[k] == 1)
			return true;
		else
			return false;
	}
	return false;
}
