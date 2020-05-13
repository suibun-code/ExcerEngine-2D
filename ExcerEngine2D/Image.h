#pragma once

#include "SDL.h"

class Image
{
private:
	SDL_Rect m_r_src = { 0, 0, 0, 0 };
	SDL_Rect m_r_dst = { 0, 0, 0, 0 };

	SDL_Surface* m_p_surface = nullptr;
	SDL_Texture* m_p_texture = nullptr;

public:
	Image(const char*, SDL_Rect, SDL_Rect);
	~Image();

	void render();
};

