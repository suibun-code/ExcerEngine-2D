#pragma once

#include <vector>
#include <string>

#include "SDL_ttf.h"
#include "Button.h"
#include "ButtonToggle.h"
#include "Image.h"
#include "Player.h"

class State
{
protected:
	TTF_Font* SDL_m_font = nullptr;
	SDL_Texture* SDL_m_fontText = nullptr;
	SDL_Rect SDL_m_fontRect = { 0, 0, 0, 0 };

	std::string state_name = "State";

public:
	State(std::string state_name) : state_name(state_name) {}

	virtual void enter() = 0;
	virtual void update() = 0;
	virtual void render();
	virtual void resume() = 0;
	virtual void exit() = 0;

	template <class T>
	void clean_vector(std::vector<T> vec);

	void render_font(bool, const char*, int, int);

	std::string get_state_name() { return state_name; }
};

class StateMachine
{
private:
	std::vector<State*> m_v_states;

public:
	~StateMachine();
	void update();
	void render();
	void push_state(State* pState);
	void change_state(State* pState);
	void pop_state();
	void clean();
	std::vector<State*>& get_states() { return m_v_states; }
};

class TitleState : public State
{
private:
	std::vector<Button*> m_v_buttons;
	std::vector<ButtonToggle*> m_v_buttontoggles;
	std::vector<Image*> m_v_images;
	enum btn { play, settings, mapper };
	enum btn_toggle { mute };

public:
	TitleState() : State("Title") {}
	void enter();
	void update();
	void render();
	void resume() {}
	void exit();
};

class SettingsState : public State
{
private:
	std::vector<Button*> m_v_buttons;
	std::vector<Image*> m_v_images;
	enum btn { go_back };

public:
	SettingsState() : State("Settings") {}
	void enter();
	void update();
	void render();
	void resume() {}
	void exit();
};

class GameState : public State
{
private:
	std::vector<Image*> m_v_images;
	SDL_Texture* player_texture = nullptr;
	Player* player = nullptr;

public:
	GameState() : State("Game") {}
	void enter();
	void update();
	void render();
	void resume() {}
	void exit();
};

class MappingState : public State
{
private:
	SDL_Rect grid[20][12] = { 0, 0, 0, 0 };
	std::vector<Image*> m_v_images;

public:
	MappingState() : State("Mapping") {}
	void enter();
	void update();
	void render();
	void resume() {}
	void exit();
};
