#pragma once

#include "Sprite.h"

class Player : public Sprite
{
private:
	int m_i_direction = 1; //-1 for left, 1 for right

public:
	Player(SDL_Rect src, SDL_Rect dst);
	void move_X(float deltaTime);
	void move_Y(float deltaTime);
	void set_direction(int m_i_direction);
};

