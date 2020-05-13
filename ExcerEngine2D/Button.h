#pragma once

#include <iostream>

#include "SDL.h"

class Button
{
protected:
	enum state { NEUTRAL, HOVER, DOWN };
	bool m_b_released = false, pressed = false;
	int m_i_frame = 0;

	Sint32 mouse_posX = 0, mouse_posY = 0;

	SDL_Rect m_r_src = { 0, 0, 0, 0 };
	SDL_Rect m_r_dst = { 0, 0, 0, 0 };

	SDL_Surface* m_p_surface = nullptr;
	SDL_Texture* m_p_texture = nullptr;

public:
	Button(const char*, SDL_Rect, SDL_Rect);
	Button() {}
	~Button();
	void update();
	void render();
	bool clicked();
};