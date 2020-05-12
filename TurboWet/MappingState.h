#pragma once

#include "FSM.h"
#include "Engine.h"
#include "ConsoleIO.h"

class MappingState : public State
{
private:
	SDL_Rect grid[20][12] = { 0, 0, 0, 0 };
	std::vector<Image*> m_v_images;

public:
	MappingState() : State("Mapping") {}
	void enter();
	void update();
	void render();
	void resume() {}
	void exit();

	template <class T>
	void clean_vector(std::vector<T> vec);
};

