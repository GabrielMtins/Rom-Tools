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
	renderToTexture(renderer, tile_viewer);
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

	ImGui::Begin(window_name.c_str(), &open);
	area_available = ImGui::GetContentRegionAvail();

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

	ImGui::SetNextItemWidth(-1.0f);
	ImGui::SliderInt("##vsliderx", &offset_tiles_x, 0, getMaxOffsetXPerZoom(), "");

	if(offset_tiles_x >= getMaxOffsetXPerZoom()) offset_tiles_x = getMaxOffsetXPerZoom();
	if(offset_tiles_x < 0) offset_tiles_x = 0;

	if(offset_tiles_y >= getMaxOffsetYPerZoom()) offset_tiles_y = getMaxOffsetYPerZoom();
	if(offset_tiles_y < 0) offset_tiles_y = 0;

	area_available = ImGui::GetContentRegionAvail();

	float less = fminf(area_available.x, area_available.y);

	ImGui::Image(
			(ImTextureID) (texture),
			ImVec2(less, less)
			);

	ImGui::EndGroup();

	handleInput();

	ImGui::End();
}

SDL_Rect Canvas::computeSrcRect(void) const {
	SDL_Rect src;
	
	src.w = TileViewer::WIDTH / zoom_level;
	src.h = TileViewer::HEIGHT / zoom_level;

	src.x = offset_tiles_x * TileViewer::TILE_SIZE;
	src.y = 0;

	/*
	src.x = (offset_tiles_x * TileViewer::TILE_SIZE - src.w / 2);

	if(src.x < 0) src.x = 0;
	if(src.x > TileViewer::WIDTH - src.w) src.x = TileViewer::WIDTH - src.w;
	*/

	//src.y = (TileViewer::HEIGHT - src.h) / 2;
	//src.y = ((TileViewer::TILES_PER_COLUMN - TileViewer::TILES_PER_COLUMN / zoom_level) / 2) * TileViewer::TILE_SIZE;

	//if(src.y > TileViewer::HEIGHT - src.h) src.y = TileViewer::HEIGHT - src.h;

	return src;
}

void Canvas::handleInput(void) {
	if(!ImGui::IsWindowFocused()) {
		return;
	}

	if(ImGui::IsKeyPressed(ImGuiKey_Minus)) {
		decreaseZoom();
	}
 
	if(ImGui::IsKeyPressed(ImGuiKey_Equal)) {
		increaseZoom();
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

Canvas::~Canvas(void) {
	if(texture != NULL) {
		SDL_DestroyTexture(texture);
	}
}
