#include "GameInstance.h"
#include "Engine.h"

//initialzie game instance
GameInstance* GameInstance::gameInstance = nullptr;

GameInstance::GameInstance()
{
	//update ImGui	
	if (Engine::singleton_instance()->is_running())
	{
		if (m_displayUI)
			m_updateImGui();
	}
}

void GameInstance::m_updateImGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(Engine::singleton_instance()->get_window());
	ImGui::NewFrame();

	if (m_UIFont != nullptr)
		ImGui::PushFont(m_UIFont);

	std::string windowString = "Game Instance";

	ImGui::SetNextWindowSize(ImVec2(150, 100), 0);

	ImGui::Begin(&windowString[0], NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar);

	ImGui::SetWindowPos(ImVec2(0, 0), true);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::Separator();
			ImGui::MenuItem("About", NULL, &m_displayAbout);
			ImGui::EndMenu();
		}

		ImGui::NextColumn();

		if (ImGui::BeginMenu("Log"))
		{
			ImGui::Separator();
			ImGui::MenuItem("Show", NULL, &m_displayLog);
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	if (m_displayAbout)
	{
		ImGui::Begin("About", &m_displayAbout, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Separator();
		ImGui::TextColored(ImVec4(0.5f, 0.f, 0.5f, 1.f), "ExcerEngine2D");
		ImGui::Text("Developed by Ramin Amiri");
		ImGui::NewLine();
		ImGui::Text("Developed using:");
		ImGui::Text("- SDL 2.0.12");
		ImGui::Text("- OpenGL 3.3");
		ImGui::Text("- ImGui");
		ImGui::NewLine();
		ImGui::Text("https://github.com/suibun-code/excerengine-2d");
		ImGui::End();
	}

	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	log.Draw("Log", &m_displayLog);

	ImGui::PopFont();

	ImGui::End();
	ImGui::EndFrame();
}

void GameInstance::render()
{
	//render ImGui
	if (m_displayUI)
	{
		ImGui::Render();
		//ImGuiSDL::Render(ImGui::GetDrawData());
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_SetRenderDrawColor(Engine::singleton_instance()->get_renderer(), 255, 255, 255, 255);
	}
}

void GameInstance::update(float deltaTime)
{
	//update ImGui
	if (m_displayUI)
		m_updateImGui();
}

void GameInstance::handle_events(const SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_GRAVE)
			if (Engine::singleton_instance()->is_game_instance_enabled() == false)
				Engine::singleton_instance()->set_game_instance(true);
			else
				Engine::singleton_instance()->set_game_instance(false);
		break;
	}
}

void GameInstance::dump_startup_log()
{
	if (!logInputBuffer.empty())
	{
		while (!logInputBuffer.empty())
		{
			log.AddLog("[%.1f sec]: %s", ImGui::GetTime(), logInputBuffer.back());
			logInputBuffer.pop_back();
			log.AddLog("\n");
		}
	}
	else
	{

		log.AddLog("GameInstance::dump_startup_log() called. Buffer is empty.");
	}
}

void GameInstance::add_log(const char* fmt, ...)
{
	if (Engine::singleton_instance()->is_running())
		log.AddLog("[%.1f sec]: %s", ImGui::GetTime(), fmt);
	//AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
	//	ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
	else
		logInputBuffer.push_back(fmt);
}
