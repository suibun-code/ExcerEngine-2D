#pragma once

#include <vector>

#include "imgui.h"

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

class GameInstance
{
private:
	static GameInstance* gameInstance;

	// ImGui menu variables
	bool m_displayAbout = false;
	bool m_displayUI = true;
	bool m_displayLog = false;

	// ImGui log variables
	AppLog log;
	std::vector<const char*> logInputBuffer;
	
	// ImGui fonts
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
	void update(float deltaTime);
	void handle_events();
	void dump_startup_log();
	void add_log(const char* fmt, ...) IM_FMTARGS(2);
};