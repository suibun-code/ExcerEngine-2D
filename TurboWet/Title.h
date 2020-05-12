#pragma once

#include "FSM.h"
#include "Engine.h"
#include "ConsoleIO.h"
#include "Button.h"

class Title : public State
{
private:
	Engine* ENGINE = Engine::singleton_instance();

	std::vector<Button*> m_v_buttons;
	std::vector<ButtonToggle*> m_v_buttontoggles;
	std::vector<Image*> m_v_images;
	enum btn { play, settings, mapper };
	enum btn_toggle { mute };

public:
	Title() : State("Title") {}
	void enter();
	void update();
	void render();
	void resume() {}
	void exit();
};

