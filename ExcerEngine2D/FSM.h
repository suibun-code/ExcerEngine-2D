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
	virtual void update(float deltaTime) = 0;
	virtual void render();
	virtual void handle_state_events();
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
	void update(float deltaTime);
	void render();
	void handle_state_events();
	void push_state(State* pState);
	void change_state(State* pState);
	void pop_state();
	void clean();
	std::vector<State*> &get_states() { return m_v_states; }
};