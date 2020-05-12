#include "Title.h"

//Title
void Title::enter()
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

void Title::update()
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

void Title::render()
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

void Title::exit()
{
	State::clean_vector(m_v_images);
	State::clean_vector(m_v_buttons);
	State::clean_vector(m_v_buttontoggles);

	ENGINE->get_AM()->clear_music();

	TTF_CloseFont(SDL_m_font);

	ConsoleIO::get_instance()->FSM_IO(this, 1); //display exit on console
}
//End Title