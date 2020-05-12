#include <iostream>

#include "FSM.h"
#include "Engine.h"
#include "ConsoleIO.h"

Engine* ENGINE = Engine::singleton_instance(); //pointer to the engine singleton global instance

//State
void State::render()
{
	SDL_RenderPresent(ENGINE->get_renderer());
}

void State::render_font(bool render, const char* text, int x, int y)
{
	if (render)
	{
		SDL_Color text_color = { 255, 255, 255, 255 };
		SDL_Surface* font_surface = TTF_RenderText_Solid(SDL_m_font, text, text_color);
		SDL_DestroyTexture(SDL_m_fontText); //de-allocate previous font texture
		SDL_m_fontText = SDL_CreateTextureFromSurface(ENGINE->get_renderer(), font_surface);
		SDL_m_fontRect = { x - font_surface->w, y - font_surface->h, font_surface->w, font_surface->h };
		SDL_FreeSurface(font_surface);
	}
	SDL_RenderCopy(ENGINE->get_renderer(), SDL_m_fontText, 0, &SDL_m_fontRect);
}
//End State

template<class T>
void State::clean_vector(std::vector<T> vec)
{
	for (int i = 0; i < (int)vec.size(); i++)
	{
		delete vec[i];
		vec[i] = nullptr;
	}
	vec.clear();
	vec.shrink_to_fit();
}

//StateMachine
void StateMachine::update()
{
	if (!m_v_states.empty())
		m_v_states.back()->update();
}

void StateMachine::render()
{
	if (!m_v_states.empty())
		m_v_states.back()->render();
}

void StateMachine::push_state(State* pState)
{
	m_v_states.push_back(pState);
	m_v_states.back()->enter();
}

void StateMachine::change_state(State* pState)
{
	while (!m_v_states.empty())
	{
		m_v_states.back()->exit();
		delete m_v_states.back();
		m_v_states.back() = nullptr;
		m_v_states.pop_back();
	}
	pState->enter();
	m_v_states.push_back(pState);
}

void StateMachine::pop_state()
{
	if (!m_v_states.empty())
	{
		m_v_states.back()->exit();
		delete m_v_states.back();
		m_v_states.back() = nullptr;
		m_v_states.pop_back();
	}
	m_v_states.back()->resume();
}

void StateMachine::clean()
{
	while (!m_v_states.empty())
	{
		m_v_states.back()->exit();
		delete m_v_states.back();
		m_v_states.back() = nullptr;
		m_v_states.pop_back();
	}
	std::cout << "state machine cleaned.\n";
}

StateMachine::~StateMachine()
{
	std::cout << "destroying state machine.\n";
	clean();
}
//End StateMachine

//Title
void TitleState::enter()
{ 
	SDL_SetRenderDrawColor(ENGINE->get_renderer(), 128, 0, 128, 255);

	//IMAGES
	//m_v_images.push_back(new Image("res/img/titlebg.png", { 0, 0, 1280, 720 }, { 0, 0, 1280, 720 }));
	m_v_images.push_back(new Image("res/img/turbowetlogo3.png", { 0, 0, 512, 288 }, { 25, 25, 512, 288 }));

	//BUTTONS
	ENGINE->set_LMB_state(false);
	m_v_buttons.push_back(new Button("res/img/button/play.png", { 0, 0, 256, 110 }, { 25, 585, 256, 110 }));
	m_v_buttons.push_back(new Button("res/img/button/settings.png", { 0, 0, 76, 76 }, { 1078, 619, 76, 76 }));
	m_v_buttons.push_back(new Button("res/img/button/settings.png", { 0, 0, 76, 76 }, { 977, 619, 76, 76 }));
	m_v_buttontoggles.push_back(new ButtonToggle("res/img/button/mute.png", { 0, 0, 76, 76 }, { 1179, 619, 76, 76 }));

	//FONTS
	SDL_m_font = TTF_OpenFont("font/Roboto-Regular.ttf", 12);

	//SOUNDS
	ENGINE->get_AM()->load_music("res/audio/music/titlemusic.mp3");
	ENGINE->get_AM()->set_sounds_volume(0, 15);
	ENGINE->get_AM()->play_music();

	ConsoleIO::get_instance()->FSM_IO(this, 0); //display enter on console
}

void TitleState::update()
{
	//BUTTONS
	for (int i = 0; i < (int)m_v_buttons.size(); i++)
		m_v_buttons[i]->update();

	for (int i = 0; i < (int)m_v_buttontoggles.size(); i++)
		m_v_buttontoggles[i]->update();

	if (m_v_buttons[btn::play]->clicked())
		ENGINE->get_FSM()->change_state(new GameState());
	else if (m_v_buttons[btn::settings]->clicked())
		ENGINE->get_FSM()->push_state(new SettingsState());
	else if (m_v_buttons[btn::mapper]->clicked())
		ENGINE->get_FSM()->change_state(new MappingState());
	else if (m_v_buttontoggles[btn_toggle::mute]->clicked())
		ENGINE->get_AM()->toggle_music();

	if (ENGINE->key_down(SDL_SCANCODE_G))
		ENGINE->get_FSM()->change_state(new GameState());

	if (ENGINE->key_down(SDL_SCANCODE_P))
		ENGINE->get_FSM()->push_state(new SettingsState());

	if (ENGINE->key_down(SDL_SCANCODE_A))
		SDL_SetWindowSize(ENGINE->get_window(), 1024, 576);
	if (ENGINE->key_down(SDL_SCANCODE_S))
		SDL_SetWindowSize(ENGINE->get_window(), 1152, 648);
	if (ENGINE->key_down(SDL_SCANCODE_D))
		SDL_SetWindowSize(ENGINE->get_window(), 1280, 720);
	if (ENGINE->key_down(SDL_SCANCODE_F))
		SDL_SetWindowSize(ENGINE->get_window(), 1408, 792);
}

void TitleState::render()
{
	SDL_RenderClear(ENGINE->get_renderer());

	//IMAGES
	for (int i = 0; i < (int)m_v_images.size(); i++)
		m_v_images[i]->render();

	//BUTTONS
	for (int i = 0; i < (int)m_v_buttons.size(); i++)
		m_v_buttons[i]->render();
	for (int i = 0; i < (int)m_v_buttontoggles.size(); i++)
		m_v_buttontoggles[i]->render();

	//FONTS
	render_font(true, ENGINE->version, 1280, 720); //generate version font texture

	State::render();
}

void TitleState::exit()
{
	State::clean_vector(m_v_images);
	State::clean_vector(m_v_buttons);
	State::clean_vector(m_v_buttontoggles);

	ENGINE->get_AM()->clear_music();

	TTF_CloseFont(SDL_m_font);

	ConsoleIO::get_instance()->FSM_IO(this, 1); //display exit on console
}
//End Title

//Settings
void SettingsState::enter()
{
	//if (!ENGINE->get_AM()->is_paused())
	//	ENGINE->get_AM()->toggle_music();

	//IMAGES
	m_v_images.push_back(new Image("res/img/settings_bg.png", { 0, 0, 693, 569 }, { 562, 25, 693, 569 }));

	//BUTTONS
	m_v_buttons.push_back(new Button("res/img/button/return.png", { 0, 0, 76, 76 }, { 620, 493, 76, 76 }));

	ConsoleIO::get_instance()->FSM_IO(this, 0); //display enter on console
}

void SettingsState::update()
{
	for (int i = 0; i < (int)m_v_buttons.size(); i++)
		m_v_buttons[i]->update();

	if (m_v_buttons[btn::go_back]->clicked())
		Engine::singleton_instance()->get_FSM()->pop_state();

	Engine::singleton_instance()->get_FSM()->get_states().front()->update();
}

void SettingsState::render()
{
	Engine::singleton_instance()->get_FSM()->get_states().front()->render();

	//IMAGES
	for (int i = 0; i < (int)m_v_images.size(); i++)
		m_v_images[i]->render();

	//BUTTONS
	for (int i = 0; i < (int)m_v_buttons.size(); i++)
		m_v_buttons[i]->render();

	State::render();
}

void SettingsState::exit()
{
	State::clean_vector(m_v_buttons);

	ConsoleIO::get_instance()->FSM_IO(this, 1);
}
//End Settings

//Game
void GameState::enter()
{
	SDL_SetRenderDrawColor(ENGINE->get_renderer(), 0, 255, 0, 255);

	//PLAYER
	SDL_Surface* player_surface = IMG_Load("res/img/player.png");
	player_texture = SDL_CreateTextureFromSurface(ENGINE->get_renderer(), player_surface);
	SDL_FreeSurface(player_surface);

	player = new Player({ 0, 0, 36, 72 }, { 640, 360, 36, 72 });

	//IMAGES
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			m_v_images.push_back(new Image("res/img/grasstile2.png", { 0, 0, 64, 64 }, { i * 64, j * 64, 64, 64 }));

	//SOUNDS
	ENGINE->get_AM()->load_music("res/audio/music/gamemusic.mp3");
	ENGINE->get_AM()->play_music(0, -1);

	ConsoleIO::get_instance()->FSM_IO(this, 0); //display enter on console
}

void GameState::update()
{
	if (ENGINE->key_down(SDL_SCANCODE_T))
		ENGINE->get_FSM()->change_state(new TitleState());

	if (ENGINE->key_down(SDL_SCANCODE_A))
	{
		player->set_direction(-1);
		player->move_X();
	}

	if (ENGINE->key_down(SDL_SCANCODE_D))
	{
		player->set_direction(1);
		player->move_X();
	}

	if (ENGINE->key_down(SDL_SCANCODE_W))
	{
		player->set_direction(-1);
		player->move_Y();
	}

	if (ENGINE->key_down(SDL_SCANCODE_S))
	{
		player->set_direction(1);
		player->move_Y();
	}
}

void GameState::render()
{
	SDL_RenderClear(ENGINE->get_renderer());

	//IMAGES
	for (int i = 0; i < (int)m_v_images.size(); i++)
		m_v_images[i]->render();

	SDL_RenderCopy(ENGINE->get_renderer(), player_texture, player->get_src(), player->get_dst());

	State::render();
}

void GameState::exit()
{
	SDL_DestroyTexture(player_texture);
	State::clean_vector(m_v_images);

	ENGINE->get_AM()->clear_music();

	ConsoleIO::get_instance()->FSM_IO(this, 1); //display exit on console
}
//End Game

//Mapping
void MappingState::enter()
{
	SDL_SetRenderDrawColor(ENGINE->get_renderer(), 128, 128, 128, 255);

	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 12; j++)
		{
			grid[i][j].w = 64;
			grid[i][j].h = 64;
			grid[i][j].x = 64 * i;
			grid[i][j].y = 64 * j;
		}

	ENGINE->get_AM()->load_music("res/audio/music/gamemusic.mp3");
	ENGINE->get_AM()->play_music(0, -1);

	ConsoleIO::get_instance()->FSM_IO(this, 0); //display enter on console
}

void MappingState::update()
{
	if (ENGINE->key_down(SDL_SCANCODE_T))
		ENGINE->get_FSM()->change_state(new TitleState());

	if (ENGINE->get_LMB_state())
	{
		m_v_images.push_back(new Image("res/img/grasstile2.png", { 0, 0, 64, 64 },
			{ ENGINE->round64(ENGINE->get_mouse_posX()),
			ENGINE->round64(ENGINE->get_mouse_posY()), 
			64, 64 }));
	}

}

void MappingState::render()
{
	SDL_SetRenderDrawColor(ENGINE->get_renderer(), 215, 215, 215, 255); //make background gray and clean screen
	SDL_RenderClear(ENGINE->get_renderer());

	SDL_SetRenderDrawColor(ENGINE->get_renderer(), 0, 200, 0, 255); //print green grid
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 12; j++)
			SDL_RenderDrawRect(ENGINE->get_renderer(), &grid[i][j]);

	//IMAGES
	for (int i = 0; i < (int)m_v_images.size(); i++)
		if (m_v_images[i] != nullptr)
			m_v_images[i]->render();

	SDL_RenderPresent(ENGINE->get_renderer());

	State::render();
}

void MappingState::exit()
{
	State::clean_vector(m_v_images);

	ENGINE->get_AM()->clear_music();

	ConsoleIO::get_instance()->FSM_IO(this, 1);
}
//End Mapping
