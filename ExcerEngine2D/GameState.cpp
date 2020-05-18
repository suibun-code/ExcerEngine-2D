#include "GameState.h"
#include "TitleState.h"

void GameState::m_ImGuiKeyMap()
{
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
}

void GameState::m_ImGuiSetStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 0.8f;
	style.FrameRounding = 3.0f;
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
	//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	//style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	//style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	//style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void GameState::m_updateImGui()
{
	// Prepare Window Frame
	ImGui::NewFrame();

	std::string windowString = "Debugging";

	ImGui::Begin(&windowString[0], NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar);

	// set window to top left corner
	ImGui::SetWindowPos(ImVec2(0, 0), true);

	/*************************************************************************************************/
	/* MENU                                                                                          */
	/*************************************************************************************************/

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Help            "))
		{
			ImGui::Separator();
			ImGui::MenuItem("About", NULL, &m_displayAbout);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	//if (m_exitApp)
	//{
	//	Engine::singleton_instance()->quit();
	//}

	if (m_displayAbout)
	{
		ImGui::Begin("ExcerEngine2D", &m_displayAbout, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Separator();
		ImGui::Text("Author: Ramin Amiri");
		ImGui::End();
	}

	ImGui::Text("%1.f: fps", Engine::singleton_instance()->getFramesPerSecond());

	ImGui::PushItemWidth(200);
	if (ImGui::SliderInt("X Position", &m_player->get_dst()->x, 0, 1280 - m_player->get_src()->w))
	{
		
	}

	// Main Window End
	ImGui::End();
	ImGui::EndFrame();
}

void GameState::enter()
{
	SDL_SetRenderDrawColor(Engine::singleton_instance()->get_renderer(), 0, 255, 0, 255);

	//PLAYER
	SDL_Surface* player_surface = IMG_Load("res/img/player.png");
	m_player_texture = SDL_CreateTextureFromSurface(Engine::singleton_instance()->get_renderer(), player_surface);
	SDL_FreeSurface(player_surface);

	m_player = new Player({ 0, 0, 36, 72 }, { 640, 360, 36, 72 });

	//IMAGES
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			m_images.push_back(new Image("res/img/grasstile2.png", { 0, 0, 64, 64 }, { i * 64, j * 64, 64, 64 }));

	//SOUNDS
	Engine::singleton_instance()->get_AM()->load_music("res/audio/music/gamemusic.mp3");
	Engine::singleton_instance()->get_AM()->play_music(0, -1);

	//update ImGui	
	if (m_displayUI)
		m_updateImGui();

	ConsoleIO::get_instance()->FSM_IO(this, 0); //display enter on console
}

void GameState::update(float deltaTime)
{
	//update ImGui
	if (m_displayUI)
		m_updateImGui();

	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_T))
		Engine::singleton_instance()->get_FSM()->change_state(new TitleState());

	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_A))
	{
		m_player->set_direction(-1);
		m_player->move_X(deltaTime);
	}

	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_D))
	{
		m_player->set_direction(1);
		m_player->move_X(deltaTime);
	}

	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_W))
	{
		m_player->set_direction(-1);
		m_player->move_Y(deltaTime);
	}

	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_S))
	{
		m_player->set_direction(1);
		m_player->move_Y(deltaTime);
	}
}

void GameState::render()
{
	SDL_RenderClear(Engine::singleton_instance()->get_renderer());

	//IMAGES
	for (int i = 0; i < (int)m_images.size(); i++)
		m_images[i]->render();

	SDL_RenderCopy(Engine::singleton_instance()->get_renderer(), m_player_texture, m_player->get_src(), m_player->get_dst());

	//render ImGui
	if (m_displayUI)
	{
		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());
		SDL_SetRenderDrawColor(Engine::singleton_instance()->get_renderer(), 255, 255, 255, 255);
	}

	State::render();
}

void GameState::handle_state_events()
{
	ImGuiIO& io = ImGui::GetIO();

	io.DeltaTime = Engine::singleton_instance()->get_delta_time();
	int mouseX = Engine::singleton_instance()->get_mouse_posX(), mouseY = Engine::singleton_instance()->get_mouse_posY();
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
	io.MousePos = ImVec2(static_cast<float>(Engine::singleton_instance()->get_mouse_posX()), static_cast<float>(Engine::singleton_instance()->get_mouse_posY()));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	//io.MouseWheel = static_cast<float>(wheel);

	io.DisplaySize.x = (float)Engine::singleton_instance()->get_window_width();
	io.DisplaySize.y = (float)Engine::singleton_instance()->get_window_height();

	m_ImGuiKeyMap();
	m_ImGuiSetStyle();
}

void GameState::exit()
{
	SDL_DestroyTexture(m_player_texture);
	clean_vector(m_images);

	Engine::singleton_instance()->get_AM()->clear_music();

	ConsoleIO::get_instance()->FSM_IO(this, 1); //display exit on console
}