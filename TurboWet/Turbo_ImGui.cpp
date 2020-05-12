#include "TurboImGui.h"
#include "Engine.h"
#include "imgui/imgui_sdl.h"
#include "imgui/imgui.h"


TurboImGui::TurboImGui()
{
	ImGuiSDL::Initialize(Engine::singleton_instance()->get_renderer(), 800, 600);
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
}

TurboImGui::~TurboImGui()
{

}