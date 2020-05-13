#include "TitleState.h"

#include "MappingState.h"
#include "SettingsState.h"
#include "GameState.h"

template<class T>
void TitleState::clean_vector(std::vector<T> vec)
{
	for (int i = 0; i < (int)vec.size(); i++)
	{
		delete vec[i];
		vec[i] = nullptr;
	}
	vec.clear();
	vec.shrink_to_fit();
}

//TitleState
void TitleState::enter()
{
	SDL_SetRenderDrawColor(Engine::singleton_instance()->get_renderer(),80, 0, 80, 255);

	//IMAGES
	m_v_images.push_back(new Image("res/img/turbowetlogo3.png", { 0, 0, 512, 288 }, { 25, 25, 512, 288 }));

	//BUTTONS
	Engine::singleton_instance()->set_LMB_state(false);
	m_v_buttons.push_back(new Button("res/img/button/play.png", { 0, 0, 256, 110 }, { 25, 585, 256, 110 }));
	m_v_buttons.push_back(new Button("res/img/button/settings.png", { 0, 0, 76, 76 }, { 1078, 619, 76, 76 }));
	m_v_buttons.push_back(new Button("res/img/button/settings.png", { 0, 0, 76, 76 }, { 977, 619, 76, 76 }));
	m_v_buttontoggles.push_back(new ButtonToggle("res/img/button/mute.png", { 0, 0, 76, 76 }, { 1179, 619, 76, 76 }));

	//FONTS
	SDL_m_font = TTF_OpenFont("font/Roboto-Regular.ttf", 12);

	//SOUNDS
	Engine::singleton_instance()->get_AM()->load_music("res/audio/music/titlemusic.mp3");
	Engine::singleton_instance()->get_AM()->set_sounds_volume(0, 15);
	Engine::singleton_instance()->get_AM()->play_music();

	ConsoleIO::get_instance()->FSM_IO(this, 0); //display enter on console
}

void TitleState::update(float deltaTime)
{
	//BUTTONS
	for (int i = 0; i < (int)m_v_buttons.size(); i++)
		m_v_buttons[i]->update();

	for (int i = 0; i < (int)m_v_buttontoggles.size(); i++)
		m_v_buttontoggles[i]->update();

	if (m_v_buttons[btn::play]->clicked())
		Engine::singleton_instance()->get_FSM()->change_state(new GameState());
	else if (m_v_buttons[btn::settings]->clicked())
		Engine::singleton_instance()->get_FSM()->push_state(new SettingsState());
	else if (m_v_buttons[btn::mapper]->clicked())
		Engine::singleton_instance()->get_FSM()->change_state(new MappingState());
	else if (m_v_buttontoggles[btn_toggle::mute]->clicked())
		Engine::singleton_instance()->get_AM()->toggle_music();

	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_G))
		Engine::singleton_instance()->get_FSM()->change_state(new GameState());

	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_P))
		Engine::singleton_instance()->get_FSM()->push_state(new SettingsState());

	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_A))
		SDL_SetWindowSize(Engine::singleton_instance()->get_window(), 1024, 576);
	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_S))
		SDL_SetWindowSize(Engine::singleton_instance()->get_window(), 1152, 648);
	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_D))
		SDL_SetWindowSize(Engine::singleton_instance()->get_window(), 1280, 720);
	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_F))
		SDL_SetWindowSize(Engine::singleton_instance()->get_window(), 1408, 792);
}

void TitleState::render()
{
	SDL_RenderClear(Engine::singleton_instance()->get_renderer());

	//IMAGES
	for (int i = 0; i < (int)m_v_images.size(); i++)
		m_v_images[i]->render();

	//BUTTONS
	for (int i = 0; i < (int)m_v_buttons.size(); i++)
		m_v_buttons[i]->render();
	for (int i = 0; i < (int)m_v_buttontoggles.size(); i++)
		m_v_buttontoggles[i]->render();

	//FONTS
	render_font(true, Engine::singleton_instance()->version, 1280, 720); //generate version font texture

	State::render();
}

void TitleState::exit()
{
	clean_vector(m_v_images);
	clean_vector(m_v_buttons);
	clean_vector(m_v_buttontoggles);

	Engine::singleton_instance()->get_AM()->clear_music();

	TTF_CloseFont(SDL_m_font);

	ConsoleIO::get_instance()->FSM_IO(this, 1); //display exit on console
}