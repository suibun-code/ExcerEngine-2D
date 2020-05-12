#include "MappingState.h"
#include "TitleState.h"

template<class T>
void MappingState::clean_vector(std::vector<T> vec)
{
	for (int i = 0; i < (int)vec.size(); i++)
	{
		delete vec[i];
		vec[i] = nullptr;
	}
	vec.clear();
	vec.shrink_to_fit();
}

void MappingState::enter()
{
	SDL_SetRenderDrawColor(Engine::singleton_instance()->get_renderer(), 128, 128, 128, 255);

	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 12; j++)
		{
			grid[i][j].w = 64;
			grid[i][j].h = 64;
			grid[i][j].x = 64 * i;
			grid[i][j].y = 64 * j;
		}

	Engine::singleton_instance()->get_AM()->load_music("res/audio/music/gamemusic.mp3");
	Engine::singleton_instance()->get_AM()->play_music(0, -1);

	ConsoleIO::get_instance()->FSM_IO(this, 0); //display enter on console
}

void MappingState::update()
{
	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_T))
		Engine::singleton_instance()->get_FSM()->change_state(new TitleState());

	if (Engine::singleton_instance()->get_LMB_state())
	{
		m_v_images.push_back(new Image("res/img/grasstile2.png", { 0, 0, 64, 64 },
			{ Engine::singleton_instance()->round64(Engine::singleton_instance()->get_mouse_posX()),
			Engine::singleton_instance()->round64(Engine::singleton_instance()->get_mouse_posY()),
			64, 64 }));
	}

}

void MappingState::render()
{
	SDL_SetRenderDrawColor(Engine::singleton_instance()->get_renderer(), 215, 215, 215, 255); //make background gray and clean screen
	SDL_RenderClear(Engine::singleton_instance()->get_renderer());

	SDL_SetRenderDrawColor(Engine::singleton_instance()->get_renderer(), 0, 200, 0, 255); //print green grid
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 12; j++)
			SDL_RenderDrawRect(Engine::singleton_instance()->get_renderer(), &grid[i][j]);

	//IMAGES
	for (int i = 0; i < (int)m_v_images.size(); i++)
		if (m_v_images[i] != nullptr)
			m_v_images[i]->render();

	SDL_RenderPresent(Engine::singleton_instance()->get_renderer());

	State::render();
}

void MappingState::exit()
{
	clean_vector(m_v_images);

	Engine::singleton_instance()->get_AM()->clear_music();

	ConsoleIO::get_instance()->FSM_IO(this, 1);
}