#include <iostream>

#include "FSM.h"
#include "Engine.h"

//State
void State::render()
{
	SDL_RenderPresent(Engine::singleton_instance()->get_renderer());
}

void State::handle_state_events()
{
	
}

void State::render_font(bool render, const char *text, int x, int y)
{
	if (render)
	{
		SDL_Color text_color = { 255, 255, 255, 255 };
		SDL_Surface* font_surface = TTF_RenderText_Solid(SDL_m_font, text, text_color);
		SDL_DestroyTexture(SDL_m_fontText); //de-allocate previous font texture
		SDL_m_fontText = SDL_CreateTextureFromSurface(Engine::singleton_instance()->get_renderer(), font_surface);
		SDL_m_fontRect = { x - font_surface->w, y - font_surface->h, font_surface->w, font_surface->h };
		SDL_FreeSurface(font_surface);
	}
	SDL_RenderCopy(Engine::singleton_instance()->get_renderer(), SDL_m_fontText, 0, &SDL_m_fontRect);
}
//End State

template<class T>
void State::clean_vector(std::vector<T> vec)
{
	for (int i = 0; i < (int)vec.size(); i++)
	{
		delete vec[i];
		vec[i] = nullptr;
	}
	vec.clear();
	vec.shrink_to_fit();
}

//StateMachine
void StateMachine::update(float deltaTime)
{
	if (!m_v_states.empty())
		m_v_states.back()->update(deltaTime);
}

void StateMachine::render()
{
	if (!m_v_states.empty())
		m_v_states.back()->render();
}

void StateMachine::handle_state_events()
{
	if (!m_v_states.empty())
		m_v_states.back()->handle_state_events();
}

void StateMachine::push_state(State *pState)
{
	m_v_states.push_back(pState);
	m_v_states.back()->enter();
}

void StateMachine::change_state(State *pState)
{
	while (!m_v_states.empty())
	{
		m_v_states.back()->exit();
		delete m_v_states.back();
		m_v_states.back() = nullptr;
		m_v_states.pop_back();
	}
	pState->enter();
	m_v_states.push_back(pState);
}

void StateMachine::pop_state()
{
	if (!m_v_states.empty())
	{
		m_v_states.back()->exit();
		delete m_v_states.back();
		m_v_states.back() = nullptr;
		m_v_states.pop_back();
	}
	m_v_states.back()->resume();

	std::cout << "popped";
}

void StateMachine::clean()
{
	while (!m_v_states.empty())
	{
		m_v_states.back()->exit();
		delete m_v_states.back();
		m_v_states.back() = nullptr;
		m_v_states.pop_back();
	}
	std::cout << "state machine cleaned.\n";
}

StateMachine::~StateMachine()
{
	std::cout << "destroying state machine.\n";
	clean();
}
//End StateMachine