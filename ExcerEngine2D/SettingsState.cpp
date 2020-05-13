#include "SettingsState.h"

template<class T>
void SettingsState::clean_vector(std::vector<T> vec)
{
	for (int i = 0; i < (int)vec.size(); i++)
	{
		delete vec[i];
		vec[i] = nullptr;
	}
	vec.clear();
	vec.shrink_to_fit();
}

void SettingsState::enter()
{
	//if (!Engine::singleton_instance()->get_AM()->is_paused())
	//	Engine::singleton_instance()->get_AM()->toggle_music();

	//IMAGES
	m_v_images.push_back(new Image("res/img/settings_bg.png", { 0, 0, 693, 569 }, { 562, 25, 693, 569 }));

	//BUTTONS
	m_v_buttons.push_back(new Button("res/img/button/return.png", { 0, 0, 76, 76 }, { 620, 493, 76, 76 }));

	ConsoleIO::get_instance()->FSM_IO(this, 0); //display enter on console
}

void SettingsState::update(float deltaTime)
{
	for (int i = 0; i < (int)m_v_buttons.size(); i++)
		m_v_buttons[i]->update();

	if (m_v_buttons[btn::go_back]->clicked())
		Engine::singleton_instance()->get_FSM()->pop_state();

	Engine::singleton_instance()->get_FSM()->get_states().front()->update(deltaTime);
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
	clean_vector(m_v_buttons);

	ConsoleIO::get_instance()->FSM_IO(this, 1);
}