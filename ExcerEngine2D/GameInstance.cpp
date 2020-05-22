#include "GameInstance.h"
#include "Engine.h"

//initialzie game instance
GameInstance* GameInstance::gameInstance = nullptr;

struct AppLog
{
	ImGuiTextBuffer     Buffer;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
	bool                AutoScroll;  // Keep scrolling if already at the bottom.

	AppLog()
	{
		AutoScroll = true;
		Clear();
	}

	void Clear()
	{
		Buffer.clear();
		LineOffsets.clear();
		LineOffsets.push_back(0);
	}

	void AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		int old_size = Buffer.size();
		va_list args;
		va_start(args, fmt);
		Buffer.appendfv(fmt, args);
		va_end(args);
		for (int new_size = Buffer.size(); old_size < new_size; old_size++)
			if (Buffer[old_size] == '\n')
				LineOffsets.push_back(old_size + 1);
	}

	void Draw(const char* title, bool* p_open = NULL)
	{
		if (*p_open)
		{
			if (!ImGui::Begin(title, p_open))
			{
				ImGui::End();
				return;
			}

			// Options menu
			if (ImGui::BeginPopup("Options"))
			{
				ImGui::Checkbox("Auto-scroll", &AutoScroll);
				ImGui::EndPopup();
			}

			// Main window
			if (ImGui::SmallButton("[Debug] Add an entry"))
			{
				AddLog("Hello\n");

				//static int counter = 0;
				//const char* categories[3] = { "info", "warn", "error" };
				//const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
				//for (int n = 0; n < 5; n++)
				//{
				//	const char* category = categories[counter % IM_ARRAYSIZE(categories)];
				//	const char* word = words[counter % IM_ARRAYSIZE(words)];
				//	AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
				//		ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
				//	counter++;
				//}
			}
			if (ImGui::Button("Options"))
				ImGui::OpenPopup("Options");
			ImGui::SameLine();
			bool clear = ImGui::Button("Clear");
			ImGui::SameLine();
			bool copy = ImGui::Button("Copy");
			ImGui::SameLine();
			ImGui::Text("Filter:");
			ImGui::SameLine();
			Filter.Draw("", -100.0f);

			ImGui::Separator();
			ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

			if (clear)
				Clear();
			if (copy)
				ImGui::LogToClipboard();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			const char* buf = Buffer.begin();
			const char* buf_end = Buffer.end();
			if (Filter.IsActive())
			{
				// In this example we don't use the clipper when Filter is enabled.
				// This is because we don't have a random access on the result on our filter.
				// A real application processing logs with ten of thousands of entries may want to store the result of
				// search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
				for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
				{
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
					if (Filter.PassFilter(line_start, line_end))
						ImGui::TextUnformatted(line_start, line_end);
				}
			}
			else
			{
				// The simplest and easy way to display the entire buffer:
				//   ImGui::TextUnformatted(buf_begin, buf_end);
				// And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
				// to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
				// within the visible area.
				// If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
				// on your side is recommended. Using ImGuiListClipper requires
				// - A) random access into your data
				// - B) items all being the  same height,
				// both of which we can handle since we an array pointing to the beginning of each line of text.
				// When using the filter (in the block of code above) we don't have random access into the data to display
				// anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
				// it possible (and would be recommended if you want to search through tens of thousands of entries).
				ImGuiListClipper clipper;
				clipper.Begin(LineOffsets.Size);
				while (clipper.Step())
				{
					for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
					{
						const char* line_start = buf + LineOffsets[line_no];
						const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
						ImGui::TextUnformatted(line_start, line_end);
					}
				}
				clipper.End();
			}
			ImGui::PopStyleVar();

			if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			ImGui::EndChild();
			ImGui::End();
		}
	}
};

GameInstance::GameInstance()
{
	ImGuiIO& io = ImGui::GetIO();
	m_UIFont = io.Fonts->AddFontFromFileTTF("font/CircularStd-Black.ttf", 13.0f);

	//update ImGui	
	if (m_displayUI)
		m_updateImGui();
}

void GameInstance::m_updateImGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(Engine::singleton_instance()->get_window());
	ImGui::NewFrame();

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
		ImGui::Text("ExcerEngine2D");
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

	static AppLog log;
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

void GameInstance::update()
{
	//update ImGui
	if (m_displayUI)
		m_updateImGui();
}

void GameInstance::handle_events()
{
	ImGuiIO& io = ImGui::GetIO();

	io.DeltaTime = Engine::singleton_instance()->get_delta_time();
	int mouseX = Engine::singleton_instance()->get_mouse_posX(), mouseY = Engine::singleton_instance()->get_mouse_posY();
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
	io.MousePos = ImVec2(static_cast<float>(Engine::singleton_instance()->get_mouse_posX()), static_cast<float>(Engine::singleton_instance()->get_mouse_posY()));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	//io.MouseWheel = static_cast<float>(wheel);

	io.DisplaySize.x = (float)Engine::singleton_instance()->get_window_width();
	io.DisplaySize.y = (float)Engine::singleton_instance()->get_window_height();
}
