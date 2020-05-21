#include <iostream>

#include "Engine.h"
#include "GameInstance.h"

int main(int argc, char* args[])
{
	Engine::singleton_instance()->init_all("ExcerEngine2D Demo Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Engine::singleton_instance()->get_window_width(), Engine::singleton_instance()->get_window_height(), SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	while (Engine::singleton_instance()->is_running())
	{
		SDL_PumpEvents();
		Engine::singleton_instance()->handle_events();
		if (Engine::singleton_instance()->tick())
		{
			Engine::singleton_instance()->update(Engine::singleton_instance()->get_delta_time());
			Engine::singleton_instance()->render();
		}
	}

	Engine::singleton_instance()->clean();

	return 0;
}