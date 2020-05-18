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
	Image();
	Image(const char*, SDL_Rect, SDL_Rect);
	~Image();

	void set_blend_mode(SDL_BlendMode blending);
	void set_alpha(Uint8 alpha);

	void render();

	void set_src(SDL_Rect src) { this->m_r_src = src; }
	void set_dst(SDL_Rect dst) { this->m_r_dst = dst; }
	void set_texture(const char* img_path);

	SDL_Rect get_src() { return m_r_src; }
	SDL_Rect get_dst() { return m_r_dst; }
	SDL_Texture* get_texture() { return m_p_texture; }
};

