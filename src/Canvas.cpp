#include "Canvas.hpp"

#include "imgui.h"

#include <unordered_map>

#define MAX_ZOOM_LEVEL 16

size_t Canvas::unique_identifier = 0;

std::unique_ptr<Canvas> Canvas::create(SDL_Renderer *renderer, const std::string& rom_path) {
	auto canvas = std::make_unique<Canvas>();

	canvas->texture = SDL_CreateTexture(
			renderer,
			TileViewer::FORMAT,
			SDL_TEXTUREACCESS_TARGET,
			WIDTH,
			HEIGHT
			);

	if(canvas->texture == NULL) {
		return nullptr;
	}

	if(!canvas->rom.load(rom_path)) {
		return nullptr;
	}

	canvas->window_name = rom_path + "##" + std::to_string(unique_identifier++);

	canvas->rom.setViewerFormat(ROM_TYPE_NES);

	return canvas;
}

void Canvas::draw(SDL_Renderer *renderer, TileViewer& tile_viewer) {
	if(focused) {
		renderToTexture(renderer, tile_viewer);
	}

	drawCanvasWindow();
}

bool Canvas::isOpen(void) const {
	return open;
}

void Canvas::renderToTexture(SDL_Renderer *renderer, TileViewer& tile_viewer) {
	SDL_Rect src;

	tile_viewer.draw(
			rom.viewer,
			rom.palette,
			offset_tiles_y
			);

	SDL_SetRenderTarget(renderer, texture);

	src = computeSrcRect();

	SDL_RenderCopy(renderer, tile_viewer.getTexture(), &src, NULL);

	renderLines(renderer);

	SDL_SetRenderTarget(renderer, NULL);
}

void Canvas::renderLines(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	int max_tiles_width = TileViewer::TILES_PER_ROW / zoom_level;
	int max_tiles_height = TileViewer::TILES_PER_COLUMN / zoom_level;

	if(zoom_level == MAX_ZOOM_LEVEL) {
		return;
	}

	for(int i = 0; i < max_tiles_width; i++) {
		SDL_Rect rect = {
			i * WIDTH / max_tiles_width - zoom_level,
			0,
			zoom_level,
			HEIGHT
		};

		SDL_RenderFillRect(
				renderer,
				&rect
				);
	}

	for(int i = 0; i < max_tiles_height; i++) {
		SDL_Rect rect = {
			0,
			i * WIDTH / max_tiles_width - zoom_level,
			WIDTH,
			zoom_level,
		};

		SDL_RenderFillRect(
				renderer,
				&rect
				);
	}
}

void Canvas::drawCanvasWindow(void) {
	ImVec2 area_available;
	focused = false;

	ImGui::Begin(window_name.c_str(), &open);
	area_available = ImGui::GetContentRegionAvail();

	handleInput();

	ImGui::VSliderInt(
			"##vslidery",
			ImVec2(24, area_available.y),
			&offset_tiles_y,
			getMaxOffsetYPerZoom(),
			0,
			""
			);

	ImGui::SameLine();

	ImGui::BeginGroup();

	ImGui::SetNextItemWidth(horizontal_slider_width);
	ImGui::SliderInt("##vsliderx", &offset_tiles_x, 0, getMaxOffsetXPerZoom(), "");

	if(offset_tiles_x >= getMaxOffsetXPerZoom()) offset_tiles_x = getMaxOffsetXPerZoom();
	if(offset_tiles_x < 0) offset_tiles_x = 0;

	if(offset_tiles_y >= getMaxOffsetYPerZoom()) offset_tiles_y = getMaxOffsetYPerZoom();
	if(offset_tiles_y < 0) offset_tiles_y = 0;

	area_available = ImGui::GetContentRegionAvail();

	float less = fminf(area_available.x, area_available.y);

	horizontal_slider_width = less;

	ImGui::Image(
			(ImTextureID) (texture),
			ImVec2(less, less)
			);

	handleClickImage();

	ImGui::EndGroup();

	ImGui::End();
}

SDL_Rect Canvas::computeSrcRect(void) const {
	SDL_Rect src;
	
	src.w = TileViewer::WIDTH / zoom_level;
	src.h = TileViewer::HEIGHT / zoom_level;

	src.x = offset_tiles_x * TileViewer::TILE_SIZE;
	src.y = 0;

	return src;
}

void Canvas::handleInput(void) {
	if(!ImGui::IsWindowFocused()) {
		return;
	}

	focused = true;

	if(ImGui::IsKeyPressed(ImGuiKey_Minus)) {
		decreaseZoom();
	}
 
	if(ImGui::IsKeyPressed(ImGuiKey_Equal) && ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
		increaseZoom();
	}

	if(ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
		offset_tiles_x--;
	}

	if(ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
		offset_tiles_x++;
	}

	if(ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
		offset_tiles_y++;
	}

	if(ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
		offset_tiles_y--;
	}

	if(ImGui::IsKeyPressed(ImGuiKey_PageDown)) {
		offset_tiles_y += TileViewer::TILES_PER_COLUMN;
	}

	if(ImGui::IsKeyPressed(ImGuiKey_PageUp)) {
		offset_tiles_y -= TileViewer::TILES_PER_COLUMN;
	}

	if(ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl)) {
		float mouse_wheel = ImGui::GetIO().MouseWheel;

		if(mouse_wheel < 0.0f) {
			decreaseZoom();
		}

		if(mouse_wheel > 0.0f) {
			increaseZoom();
		}
	}
	else {
		if(ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
			offset_tiles_x += int(-ImGui::GetIO().MouseWheel * 1.0f);
		} else {
			offset_tiles_y += int(-ImGui::GetIO().MouseWheel * 1.0f);
		}
	}

	if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_Z)) {
		undo_system.undoAction(rom.viewer);
	}

	offset_tiles_x += int(-ImGui::GetIO().MouseWheelH);

	if(ImGui::IsKeyPressed(ImGuiKey_1)) {
		selected_color = 0;
	}
	if(ImGui::IsKeyPressed(ImGuiKey_2)) {
		selected_color = 1;
	}
	if(ImGui::IsKeyPressed(ImGuiKey_3)) {
		selected_color = 2;
	}
	if(ImGui::IsKeyPressed(ImGuiKey_4)) {
		selected_color = 3;
	}
}

void Canvas::handleClickImage(void) {
	if(!ImGui::IsItemHovered()) {
		return;
	}

	if(!ImGui::IsWindowFocused()) {
		return;
	}

	ImVec2 image_pos = ImGui::GetItemRectMin();
	ImVec2 image_size = ImGui::GetItemRectSize();

	if(ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		ImVec2 mouse_pos = ImGui::GetMousePos();

		int x = (mouse_pos.x - image_pos.x) / image_size.x * TileViewer::WIDTH;
		int y = (mouse_pos.y - image_pos.y) / image_size.y * TileViewer::HEIGHT;

		undo_system.beginAction();
		putPixel(x, y, selected_color);
	} else {
		undo_system.endAction();
	}
}

void Canvas::increaseZoom(void) {
	if(zoom_level == MAX_ZOOM_LEVEL) {
		return;
	}

	zoom_level *= 2;
	offset_tiles_x += TileViewer::TILES_PER_ROW / zoom_level / 2;
	offset_tiles_y += TileViewer::TILES_PER_ROW / zoom_level / 2;
}

void Canvas::decreaseZoom(void) {
	if(zoom_level == 1) {
		return;
	}

	offset_tiles_x -= TileViewer::TILES_PER_ROW / zoom_level / 2;
	offset_tiles_y -= TileViewer::TILES_PER_ROW / zoom_level / 2;
	zoom_level /= 2;
}

int Canvas::getMaxOffsetXPerZoom(void) const {
	return TileViewer::TILES_PER_ROW - TileViewer::TILES_PER_ROW / zoom_level;
}

int Canvas::getMaxOffsetYPerZoom(void) const {
	return 
		int(
			rom.viewer.num_tiles / size_t(TileViewer::TILES_PER_ROW) +
			(rom.viewer.num_tiles % size_t(TileViewer::TILES_PER_ROW) != 0)
			) - 
		TileViewer::TILES_PER_COLUMN / zoom_level;
}

void Canvas::putPixel(int x, int y, int selected_color) {
	PixelTile px = convertToPixelTile(x, y);

	undo_system.addTile(rom.viewer, px.tile_id);

	Rom_SetTilePixelColor(
			&rom.viewer,
			px.tile_id,
			px.x,
			px.y,
			selected_color
			);
}

Canvas::PixelTile Canvas::convertToPixelTile(int x, int y) const {
	int tile_size = TileViewer::TILE_SIZE * zoom_level;

	int tile_x = (x / tile_size) + offset_tiles_x;
	int tile_y = (y / tile_size) + offset_tiles_y;

	size_t tile_id = tile_x + tile_y * TileViewer::TILES_PER_ROW;

	int inside_x = ((x % tile_size) / zoom_level) % TileViewer::TILE_SIZE;
	int inside_y = ((y % tile_size) / zoom_level) % TileViewer::TILE_SIZE;

	return PixelTile{tile_id, inside_x, inside_y};
}

Canvas::~Canvas(void) {
	if(texture != NULL) {
		SDL_DestroyTexture(texture);
	}
}
