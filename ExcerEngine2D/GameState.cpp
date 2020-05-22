#include "GameState.h"
#include "TitleState.h"

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
}

void GameState::exit()
{
	SDL_DestroyTexture(m_player_texture);
	clean_vector(m_images);

	Engine::singleton_instance()->get_AM()->clear_music();

	ConsoleIO::get_instance()->FSM_IO(this, 1); //display exit on console
}