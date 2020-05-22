#include <iostream>
#include <ctime>
#include <chrono>

#include "Engine.h"
#include "TitleState.h"
#include "SplashState.h"
#include "OpenGLTestState.h"
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

	//ImGui
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(SDL_m_Window, gContext);
	ImGui_ImplOpenGL3_Init("#version 150");

	init_imgui();

	//set random seed
	srand((unsigned)time(NULL));

	m_keyStates = SDL_GetKeyboardState(nullptr);
	m_p_FSM = new StateMachine();
	m_p_AM = new AudioManager();
	m_p_AM->set_music_volume(15);
	m_p_AM->load_sound("res/audio/effect/menubtn.wav");
	m_p_FSM->change_state(new OpenGLTestState());

	m_b_running = true;

	//enable game instance
	m_b_gameInstanceEnabled = true;

	return true;
}

void Engine::init_imgui()
{
	//*****Input*****

	ImGuiIO& io = ImGui::GetIO();

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
	io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
	io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;

	io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
	io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
	io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
	io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
	io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
	io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

	//*****Style*****

	// purple colors, 3 intensities
	#define HI(v)   ImVec4(0.333f, 0.102f, 0.545f, v)
	#define MED(v)  ImVec4(0.392f, 0.325f, 0.580f, v)
	#define LOW(v)  ImVec4(0.232f, 0.201f, 0.271f, v)
	// backgrounds (@todo: complete with BG_MED, BG_LOW)
	#define BG(v)   ImVec4(0.20f, 0.220f, 0.270f, v)
	// text
	#define TEXTT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = TEXTT(0.78f);
	style.Colors[ImGuiCol_TextDisabled] = TEXTT(0.28f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 0.99f); //window bg
	//style.Colors[ImGuiCol_ChildWindowBg] = BG(0.58f);
	style.Colors[ImGuiCol_PopupBg] = BG(0.9f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.0f, 0.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = BG(1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = MED(0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = MED(1.0f);
	style.Colors[ImGuiCol_TitleBg] = LOW(1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = HI(1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = BG(0.75f);
	style.Colors[ImGuiCol_MenuBarBg] = BG(0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = BG(1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = MED(0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = MED(1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_ButtonHovered] = MED(0.86f);
	style.Colors[ImGuiCol_ButtonActive] = MED(1.0f);
	style.Colors[ImGuiCol_Header] = MED(0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = MED(0.86f);
	style.Colors[ImGuiCol_HeaderActive] = HI(1.0f);
	//style.Colors[ImGuiCol_Column] = ImVec4(0.14f, 0.16f, 0.19f, 1.0f);
	//style.Colors[ImGuiCol_ColumnHovered] = MED(0.78f);
	//style.Colors[ImGuiCol_ColumnActive] = MED(1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered] = MED(0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = MED(1.0f);
	style.Colors[ImGuiCol_PlotLines] = TEXTT(0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = MED(1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = TEXTT(0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = MED(1.0f);
	style.Colors[ImGuiCol_TextSelectedBg] = MED(0.43f);
	// [...]
	style.Colors[ImGuiCol_ModalWindowDarkening] = BG(0.73f);

	style.WindowPadding = ImVec2(6, 4);
	style.WindowRounding = 0.0f;
	style.FramePadding = ImVec2(5, 2);
	style.FrameRounding = 3.0f;
	style.ItemSpacing = ImVec2(7, 1);
	style.ItemInnerSpacing = ImVec2(1, 1);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 6.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 2.0f;

	style.WindowTitleAlign.x = 0.5f;

	style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
	style.FrameBorderSize = 0.0f;
	style.WindowBorderSize = 1.0f;
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

	if (m_b_gameInstanceEnabled == true)
		GameInstance::singleton_instance()->update();
}

void Engine::render()
{
	m_p_FSM->render();

	if (m_b_gameInstanceEnabled == true)
		GameInstance::singleton_instance()->render();

	SDL_GL_SwapWindow(Engine::singleton_instance()->get_window());
}

void Engine::handle_events()
{
	ImGuiIO& io = ImGui::GetIO();

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

		case SDL_TEXTINPUT:
			io.AddInputCharactersUTF8(event.text.text);
			break;

		case SDL_KEYUP:
			int key = event.key.keysym.scancode;
			IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
			io.KeysDown[key] = (event.type == SDL_KEYDOWN);
			io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
			io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
			io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
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
