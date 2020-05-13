#pragma once

#include "FSM.h"
#include "Engine.h"
#include "ConsoleIO.h"

class TitleState : public State
{
private:
	std::vector<Button*> m_v_buttons;
	std::vector<ButtonToggle*> m_v_buttontoggles;
	std::vector<Image*> m_v_images;
	enum btn { play, settings, mapper };
	enum btn_toggle { mute };

	template <class T>
	void clean_vector(std::vector<T> vec);

public:
	TitleState() : State("Title") {}
	void enter();
	void update(float deltaTime);
	void render();
	void resume() {}
	void exit();
};

