#include "SplashState.h"

void SplashState::enter()
{
	SDL_SetRenderDrawColor(Engine::singleton_instance()->get_renderer(), 255, 255, 255, 255);

	//IMAGES
	m_v_images.push_back(new Image("res/img/excerlogo.png", { 0, 0, 495, 83 }, { (Engine::singleton_instance()->get_window_width() / 2) - (495 / 2), (Engine::singleton_instance()->get_window_height() / 2) - (83 / 2), 495, 83 }));
	ConsoleIO::get_instance()->FSM_IO(this, 0); //display enter on console
}

void SplashState::update(float deltaTime)
{
}

void SplashState::render()
{
	SDL_RenderClear(Engine::singleton_instance()->get_renderer());

	//IMAGES
	for (int i = 0; i < (int)m_v_images.size(); i++)
		m_v_images[i]->render();

	State::render();
}

void SplashState::exit()
{
	clean_vector(m_v_images);

	ConsoleIO::get_instance()->FSM_IO(this, 1); //display exit on console
}
