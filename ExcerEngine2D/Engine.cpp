#include <iostream>
#include <ctime>
#include <chrono>

#include "Engine.h"
#include "TitleState.h"

Engine* Engine::instance = nullptr;

Engine::Engine() {}
Engine::~Engine() {}

//unsigned long Engine::getFileLength(std::ifstream& file)
//{
//	if (!file.good()) return 0;
//
//	unsigned long pos = file.tellg();
//	file.seekg(0, std::ios::end);
//
//	unsigned long len = file.tellg();
//	file.seekg(std::ios::beg);
//
//	return len;
//}
//
//int Engine::loadShader(char* filename, GLchar** ShaderSource, unsigned long* len)
//{
//	std::ifstream file;
//	file.open(filename, std::ios::in); // opens as ASCII!
//	if (!file) return -1;
//
//	len = getFileLength(file);
//
//	if (len == 0) return -2;   // Error: Empty File 
//
//	*ShaderSource = (GLubyte*) new char[len + 1];
//	if (*ShaderSource == 0) return -3;   // can't reserve memory
//
//	 // len isn't always strlen cause some characters are stripped in ascii read...
//	 // it is important to 0-terminate the real length later, len is just max possible value... 
//	*ShaderSource[len] = 0;
//
//	unsigned int i = 0;
//	while (file.good())
//	{
//		*ShaderSource[i] = file.get();       // get character from file.
//		if (!file.eof())
//			i++;
//	}
//
//	*ShaderSource[i] = 0;  // 0-terminate it at the correct position
//
//	file.close();
//
//	return 0; // No Error
//}
//
//int Engine::unloadShader(GLubyte** ShaderSource)
//{
//	if (*ShaderSource != 0)
//		delete[] * ShaderSource;
//	*ShaderSource = 0;
//}

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
	//vertex shader
	const char* vertexSource = R"glsl(
    #version 150 core

    in vec2 position;

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

	//fragment shader
	const char* fragmentSource = R"glsl(
	#version 150 core

	out vec4 outColor;

	void main()
	{
		outColor = vec4(1.0, 1.0, 1.0, 1.0);
	}	
)glsl";

	//success flag
	bool success = true;

	//generate program
	gProgramID = glCreateProgram();

	//create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//set vertex source
	glShaderSource(vertexShader, 1, &vertexSource, NULL);

	//compile vertex source
	glCompileShader(vertexShader);

	//check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		printShaderLog(vertexShader);
		success = false;
	}
	else
	{
		//attach vertex shader to program
		glAttachShader(gProgramID, vertexShader);

		//create fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//set fragment source
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

		//compile fragment source
		glCompileShader(fragmentShader);

		//check fragment shader for errors
		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{
			printf("Unable to compile fragment shader %d!\n", fragmentShader);
			printShaderLog(fragmentShader);
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
				printProgramLog(gProgramID);
				success = false;
			}
			else
			{
				//get vertex attribute location
				gVertexPos2DLocation = glGetAttribLocation(gProgramID, "position");
				if (gVertexPos2DLocation == -1)
				{
					printf("LVertexPos2D is not a valid glsl program variable!\n");
					success = false;
				}
				else
				{
					//initialize clear color
					glClearColor(0.f, 0.f, 1.f, 1.f);

					GLint posAttrib = glGetAttribLocation(gProgramID, "position");

					//IBO data
					GLuint indexData[] =
					{
						0, 1, 2, 3
					};

					//VBO data
					GLfloat vertexData[] =
					{
						-0.5f, -0.5f,
						 0.5f, -0.5f,
						 0.5f,  0.5f,
						-0.5f,  0.5f
					};

					GLfloat colorData[] =
					{
						1.f, 1.f, 1.f, 1.f,
						0.f, 1.f, 0.f, 1.f,
						1.f, 0.f, 0.f, 1.f,
						0.f, 0.f, 1.f, 1.f
					};

					//create and bind VAO
					gVAO = 0;
					glGenVertexArrays(1, &gVAO);
					glBindVertexArray(gVAO);

					//create IBO
					gIBO = 0;
					glGenBuffers(1, &gIBO);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

					//create VBO
					gVBO = 0;
					glGenBuffers(1, &gVBO);
					glBindBuffer(GL_ARRAY_BUFFER, gVBO);
					glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
					glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(0);

					//cVBO = 0;
					//glGenBuffers(1, &cVBO);
					//glBindBuffer(GL_ARRAY_BUFFER, cVBO);
					//glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
					//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
					//glEnableVertexAttribArray(1);

					//set vertex data
					glBindBuffer(GL_ARRAY_BUFFER, gVBO);
					glBindBuffer(GL_ARRAY_BUFFER, gIBO);

					//unbind VAO
					glBindVertexArray(NULL);
				}
			}
		}
	}

	return success;
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
	m_p_FSM->update(deltaTime);
}

void Engine::render()
{
	//m_p_FSM->render();

	//clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//render quad
	if (gRenderQuad)
	{
		//bind program
		glUseProgram(gProgramID);

		//enable vertex position
		glEnableVertexAttribArray(gVertexPos2DLocation);

		//set vertex data
		//glBindBuffer(GL_ARRAY_BUFFER, gVBO);
		//glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

		//set index data
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);

		//set array data
		glBindVertexArray(gVAO);

		//render
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

		//disable vertex position
		glDisableVertexAttribArray(gVertexPos2DLocation);

		//unbind program
		glUseProgram(NULL);
	}
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

	//deallocate program
	glDeleteProgram(gProgramID);

	SDL_DestroyRenderer(SDL_m_Renderer);
	SDL_DestroyWindow(SDL_m_Window);
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
