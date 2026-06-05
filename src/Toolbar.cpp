#include "Toolbar.hpp"
#include "imgui.h"

#include "Tool.hpp"
#include "Canvas.hpp"
#include "Palette.hpp"

constexpr const char * const tool_icons[] = {
	FOR_TOOL_LIST(TOOL_LIST_EXPAND_AS_ICON_LIST)
};

void Toolbar::render(Canvas *canvas) {
	ImGui::Begin("Toolbar");

	renderIcons();

	ImGui::Separator();

	renderPaletteColors(canvas);

	ImGui::End();
}

void Toolbar::renderPaletteColors(Canvas *active_canvas) {
	if(active_canvas == nullptr) return;

	const auto& palette = active_canvas->getRomData().palette;

	uint32_t bg_color = palette.at(active_canvas->getColorBg());
	uint32_t fg_color = palette.at(active_canvas->getColorFg());

	if(renderCurrentColors(fg_color, bg_color)) {
		active_canvas->swapColorsBgFg();
	}
}

void Toolbar::renderIcons(void) {
	static constexpr ImVec2 button_dim(36.0f, 36.0f);
	ImVec2 available_area;

	for(size_t i = 0; i < Canvas::NUM_TOOLS; i++) {
		bool is_selected = (Canvas::getTool() == i);

		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

		if(ImGui::Selectable(tool_icons[i], is_selected, 0, button_dim)) {
			Canvas::setTool(static_cast<Canvas::Tool>(i));
		}

		ImGui::PopStyleVar();

		if(i < Canvas::NUM_TOOLS - 1) {
			ImGui::SameLine();

			available_area = ImGui::GetContentRegionAvail();

			if(available_area.x < button_dim.x) {
				ImGui::NewLine();
			}
		}

	}
}

bool Toolbar::renderCurrentColors(uint32_t fg_color, uint32_t bg_color) {
	constexpr ImVec2 size(25.0f, 25.0f);
	constexpr ImVec2 offset(8.0f, 8.0f);
	constexpr ImVec2 full_size(size.x + offset.x, size.y + offset.y);
	bool pressed = false;

	ImVec2 available_space = ImGui::GetContentRegionAvail();
	float button_offset_x = (available_space.x - full_size.x) * 0.5f;

	if(button_offset_x < 0.0f) button_offset_x = 0.0f;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 p = ImGui::GetCursorScreenPos();

	p.x += button_offset_x;
	ImGui::SetCursorScreenPos(p);

	ImU32 actual_bg_color = IM_COL32(
		PALETTE_GET_R(bg_color),
		PALETTE_GET_G(bg_color),
		PALETTE_GET_B(bg_color),
		255
	);

	ImU32 actual_fg_color = IM_COL32(
		PALETTE_GET_R(fg_color),
		PALETTE_GET_G(fg_color),
		PALETTE_GET_B(fg_color),
		255
	);

	if(ImGui::InvisibleButton("##change_color", full_size)) {
		pressed = true;
	}

	ImVec2 bg_min = ImVec2(p.x + offset.x, p.y + offset.y);
	ImVec2 bg_max = ImVec2(bg_min.x + size.x, bg_min.y + size.y);
	
	draw_list->AddRectFilled(bg_min, bg_max, actual_bg_color);

	ImVec2 fg_min = p;
	ImVec2 fg_max = ImVec2(p.x + size.x, p.y + size.y);
	
	draw_list->AddRectFilled(fg_min, fg_max, actual_fg_color);

	return pressed;
}
