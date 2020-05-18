#include "Image.h"

#include "SDL_image.h"
#include "Engine.h"

Image::Image()
{

}

Image::Image(const char* img_path, SDL_Rect src, SDL_Rect dst)
{
	m_p_surface = IMG_Load(img_path);
	m_p_texture = SDL_CreateTextureFromSurface(Engine::singleton_instance()->get_renderer(), m_p_surface);
	SDL_FreeSurface(m_p_surface);
	m_p_surface = NULL;

	m_r_src = src;
	m_r_dst = dst;
}

Image::~Image()
{
	SDL_DestroyTexture(m_p_texture);
}

void Image::set_blend_mode(SDL_BlendMode blending)
{

}

void Image::set_alpha(Uint8 alpha)
{

}

void Image::render()
{
	SDL_RenderCopy(Engine::singleton_instance()->get_renderer(), m_p_texture, &m_r_src, &m_r_dst);
}

void Image::set_texture(const char* img_path)
{
	m_p_surface = IMG_Load(img_path);
	m_p_texture = SDL_CreateTextureFromSurface(Engine::singleton_instance()->get_renderer(), m_p_surface);
	SDL_FreeSurface(m_p_surface);
	m_p_surface = NULL;
}
