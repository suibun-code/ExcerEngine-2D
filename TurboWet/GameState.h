#pragma once

#include "FSM.h"
#include "Engine.h"
#include "ConsoleIO.h"

class GameState : public State
{
private:
	std::vector<Image*> m_v_images;
	SDL_Texture* player_texture = nullptr;
	Player* player = nullptr;

public:
	GameState() : State("Game") {}
	void enter();
	void update();
	void render();
	void resume() {}
	void exit();

	template <class T>
	void clean_vector(std::vector<T> vec);
};

