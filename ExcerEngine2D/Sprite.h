#pragma once

#include "SDL.h"

class Sprite
{
protected:
	SDL_Rect m_r_src = { 0, 0, 0, 0 };
	SDL_Rect m_r_dst = { 0, 0, 0, 0 };

public:
	Sprite(SDL_Rect src, SDL_Rect dst) : m_r_src(src), m_r_dst(dst) {}
	SDL_Rect* get_src() { return &m_r_src; }
	SDL_Rect* get_dst() { return &m_r_dst; }
	~Sprite() {}
};

