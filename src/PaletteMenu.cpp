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

std::unique_ptr<PaletteMenu> PaletteMenu::create(SDL_Renderer *renderer) {
	auto palette_menu = std::make_unique<PaletteMenu>();

	palette_menu->palette_texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_ARGB32,
			SDL_TEXTUREACCESS_TARGET,
			TEXTURE_WIDTH,
			TEXTURE_HEIGHT
			);

	return palette_menu;
}

void PaletteMenu::drawPaleteTexture(Canvas& canvas, SDL_Renderer *renderer) {
	const auto& palette = canvas.getRomData().palette;

	SDL_SetRenderTarget(renderer, palette_texture);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
	SDL_RenderClear(renderer);

	// retângulo branco de "escolha"
	{
		SDL_Rect rect = {
			int(canvas.getColorFg() % 4) * TILE_WIDTH,
			int(canvas.getColorFg() / 4) * TILE_HEIGHT,
			TILE_WIDTH,
			TILE_HEIGHT
		};

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderFillRect(renderer, &rect);
	}

	for(int j = 0; j < 4; j++) {
		for(int i = 0; i < 4; i++) {
			SDL_Rect rect = {
				i * TILE_WIDTH + 3,
				j * TILE_HEIGHT + 3,
				TILE_WIDTH - 6,
				TILE_HEIGHT - 6
			};

			SDL_SetRenderDrawColor(
					renderer,
					PALETTE_GET_R(palette[i + j * 4]),
					PALETTE_GET_G(palette[i + j * 4]),
					PALETTE_GET_B(palette[i + j * 4]),
					0xff
					);

			SDL_RenderFillRect(renderer, &rect);
		}
	}

	SDL_SetRenderTarget(renderer, NULL);
}

void PaletteMenu::render(Canvas& canvas) {
	ImGui::Begin("Palette Menu");
	ImGui::Text("ROM Palette");

	drawPaletteRects(
			canvas
			);

	ImGui::Separator();

	drawColorPicker(canvas);

	ImGui::Separator();

	ImGui::Text("NES Palette");
	uint32_t selected = drawMainPalette();

	if(selected) {
		canvas.getRomData().palette[canvas.getColorFg()] = famicom_palette[selected - 1];
	}

	ImGui::End();
}

PaletteMenu::~PaletteMenu(void) {
	if(palette_texture != NULL) {
		SDL_DestroyTexture(palette_texture);
	}
}

void PaletteMenu::drawPaletteRects(Canvas& canvas) {
	size_t num_colors = canvas.getRomData().getMaxColors();
	int vertical_slice = num_colors / 4;

	ImGui::Image(
			(ImTextureID) palette_texture,
			ImVec2(160, 40),
			ImVec2(0.0f, 0.0f),
			ImVec2(1.0f, float(vertical_slice) / 4)
			);

	if(!ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		return;
	}

	if(!ImGui::IsItemHovered()) {
		return;
	}

	ImVec2 image_pos = ImGui::GetItemRectMin();
	ImVec2 image_size = ImGui::GetItemRectSize();
	ImVec2 mouse_pos = ImGui::GetMousePos();

	int actual_pos_x = int((mouse_pos.x - image_pos.x) * 4 / image_size.x) % 4;
	int actual_pos_y = int((mouse_pos.y - image_pos.y) * vertical_slice / image_size.y) % vertical_slice;

	if(actual_pos_x < 0) actual_pos_x = 0;
	if(actual_pos_y < 0) actual_pos_y = 0;

	canvas.selectColorFg(actual_pos_x + actual_pos_y * 4);
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

void PaletteMenu::drawColorPicker(Canvas& canvas) {
	ImGui::Text("Color Picker (Foreground Color)");

	uint32_t& color = canvas.getRomData().palette[canvas.getColorFg()];

	fg_color[0] = float(PALETTE_GET_R(color)) / 255.0f;
	fg_color[1] = float(PALETTE_GET_G(color)) / 255.0f;
	fg_color[2] = float(PALETTE_GET_B(color)) / 255.0f;

	ImGui::ColorPicker3("##oi", fg_color);

	PALETTE_SET_R(color, fg_color[0] * 255.0f);
	PALETTE_SET_G(color, fg_color[1] * 255.0f);
	PALETTE_SET_B(color, fg_color[2] * 255.0f);
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
