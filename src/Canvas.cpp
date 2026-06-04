#include "Canvas.hpp"

#include "imgui.h"
#include "Toolbar.hpp"

#include "App.hpp"

#define MAX_ZOOM_LEVEL 16
#define sign(x) ((x) > 0 ? 1 : -1)

size_t Canvas::unique_identifier = 0;
TileBuffer Canvas::tile_copy_buffer;
std::unique_ptr<TileViewer> Canvas::viewer_copy = nullptr;
uint8_t Canvas::selected_color = 0;
Canvas::Tool Canvas::tool = Canvas::TOOL_SELECT;
Canvas::Tool Canvas::old_tool = Canvas::TOOL_SELECT;

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

	if(viewer_copy == nullptr) {
		viewer_copy = TileViewer::create(renderer);
	}

	return canvas;
}

void Canvas::renderFramebuffer(SDL_Renderer *renderer, TileViewer& tile_viewer) {
	if(is_on_focus) {
		renderToTexture(renderer, tile_viewer);
	}
}

void Canvas::setTool(Tool new_tool) {
	if(new_tool != tool) {
		old_tool = tool;
	}

	tool = new_tool;
}

Canvas::Tool Canvas::getTool(void) {
	return tool;
}

bool Canvas::isOpen(void) const {
	return open;
}

bool Canvas::isOnFocus(void) const {
	return is_on_focus;
}

void Canvas::renderToTexture(SDL_Renderer *renderer, TileViewer& tile_viewer) {
	SDL_Rect src;

	tile_viewer.draw(
			rom.viewer,
			rom.palette,
			offset_tiles_y
			);

	renderToolLine(tile_viewer);

	SDL_SetRenderTarget(renderer, texture);

	src = computeSrcRect();

	SDL_RenderCopy(renderer, tile_viewer.getTexture(), &src, NULL);

	renderPaste(renderer);
	renderToolRect(renderer);
	renderLines(renderer);
	renderSelectRect(renderer);

	SDL_SetRenderTarget(renderer, NULL);
}

void Canvas::renderSelectRect(SDL_Renderer *renderer) {
	if(!tools.select.selected) {
		return;
	}

	int tile_size = getTileSizeZoomed();

	SDL_Rect rect = {
		(tools.select.tile_rect.x - offset_tiles_x) * tile_size,
		(tools.select.tile_rect.y - offset_tiles_y) * tile_size,
		(tools.select.tile_rect.w) * tile_size,
		(tools.select.tile_rect.h) * tile_size,
	};

	SDL_Rect masks[4] = {
		{0, 0, WIDTH, rect.y - zoom_level},
		{0, rect.y + rect.h, WIDTH, HEIGHT - (rect.y + rect.h) + zoom_level},
		{0, rect.y - zoom_level, rect.x - zoom_level, rect.h + zoom_level},
		{rect.x + rect.w, rect.y - zoom_level, WIDTH - (rect.x + rect.w), rect.h + zoom_level},
	};

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);

	for(int i = 0; i < 4; i++) {
		SDL_RenderFillRect(renderer, &masks[i]);
	}
}

void Canvas::renderLines(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 0xaa, 0xaa, 0xaa, 0xff);
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

void Canvas::renderPaste(SDL_Renderer *renderer) {
	if(image_input_type != IMAGE_INPUT_PASTE) {
		return;
	}

	viewer_copy->drawBuffer(tile_copy_buffer, rom.palette);
	int tile_size = getTileSizeZoomed();

	SDL_Rect src = {
		0, 0,
		int(tile_copy_buffer.width) * TileViewer::TILE_SIZE,
		int(tile_copy_buffer.height) * TileViewer::TILE_SIZE
	};

	SDL_Rect dst = {
		(tools.paste.x - offset_tiles_x) * tile_size,
		(tools.paste.y - offset_tiles_y) * tile_size,
		int(tile_copy_buffer.width) * tile_size,
		int(tile_copy_buffer.height) * tile_size,
	};

	SDL_RenderCopy(
			renderer,
			viewer_copy->getTexture(),
			&src,
			&dst
			);

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x80);
	SDL_RenderFillRect(renderer, &dst);
}

void Canvas::renderToolRect(SDL_Renderer *renderer) {
	if(!tools.rect.selected) {
		return;
	}

	uint8_t r, g, b;

	r = (rom.palette.at(selected_color) >> 16) & 0xff;
	g = (rom.palette.at(selected_color) >> 8) & 0xff;
	b = (rom.palette.at(selected_color)) & 0xff;

	SDL_Rect dst = tools.rect.px_rect;

	dst.x -= offset_tiles_x * TileViewer::TILE_SIZE;
	dst.y -= offset_tiles_y * TileViewer::TILE_SIZE;

	dst.x *= (zoom_level) * canvas_view_ratio;
	dst.y *= (zoom_level) * canvas_view_ratio;
	dst.w *= (zoom_level) * canvas_view_ratio;
	dst.h *= (zoom_level) * canvas_view_ratio;

	SDL_SetRenderDrawColor(renderer, r, g, b, 0xff);
	SDL_RenderFillRect(renderer, &dst);
}

void Canvas::renderToolLine(TileViewer& tile_viewer) {
	if(!tools.line.active) {
		return;
	}

	tile_viewer.drawLine(
			tools.line.start_x,
			tools.line.start_y - offset_tiles_y * TileViewer::TILE_SIZE,
			tools.line.end_x,
			tools.line.end_y - offset_tiles_y * TileViewer::TILE_SIZE,
			rom.palette.at(selected_color)
			);
}

void Canvas::drawCanvasWindow(const App& app) {
	ImVec2 area_available;
	is_on_focus = false;

	ImGui::Begin(window_name.c_str(), &open, window_flags);

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

	doOffsetCorrection();

	area_available = ImGui::GetContentRegionAvail();

	float less = fminf(area_available.x, area_available.y);

	horizontal_slider_width = less;

	ImGui::Image(
			(ImTextureID) (texture),
			ImVec2(less, less)
			);

	handleClickImage(app);

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

	is_on_focus = true;

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

	if(ImGui::IsKeyPressed(ImGuiKey_Space)) {
		image_input_type = IMAGE_INPUT_MOVE;
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

	if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_Y)) {
		undo_system.redoAction(rom.viewer);
	}

	if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_C)) {
		if(tools.select.selected) {
			copyFromViewerToBuffer(
					tools.select.tile_rect.x,
					tools.select.tile_rect.y,
					tools.select.tile_rect.w,
					tools.select.tile_rect.h,
					tile_copy_buffer
					);

			tools.select.selected = false;
		}
	}

	if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_X)) {
		if(tools.select.selected) {
			copyFromViewerToBuffer(
					tools.select.tile_rect.x,
					tools.select.tile_rect.y,
					tools.select.tile_rect.w,
					tools.select.tile_rect.h,
					tile_copy_buffer
					);

			undo_system.beginAction();
			floodVisible(0, true);
			undo_system.endAction(rom.viewer);

			tools.select.selected = false;
		}
	}

	if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_V)) {
		image_input_type = IMAGE_INPUT_PASTE;
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

	if(ImGui::IsKeyPressed(ImGuiKey_Escape)) {
		tools.select.selected = false;
	}

	if(ImGui::IsKeyPressed(ImGuiKey_Delete)) {
		undo_system.beginAction();
		floodVisible(0, true);
		undo_system.endAction(rom.viewer);

		tools.select.selected = false;
	}
}

void Canvas::handleClickImage(const App& app) {
	if(!ImGui::IsItemHovered()) {
		window_flags = 0;
		return;
	}

	if(!ImGui::IsWindowFocused() && app.isAnyCanvasOnFocus()) {
		wait_for_mouse_button_release = true;
		return;
	}

	if(wait_for_mouse_button_release) {
		if(!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
			wait_for_mouse_button_release = false;
		}

		return;
	}

	switch(image_input_type) {
		case IMAGE_INPUT_PASTE:
			handleInputPaste();
			return;

		case IMAGE_INPUT_MOVE:
			handleInputMove();
			return;

		default:
			break;
	}

	window_flags = ImGuiWindowFlags_NoMove;

	switch(tool) {
		FOR_TOOL_LIST(TOOL_LIST_EXPAND_AS_CASE_FUNC)

		default:
			break;
	}
}

void Canvas::handleToolBrush(void) {
	ImVec2 pos = getIntegerPositionOnViewer();

	if(!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		if(!tools.brush.active) return;

		tools.brush.active = false;
		undo_system.endAction(rom.viewer);
		return;
	}

	if(!tools.brush.active) {
		tools.brush.active = true;

		undo_system.beginAction();
		tools.brush.old_x = pos.x;
		tools.brush.old_y = pos.y;

		return;
	}

	bresenhamLine(
			tools.brush.old_x,
			tools.brush.old_y,
			pos.x,
			pos.y,
			selected_color,
			true
			);

	tools.brush.old_x = pos.x;
	tools.brush.old_y = pos.y;
}

void Canvas::handleToolSelect(void) {
	ImVec2 pos = getTilePositionOnViewer();
	int w, h;

	if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		tools.select.tile_start_x = pos.x;
		tools.select.tile_start_y = pos.y;
		tools.select.selected = true;
	}

	if(ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		tools.select.tile_end_x = pos.x;
		tools.select.tile_end_y = pos.y;
	}

	w = tools.select.tile_end_x - tools.select.tile_start_x;
	h = tools.select.tile_end_y - tools.select.tile_start_y;

	if(std::abs(w) >= TileViewer::TILES_PER_ROW) {
		w = sign(w) * (TileViewer::TILES_PER_ROW - 1);
	}

	if(std::abs(h) >= TileViewer::TILES_PER_COLUMN) {
		h = sign(h) * (TileViewer::TILES_PER_COLUMN - 1);
	}

	tools.select.tile_rect = SDL_Rect{
		((w >= 0) ? (tools.select.tile_start_x) : (tools.select.tile_end_x)),
		((h >= 0) ? (tools.select.tile_start_y) : (tools.select.tile_end_y)),
		((w >= 0) ? (w + 1) : (-w + 1)),
		((h >= 0) ? (h + 1) : (-h + 1)),
	};
}

void Canvas::handleToolBucket(void) {
	if(!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		return;
	}
	
	bool fill_all = ImGui::IsKeyDown(ImGuiKey_LeftShift);

	if(fill_all) {
		undo_system.beginAction();
		floodVisible(selected_color, true);
		undo_system.endAction(rom.viewer);

		return;
	}

	ImVec2 pos = getIntegerPositionOnViewer();

	undo_system.beginAction();
	floodFill(pos.x, pos.y, selected_color, true);
	undo_system.endAction(rom.viewer);
}

void Canvas::handleToolInvert(void) {
	if(!ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		return;
	}

	bool horizontal = !ImGui::IsKeyDown(ImGuiKey_LeftShift);

	SDL_Rect rect = tools.select.tile_rect;

	if(!tools.select.selected) {
		ImVec2 pos = getTilePositionOnViewer();

		rect.x = pos.x;
		rect.y = pos.y;
		rect.w = 1;
		rect.h = 1;
	}

	copyFromViewerToBuffer(rect.x, rect.y, rect.w, rect.h, tile_tmp_buffer);

	tile_tmp_buffer.invert(horizontal);

	undo_system.beginAction();

	copyFromBufferToViewer(rect.x, rect.y, tile_tmp_buffer);

	undo_system.endAction(rom.viewer);
}

void Canvas::handleInputPaste(void) {
	if(ImGui::IsKeyPressed(ImGuiKey_Escape)) {
		image_input_type = IMAGE_INPUT_TOOL;
		return;
	}

	ImVec2 pos = getTilePositionOnViewer();

	tools.paste.x = pos.x;
	tools.paste.y = pos.y;

	if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		undo_system.beginAction();

		copyFromBufferToViewer(pos.x, pos.y, tile_copy_buffer);

		undo_system.endAction(rom.viewer);

		image_input_type = IMAGE_INPUT_TOOL;
	}
}

void Canvas::handleToolRect(void) {
	ImVec2 pos = getIntegerPositionOnViewer();

	if(!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		if(!tools.rect.selected) {
			return;
		}

		undo_system.beginAction();

		const auto& rect = tools.rect.px_rect;

		for(int j = 0; j < rect.h; j++) {
			for(int i = 0; i < rect.w; i++) {
				putPixel(
						rect.x + i,
						rect.y + j,
						selected_color,
						true
						);
			}
		}

		undo_system.endAction(rom.viewer);

		tools.rect.selected = false;
		return;
	}

	if(!tools.rect.selected) {
		tools.rect.start_x = pos.x;
		tools.rect.start_y = pos.y;
		tools.rect.selected = true;
	}

	tools.rect.end_x = pos.x;
	tools.rect.end_y = pos.y;

	{
		int start_x = tools.rect.start_x;
		int start_y = tools.rect.start_y;
		int end_x = tools.rect.end_x;
		int end_y = tools.rect.end_y;

		int width, height;

		if(start_x > end_x) std::swap(start_x, end_x);
		if(start_y > end_y) std::swap(start_y, end_y);

		width = end_x - start_x + 1;
		height = end_y - start_y + 1;

		tools.rect.px_rect = SDL_Rect{
			start_x, start_y, width, height
		};
	}
}

void Canvas::handleToolLine(void) {
	ImVec2 pos = getIntegerPositionOnViewer();

	if(!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		if(!tools.line.active) return;

		tools.line.active = false;

		undo_system.beginAction();

		bresenhamLine(
				tools.line.start_x,
				tools.line.start_y,
				tools.line.end_x,
				tools.line.end_y,
				selected_color
				);

		undo_system.endAction(rom.viewer);
		return;
	}

	if(!tools.line.active) {
		tools.line.start_x = pos.x;
		tools.line.start_y = pos.y;
		tools.line.active = true;
	}

	tools.line.end_x = pos.x;
	tools.line.end_y = pos.y;

	if(ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
		static constexpr int pixel_lim = 4;

		int dx = (pos.x - tools.line.start_x);
		int dy = (pos.y - tools.line.start_y);

		if(std::abs(dx) < pixel_lim) {
			tools.line.end_x = tools.line.start_x;
		} else if (std::abs(dy) < pixel_lim) {
			tools.line.end_y = tools.line.start_y;
		} else {
			int coef_ang = (std::abs(dy) * 100) / (std::abs(dx));
			static constexpr int tan_45 = 100;
			static constexpr int ang_lim = 20;

			if(std::abs(coef_ang - tan_45) < ang_lim) {
				int dtot = std::min(std::abs(dx), std::abs(dy));
				tools.line.end_x = tools.line.start_x + dtot * sign(dx); 
				tools.line.end_y = tools.line.start_y + dtot * sign(dy); 

			}
		}
	}
}

void Canvas::handleInputMove(void) {
	if(!ImGui::IsKeyDown(ImGuiKey_Space)) {
		image_input_type = IMAGE_INPUT_TOOL;
		tools.move.active = false;
		return;
	}

	ImVec2 pos = getNormalPositionOnCanvas();

	if(!tools.move.active) {
		tools.move.offset_tiles_x = offset_tiles_x;
		tools.move.offset_tiles_y = offset_tiles_y;

		tools.move.pos_start = pos;

		tools.move.active = true;

		return;
	}

	const auto& old_pos = tools.move.pos_start;

	int tile_dist_x = roundf((old_pos.x - pos.x) * (TileViewer::TILES_PER_ROW / zoom_level));
	int tile_dist_y = roundf((old_pos.y - pos.y) * (TileViewer::TILES_PER_COLUMN / zoom_level));

	offset_tiles_x = tools.move.offset_tiles_x + tile_dist_x;
	offset_tiles_y = tools.move.offset_tiles_y + tile_dist_y;

	doOffsetCorrection();
}

void Canvas::floodVisible(uint8_t selected_color, bool check_for_selection) {
	int x, y, w, h;

	x = offset_tiles_x * TileViewer::TILE_SIZE;
	y = offset_tiles_y * TileViewer::TILE_SIZE;

	w = TileViewer::WIDTH / zoom_level;
	h = TileViewer::HEIGHT / zoom_level;
	
	for(int j = 0; j < h; j++) {
		for(int i = 0; i < w; i++) {
			putPixel(x + i, y + j, selected_color, check_for_selection);
		}
	}
}

void Canvas::floodFill(int start_x, int start_y, uint8_t selected_color, bool check_for_selection) {
	uint8_t first_color = getPixel(start_x, start_y);

	if(first_color == selected_color) {
		return;
	}

	std::vector<std::pair<int, int>> pixel_list;

	pixel_list.emplace_back(start_x, start_y);

	while(!pixel_list.empty()) {
		auto [x, y] = pixel_list.back();
		pixel_list.pop_back();

		if(!isPixelInsideCanvasView(x, y)) {
			continue;
		}

		uint8_t c = getPixel(x, y);

		if(c != first_color) {
			continue;
		}

		if(!putPixel(x, y, selected_color, check_for_selection)) {
			continue;
		}

		pixel_list.emplace_back(x + 1, y);
		pixel_list.emplace_back(x - 1, y);
		pixel_list.emplace_back(x, y + 1);
		pixel_list.emplace_back(x, y - 1);
	}
}

ImVec2 Canvas::getNormalPositionOnCanvas(void) const {
	ImVec2 image_pos = ImGui::GetItemRectMin();
	ImVec2 image_size = ImGui::GetItemRectSize();
	ImVec2 mouse_pos = ImGui::GetMousePos();

	return ImVec2(
			(mouse_pos.x - image_pos.x) / image_size.x,
			(mouse_pos.y - image_pos.y) / image_size.y
			);
}

ImVec2 Canvas::getIntegerPositionOnViewer(void) const {
	ImVec2 pos = getNormalPositionOnCanvas();

	return ImVec2(
			pos.x * TileViewer::WIDTH / zoom_level + offset_tiles_x * TileViewer::TILE_SIZE,
			pos.y * TileViewer::HEIGHT / zoom_level + offset_tiles_y * TileViewer::TILE_SIZE
			);
}

ImVec2 Canvas::getTilePositionOnViewer(void) const {
	ImVec2 pos = getNormalPositionOnCanvas();

	return ImVec2(
			floorf(pos.x * (TileViewer::TILES_PER_ROW / zoom_level)) + offset_tiles_x,
			floorf(pos.y * (TileViewer::TILES_PER_COLUMN / zoom_level)) + offset_tiles_y
			);
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

void Canvas::doOffsetCorrection(void) {
	int max_x, max_y;

	max_x = getMaxOffsetXPerZoom();
	max_y = getMaxOffsetYPerZoom();

	if(offset_tiles_x >= max_x) offset_tiles_x = max_x;
	if(offset_tiles_x < 0) offset_tiles_x = 0;

	if(offset_tiles_y >= max_y) offset_tiles_y = max_y;
	if(offset_tiles_y < 0) offset_tiles_y = 0;
}

int Canvas::getTileSizeZoomed(void) const {
	return (WIDTH / TileViewer::WIDTH) * TileViewer::TILE_SIZE * zoom_level;
}

bool Canvas::isTileInsideSelection(size_t tile_id) const {
	int x = tile_id % TileViewer::TILES_PER_ROW;
	int y = tile_id / TileViewer::TILES_PER_ROW;

	if(!tools.select.selected) {
		return false;
	}

	const SDL_Rect& r = tools.select.tile_rect;

	if(x < r.x || x >= r.x + r.w || y < r.y || y >= r.y + r.h) {
		return false;
	}

	return true;
}

bool Canvas::putPixel(int x, int y, uint8_t selected_color, bool check_for_selection) {
	PixelTile px = convertToPixelTile(x, y);

	if(check_for_selection && tools.select.selected) {
		if(!isTileInsideSelection(px.tile_id)) {
			return false;
		}
	}

	if(Rom_GetTilePixelColor(&rom.viewer, px.tile_id, px.x, px.y) == selected_color) {
		return false;
	}

	undo_system.addTile(rom.viewer, px.tile_id);

	Rom_SetTilePixelColor(
			&rom.viewer,
			px.tile_id,
			px.x,
			px.y,
			selected_color
			);

	return true;
}

void Canvas::bresenhamLine(int x1, int y1, int x2, int y2, uint8_t selected_color, bool check_for_selection) {
	int dx = std::abs(x2 - x1);
	int dy = std::abs(y2 - y1);

	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;

	int err = dx - dy;
	int x = x1, y = y1;

	while(1) {
		putPixel(x, y, selected_color, check_for_selection);

		if(x == x2 && y == y2) break;

		int new_err = 2 * err;

		if(new_err > -dy) {
			err -= dy;
			x += sx;
		}

		if(new_err < dx) {
			err += dx;
			y += sy;
		}
	}
}

uint8_t Canvas::getPixel(int x, int y) const {
	PixelTile px = convertToPixelTile(x, y);

	return Rom_GetTilePixelColor(
			&rom.viewer,
			px.tile_id,
			px.x,
			px.y
			);
}

bool Canvas::isPixelInsideCanvasView(int x, int y) const {
	int w, h;

	w = TileViewer::WIDTH / zoom_level;
	h = TileViewer::HEIGHT / zoom_level;

	x -= offset_tiles_x * TileViewer::TILE_SIZE;
	y -= offset_tiles_y * TileViewer::TILE_SIZE;

	if(x < 0 || y < 0 || x >= w || y >= h) {
		return false;
	}

	return true;
}

void Canvas::copyFromViewerToBuffer(int x, int y, int w, int h, TileBuffer& buffer) const {
	buffer.reset();

	for(int j = 0; j < h; j++) {
		for(int i = 0; i < w; i++) {
			buffer.appendData(
					rom.viewer,
					(i + x) + (j + y) * TileViewer::TILES_PER_ROW
					);
		}
	}

	buffer.width = w;
	buffer.height = h;
}

void Canvas::copyFromBufferToViewer(int x, int y, const TileBuffer& buffer) {
	size_t i = 0, j = 0;

	for(const auto& tile : buffer.raw_data) {
		if(i >= buffer.width) {
			i = 0;
			j++;
		}

		if(i + x >= TileViewer::TILES_PER_ROW) {
			i++;
			continue;
		}

		int tile_id = (i + x) + (j + y) * TileViewer::TILES_PER_ROW;
		undo_system.addTile(rom.viewer, tile_id);

		for(int k = 0; k < 64; k++) {
			Rom_SetTilePixelColor(
					&rom.viewer,
					tile_id,
					k % 8,
					k / 8,
					tile.at(k)
					);
		}

		i++;
	}
}

Canvas::PixelTile Canvas::convertToPixelTile(int x, int y) const {
	int tile_x = (x / TileViewer::TILE_SIZE);
	int tile_y = (y / TileViewer::TILE_SIZE);

	size_t tile_id = tile_x + tile_y * TileViewer::TILES_PER_ROW;

	return PixelTile{
		tile_id,
		x % TileViewer::TILE_SIZE,
		y % TileViewer::TILE_SIZE
	};
}

Canvas::~Canvas(void) {
	if(texture != NULL) {
		SDL_DestroyTexture(texture);
	}
}
