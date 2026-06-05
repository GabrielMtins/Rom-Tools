#include "PaletteMenu.hpp"
#include "App.hpp"

#define FAMICOM_PALETTE_SIZE 64

static constexpr std::array<uint32_t, FAMICOM_PALETTE_SIZE> famicom_palette = {
	0xFF7C7C7C, 0xFF0000FC, 0xFF0000BC, 0xFF4428BC, 0xFF940084, 0xFFA80020, 0xFFA81000, 0xFF881400,
	0xFF503000, 0xFF007800, 0xFF006800, 0xFF005800, 0xFF004058, 0xFF000000, 0xFF000000, 0xFF000000,

	0xFFBCBCBC, 0xFF0078F8, 0xFF0058F8, 0xFF6844FC, 0xFFD800CC, 0xFFE40058, 0xFFF83800, 0xFFE45C10,
	0xFFAC7C00, 0xFF00B800, 0xFF00A800, 0xFF00A844, 0xFF008888, 0xFF000000, 0xFF000000, 0xFF000000,

	0xFFF8F8F8, 0xFF3CBCFC, 0xFF6888FC, 0xFF9878F8, 0xFFF878F8, 0xFFF85898, 0xFFF87858, 0xFFFCA044,
	0xFFF8B800, 0xFFB8F818, 0xFF58D854, 0xFF58F898, 0xFF00E8D8, 0xFF787878, 0xFF000000, 0xFF000000,

	0xFFF8F8F8, 0xFFA4E4FC, 0xFFB8B8F8, 0xFFD8B8F8, 0xFFF8B8F8, 0xFFF8A4C0, 0xFFF0D0B0, 0xFFECE0A8,
	0xFFF8D878, 0xFFD8F878, 0xFFB8F8B8, 0xFFB8F8D8, 0xFF00FCFC, 0xFFD8D8D8, 0xFF000000, 0xFF000000
};

void PaletteMenu::render(Canvas& canvas) {
	ImGui::Begin("Palette Menu", &open);
	ImGui::Text("User Palette");

	drawPaletteRects(
			canvas,
			canvas.getRomData().palette,
			canvas.getRomData().getMaxColors()
			);

	ImGui::Separator();

	ImGui::Text("NES Palette");
	uint32_t selected = drawMainPalette();


	ImGui::End();
}

void PaletteMenu::drawPaletteRects(Canvas& canvas, const Palette& palette, size_t num_colors) {
	static constexpr ImVec2 rect_size(48, 48);

	ImGui::BeginGroup();

	ImDrawList *draw_list = ImGui::GetWindowDrawList();
	ImVec2 cursor_pos = ImGui::GetCursorScreenPos();

	for(size_t i = 0; i < num_colors; i++) {
		ImVec2 rect_min = ImVec2(cursor_pos.x + (i * rect_size.x), cursor_pos.y);
		ImVec2 rect_max = ImVec2(rect_min.x + rect_size.x, rect_min.y + rect_size.y);

		uint32_t raw_color = palette[i];
		ImU32 current_color = IM_COL32(
			PALETTE_GET_R(raw_color),
			PALETTE_GET_G(raw_color),
			PALETTE_GET_B(raw_color),
			255
		);

		draw_list->AddRectFilled(rect_min, rect_max, current_color);

		if(ImGui::IsMouseHoveringRect(rect_min, rect_max)) {
			if(ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
				canvas.selectColorFg(i);
			} else if(ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
				canvas.selectColorBg(i);
			}
		}
	}


	ImGui::Dummy(ImVec2(rect_size.x * num_colors, rect_size.y));

	ImGui::EndGroup();
}

uint32_t PaletteMenu::drawMainPalette(void) {
	static constexpr ImVec2 button_size(24, 24);
	uint32_t selected = 0;

	ImGui::BeginGroup();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

	for(size_t i = 0; i < FAMICOM_PALETTE_SIZE; i++) {
		ImVec4 current_color = getVec4Color(famicom_palette[i]);

		ImGui::PushStyleColor(ImGuiCol_Button, current_color);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, getBrightColor(current_color));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, getDimmedColor(current_color));

		bool clicked = ImGui::Button(
				("##famcolor" + std::to_string(i)).c_str(),
				button_size
				);

		if(clicked) {
			selected = i + 1;
		}

		ImGui::PopStyleColor(3);

		if(i + 1 == FAMICOM_PALETTE_SIZE) {
			continue;
		}

		if(i % 16 != 15) {
			ImGui::SameLine();
		}
	}

	ImGui::PopStyleVar(4);

	ImGui::EndGroup();

	return selected;
}

ImVec4 PaletteMenu::getVec4Color(uint32_t color) {
	return ImVec4(
			float(PALETTE_GET_R(color)) / 255.0f,
			float(PALETTE_GET_G(color)) / 255.0f,
			float(PALETTE_GET_B(color)) / 255.0f,
			1.0f
			);
}

ImVec4 PaletteMenu::getDimmedColor(const ImVec4& color) {
	return ImVec4(
			color.x * 0.5f,
			color.y * 0.5f,
			color.z * 0.5f,
			color.w
			);
}

ImVec4 PaletteMenu::getBrightColor(const ImVec4& color) {
	return ImVec4(
			fminf(color.x + 0.8f, 1.0f),
			fminf(color.y + 0.8f, 1.0f),
			fminf(color.z + 0.8f, 1.0f),
			color.w
			);
}
