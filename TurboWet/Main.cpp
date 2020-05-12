#include <iostream>

#include "Engine.h"

int main(int argc, char* args[])
{
	Engine *ENGINE = Engine::singleton_instance(); //pointer to the engine singleton global instance
	ENGINE->init_all ("Turbo Wet", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ENGINE->get_window_width(), ENGINE->get_window_height(), SDL_WINDOW_RESIZABLE);

	while (ENGINE->is_running())
	{
		ENGINE->handle_events();
		if (ENGINE->tick())
		{
			ENGINE->update();
			ENGINE->render();
		}
	}

	ENGINE->clean();

	return 0;
}