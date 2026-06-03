#include "Toolbar.hpp"
#include "imgui.h"

#include "Tool.hpp"
#include "Canvas.hpp"

void Toolbar::render(void) {
	ImGui::Begin("Toolbar");

	static constexpr ImVec2 button_dim(36.0f, 36.0f);
	ImVec2 available_area;

	for(size_t i = 0; i < Canvas::NUM_TOOLS; i++) {
		bool is_selected = (Canvas::getTool() == i);

		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

		if(ImGui::Selectable(tool_icons[i], is_selected, 0, button_dim)) {
			Canvas::setTool(static_cast<Canvas::Tool>(i));
		}

		ImGui::PopStyleVar();

		ImGui::SameLine();
		available_area = ImGui::GetContentRegionAvail();

		if(available_area.x < button_dim.x) {
			ImGui::NewLine();
		}
	}

	ImGui::End();
}
