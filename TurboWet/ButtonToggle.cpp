#include "ButtonToggle.h"
#include "SDL_image.h"
#include "Engine.h"
#include "FSM.h" //remove?

ButtonToggle::ButtonToggle(const char* img_path, SDL_Rect src, SDL_Rect dst) : Button(img_path, src, dst) {}

ButtonToggle::~ButtonToggle() 
{
	SDL_DestroyTexture(m_p_texture);
}

void ButtonToggle::set_frame(int m_i_frame)
{
	if (toggle == 0 && m_i_frame > 3)
		m_i_frame -= 3;
	else if (toggle == 1 && m_i_frame < 3)
		m_i_frame += 3;

	this->m_i_frame = m_i_frame;
}

bool ButtonToggle::swap_if_true(bool& original)
{
	bool temp = original;

	if (original == true)
		original = false;

	return temp;
}

bool ButtonToggle::clicked()
{
	if (pressed == true)
		return swap_if_true(pressed);

	return false;
}

void ButtonToggle::update()
{
	mouse_posX = Engine::singleton_instance()->get_mouse_posX();
	mouse_posY = Engine::singleton_instance()->get_mouse_posY();

	if (mouse_posX < (m_r_dst.x + m_r_dst.w) && mouse_posX > m_r_dst.x &&
		mouse_posY < (m_r_dst.y + m_r_dst.h) && mouse_posY > m_r_dst.y)
	{
		if (Engine::singleton_instance()->LM_state() && m_b_released)
		{
			pressed = true;
			(toggle == 0) ? toggle = 1 : toggle = 0;
			m_b_released = false;
			set_frame(DOWN);

			Engine::singleton_instance()->get_AM()->play_sound(0); // play the only sfx at index 0
			render(); //show clicked sprite
			Engine::singleton_instance()->wait_for(150); //wait so can show clicked sprite
		}
		else if (!Engine::singleton_instance()->LM_state())
		{
			set_frame(HOVER);
			m_b_released = true;
		}
	}
	else
		set_frame(NEUTRAL);
}

void ButtonToggle::render()
{
	m_r_src.x = m_r_src.w * m_i_frame;

	SDL_RenderCopy(Engine::singleton_instance()->get_renderer(), m_p_texture, &m_r_src, &m_r_dst);

	if (pressed == true)
	{
		SDL_RenderPresent(Engine::singleton_instance()->get_renderer());
		//pressed = false;
	}
}

