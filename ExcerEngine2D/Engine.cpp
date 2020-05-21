#include <iostream>
#include <ctime>
#include <chrono>

#include "Engine.h"
#include "TitleState.h"
#include "SplashState.h"
#include "GameInstance.h"

Engine* Engine::instance = nullptr;

Engine::Engine() {}
Engine::~Engine() {}

bool Engine::init_all(const char* title, const int xpos, const int ypos, const int width, const int height, const int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		ConsoleIO::get_instance()->system_ok("SDL Core Initilization", 1);

		//Use OpenGL 3.1 core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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

	//create context
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
			printf("Error initializing GLEW. %s\n", glewGetErrorString(glewError));
		}
	}

	if (!initGL())
		printf("Unable to initialize OpenGL!\n");

	//ImGui
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(SDL_m_Window, gContext);
	ImGui_ImplOpenGL3_Init("#version 150");

	//set random seed
	srand((unsigned)time(NULL));

	m_keyStates = SDL_GetKeyboardState(nullptr);
	m_p_FSM = new StateMachine();
	m_p_AM = new AudioManager();
	m_p_AM->set_music_volume(15);
	m_p_AM->load_sound("res/audio/effect/menubtn.wav");
	m_p_FSM->change_state(new TitleState());

	m_b_running = true;

	//enable game instance
	m_b_gameInstanceEnabled = true;

	return true;
}

bool Engine::initGL()
{
	//IBO data
	GLuint indexData[] =
	{
		0, 1, 2, //first triangle
		2, 3, 0  //second triangle
	};

	//VBO data
	GLfloat vertexData[] =
	{
		-0.25f,  0.25f, //top left
		 0.75f,  0.25f, //top right
		 0.75f, -0.75f, //bottom right
		-0.25f, -0.75f, //bottom left
	};

	GLfloat colorData[] =
	{
		1.f, 0.f, 0.f, //top left
		1.f, 0.f, 0.f, //top right
		0.f, 1.f, 0.f, //bottom right
		1.f, 0.f, 0.f  //bottom left
	};

	GLfloat textureData[] =
	{
		0.f, 0.f, //top left
		1.f, 0.f, //top right
		1.f, 1.f, //bottom right
		0.f, 1.f  //bottom left
	};

	//test
	GLuint indexData2[] =
	{
		0, 1, 2, //first triangle
		2, 3, 0	 //second triangle
	};

	GLfloat vertexData2[] =
	{
		-0.75f,  0.75f, //top left
		 0.25f,  0.75f, //top right
		 0.25f, -0.25f, //bottom right
		-0.75f, -0.25f, //bottom left
	};

	GLfloat colorData2[] =
	{
		0.f, 1.f, 0.f, //top left
		0.f, 0.f, 1.f, //top right
		0.f, 0.f, 1.f, //bottom right
		0.f, 0.f, 1.f  //bottom left
	};

	GLfloat textureData2[] =
	{
		0.f, 0.f, //top left
		1.f, 0.f, //top right
		1.f, 1.f, //bottom right
		0.f, 1.f  //bottom left
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//load program
	shaderUtil.load_shaders("shaders/vs.shader", "shaders/fs.shader");
	//bind program
	shaderUtil.use_shaders();

	//get vertex attribute location
	gVertexPos2DLocation = glGetAttribLocation(shaderUtil.get_shaders(), "position");

	//clear to this color
	glClearColor(.5f, .5f, .5f, 1.f);

	GLint posAttrib = glGetAttribLocation(shaderUtil.get_shaders(), "position");
	GLint colAttrib = glGetAttribLocation(shaderUtil.get_shaders(), "color");
	GLint monoAlpha = glGetUniformLocation(shaderUtil.get_shaders(), "alpha");
	GLint texAttrib = glGetAttribLocation(shaderUtil.get_shaders(), "texcoord");
	glUniform1f(monoAlpha, .5f);

	SDL_Surface* testSurface = IMG_Load("res/img/VIDEOGAME.png");

	glGenTextures(1, &tex); // generate texture
	glBindTexture(GL_TEXTURE_2D, tex); //bind texture

	int mode = GL_RGB;

	if (testSurface->format->BytesPerPixel == 4) {
		mode = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, mode, testSurface->w, testSurface->h, 0, mode, GL_UNSIGNED_BYTE, testSurface->pixels);
	SDL_FreeSurface(testSurface);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); //wrapping repeat on X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); //wrapping clamp to border on 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //interpolation method for scaling down
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //interpolation method for scaling up

	//float borderColor[] = { 1.f, 1.f, 1.f, 1.f }; //border color if clamping to border
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); //set the border color

	glGenerateMipmap(GL_TEXTURE_2D); //generate mipmap

	//create and bind VAO
	gVAO = 0;
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	//create IBO
	gIBO = 0;
	glGenBuffers(1, &gIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	//create vertex VBO
	gVBO = 0;
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	//create color VBO
	cVBO = 0;
	glGenBuffers(1, &cVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colAttrib);

	//create texture VBO
	tVBO = 0;
	glGenBuffers(1, &tVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureData), textureData, GL_STATIC_DRAW);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texAttrib);

	//***********************************************************************

	gVAO2 = 0;
	glGenVertexArrays(1, &gVAO2);
	glBindVertexArray(gVAO2);

	GLuint gIBO2 = 0;
	glGenBuffers(1, &gIBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData2), indexData2, GL_STATIC_DRAW);

	GLuint gVBO2 = 0;
	glGenBuffers(1, &gVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData2), vertexData2, GL_STATIC_DRAW);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	GLuint cVBO2 = 0;
	glGenBuffers(1, &cVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, cVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData2), colorData2, GL_STATIC_DRAW);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colAttrib);

	//create texture VBO
	GLuint tVBO2 = 0;
	glGenBuffers(1, &tVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, tVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureData2), textureData2, GL_STATIC_DRAW);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, NULL); // Can optionally unbind the buffer to avoid modification.
	glBindVertexArray(NULL); // Can optionally unbind the vertex array to avoid modification.

	return true;
}

void Engine::printProgramLog(GLuint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}

void Engine::printShaderLog(GLuint shader)
{
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
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

	auto duration = std::chrono::steady_clock::now().time_since_epoch();
	auto count = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
	m_tick = 1000000 / FPS;
	if (count % m_tick < 325) // Margin of error for modulus.
	{
		if (m_b_gotTick == false) // Drops potential duplicate frames.
			m_b_gotTick = true;
	}
	else m_b_gotTick = false;

	return m_b_gotTick;
}

void Engine::update(float deltaTime)
{
	//m_p_FSM->update(deltaTime);

	if (m_b_gameInstanceEnabled == true)
		GameInstance::singleton_instance()->update();
}

void Engine::render()
{
	//m_p_FSM->render();

	//clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//enable vertex position
	glEnableVertexAttribArray(gVertexPos2DLocation);

	//set array data
	glBindVertexArray(gVAO);

	//render
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	//set array data
	glBindVertexArray(gVAO2);

	//render
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	//disable vertex position
	glDisableVertexAttribArray(gVertexPos2DLocation);

	//unbind program
	//glUseProgram(NULL);

	if (m_b_gameInstanceEnabled == true)
		GameInstance::singleton_instance()->render();

	SDL_GL_SwapWindow(Engine::singleton_instance()->get_window());
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

	//m_p_FSM->handle_state_events();

	if (m_b_gameInstanceEnabled == true)
		GameInstance::singleton_instance()->handle_events();
}

void Engine::quit()
{
	m_b_running = false;
}

void Engine::clean()
{
	//cleanup ImGui
	ImGui::DestroyContext();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	//deallocate program
	shaderUtil.delete_shaders();

	//cleanup sdl
	SDL_DestroyRenderer(SDL_m_Renderer);
	SDL_DestroyWindow(SDL_m_Window);
	SDL_GL_DeleteContext(gContext);
	SDL_m_Window = NULL;
	SDL_m_Renderer = NULL;
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
