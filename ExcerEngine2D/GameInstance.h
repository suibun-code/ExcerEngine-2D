#pragma once

#include "imgui.h"

class GameInstance
{
private:
	static GameInstance* gameInstance;

	// ImGui menu variables
	bool m_displayAbout = false;
	bool m_displayUI = true;
	bool m_displayLog = false;
	
	//ImGui fonts
	ImFont* m_UIFont = nullptr;

	// ImGui utility functions
	void m_updateImGui();

public:
	static GameInstance* singleton_instance()
	{
		if (gameInstance == nullptr)
			gameInstance = new GameInstance();
		return gameInstance;
	}

	GameInstance();
	~GameInstance() {}

	void render();
	void update();
	void handle_events();
};