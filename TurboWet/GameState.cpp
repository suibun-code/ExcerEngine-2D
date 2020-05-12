#include "GameState.h"
#include "TitleState.h"

template<class T>
void GameState::clean_vector(std::vector<T> vec)
{
	for (int i = 0; i < (int)vec.size(); i++)
	{
		delete vec[i];
		vec[i] = nullptr;
	}
	vec.clear();
	vec.shrink_to_fit();
}

void GameState::enter()
{
	SDL_SetRenderDrawColor(Engine::singleton_instance()->get_renderer(), 0, 255, 0, 255);

	//PLAYER
	SDL_Surface* player_surface = IMG_Load("res/img/player.png");
	player_texture = SDL_CreateTextureFromSurface(Engine::singleton_instance()->get_renderer(), player_surface);
	SDL_FreeSurface(player_surface);

	player = new Player({ 0, 0, 36, 72 }, { 640, 360, 36, 72 });

	//IMAGES
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			m_v_images.push_back(new Image("res/img/grasstile2.png", { 0, 0, 64, 64 }, { i * 64, j * 64, 64, 64 }));

	//SOUNDS
	Engine::singleton_instance()->get_AM()->load_music("res/audio/music/gamemusic.mp3");
	Engine::singleton_instance()->get_AM()->play_music(0, -1);

	ConsoleIO::get_instance()->FSM_IO(this, 0); //display enter on console
}

void GameState::update()
{
	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_T))
		Engine::singleton_instance()->get_FSM()->change_state(new TitleState());

	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_A))
	{
		player->set_direction(-1);
		player->move_X();
	}

	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_D))
	{
		player->set_direction(1);
		player->move_X();
	}

	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_W))
	{
		player->set_direction(-1);
		player->move_Y();
	}

	if (Engine::singleton_instance()->key_down(SDL_SCANCODE_S))
	{
		player->set_direction(1);
		player->move_Y();
	}
}

void GameState::render()
{
	SDL_RenderClear(Engine::singleton_instance()->get_renderer());

	//IMAGES
	for (int i = 0; i < (int)m_v_images.size(); i++)
		m_v_images[i]->render();

	SDL_RenderCopy(Engine::singleton_instance()->get_renderer(), player_texture, player->get_src(), player->get_dst());

	State::render();
}

void GameState::exit()
{
	SDL_DestroyTexture(player_texture);
	clean_vector(m_v_images);

	Engine::singleton_instance()->get_AM()->clear_music();

	ConsoleIO::get_instance()->FSM_IO(this, 1); //display exit on console
}