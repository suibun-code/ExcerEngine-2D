#pragma once

#include "Button.h"

class ButtonToggle : private Button
{
private:
	int toggle = 0;
	void set_frame(int m_i_frame);

public:
	ButtonToggle(const char*, SDL_Rect, SDL_Rect);
	ButtonToggle();
	~ButtonToggle();

	bool swap_if_true(bool& original);
	bool clicked();
	void update();
	void render();
};
