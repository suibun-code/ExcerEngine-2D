#pragma once

class GameInstance
{
private:
	static GameInstance* gameInstance;

	// ImGui menu variables
	bool m_displayAbout = false;
	bool m_displayUI = true;

	// ImGui utility functions
	void m_ImGuiKeyMap();
	void m_ImGuiSetStyle();
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