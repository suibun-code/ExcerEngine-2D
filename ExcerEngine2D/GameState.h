#pragma once

#include "FSM.h"
#include "Engine.h"
#include "ConsoleIO.h"

class GameState : public State
{
private:
	std::vector<Image*> m_images;
	SDL_Texture* m_player_texture = nullptr;
	Player* m_player = nullptr;

	template <class T>
	void clean_vector(std::vector<T> vec);

	// ImGui utility functions
	void m_ImGuiKeyMap();
	void m_ImGuiSetStyle();
	void m_updateImGui();

	// ImGui menu variables
	bool m_displayAbout = false;
	bool m_displayUI = true;

public:
	GameState() : State("Game") {}
	void enter();
	void update(float deltaTime);
	void render();
	void handle_state_events() override;
	void resume() {}
	void exit();
};

