#include "Button.h"
#include "SDL_image.h"
#include "Engine.h"
#include "FSM.h" //remove?

Button::Button(const char* img_path, SDL_Rect src, SDL_Rect dst)
{
	m_p_surface = IMG_Load(img_path);
	m_p_texture = SDL_CreateTextureFromSurface(Engine::singleton_instance()->get_renderer(), m_p_surface);
	SDL_FreeSurface(m_p_surface);
	m_p_surface = NULL;

	m_b_released = true; // ?
	m_r_src = src;
	m_r_dst = dst;
}

Button::~Button()
{
	SDL_DestroyTexture(m_p_texture);
}

bool Button::clicked()
{
	return m_i_frame == 2;
}

void Button::update()
{
	mouse_posX = Engine::singleton_instance()->get_mouse_posX();
	mouse_posY = Engine::singleton_instance()->get_mouse_posY();

	if (mouse_posX < (m_r_dst.x + m_r_dst.w) && mouse_posX > m_r_dst.x &&
		mouse_posY < (m_r_dst.y + m_r_dst.h) && mouse_posY > m_r_dst.y)
	{
		if (Engine::singleton_instance()->LM_state() && m_b_released)
		{
			pressed = true;
			m_b_released = false;
			m_i_frame = DOWN;

			Engine::singleton_instance()->get_AM()->play_sound(0); // play the only sfx at index 0
			render(); //show clicked sprite
			Engine::singleton_instance()->wait_for(150); //wait so can show clicked sprite
		}
		else if (!Engine::singleton_instance()->LM_state())
		{
			m_i_frame = HOVER;
			m_b_released = true;
		}
	}
	else
		m_i_frame = NEUTRAL;
}

void Button::render()
{
	m_r_src.x = m_r_src.w * m_i_frame;

	SDL_RenderCopy(Engine::singleton_instance()->get_renderer(), m_p_texture, &m_r_src, &m_r_dst);

	if (pressed == true)
	{
		SDL_RenderPresent(Engine::singleton_instance()->get_renderer());
		pressed = false;
	}
}
