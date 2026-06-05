#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "RomData.hpp"
#include "TileViewer.hpp"
#include "UndoSystem.hpp"
#include "TileBuffer.hpp"

#include "imgui.h"

#include "Tool.hpp"

class App;

class Canvas {
	public:
		enum Tool {
			FOR_TOOL_LIST(TOOL_LIST_EXPAND_AS_ENUM)
			COUNT
		};

		static std::unique_ptr<Canvas> create(SDL_Renderer *renderer, const std::string& rom_path);
		void renderFramebuffer(SDL_Renderer *renderer);
		void drawCanvasWindow(const App& app);
		static void setTool(Tool new_tool);
		static Tool getTool(void);

		void selectColorFg(uint8_t color);
		void selectColorBg(uint8_t color);
		void swapColorsBgFg(void);

		uint8_t getColorFg(void) const;
		uint8_t getColorBg(void) const;

		bool isOpen(void) const;
		bool isOnFocus(void) const;

		RomData& getRomData(void);

		~Canvas(void);

		static constexpr int NUM_TOOLS = COUNT;

	private:
		struct PixelTile {
			size_t tile_id;
			int x;
			int y;
		};

		enum ImageInputType {
			IMAGE_INPUT_TOOL,
			IMAGE_INPUT_MOVE,
			IMAGE_INPUT_PASTE
		};

		void renderToTexture(SDL_Renderer *renderer);
		void renderSelectRect(SDL_Renderer *renderer);
		void renderLines(SDL_Renderer *renderer);
		void renderPaste(SDL_Renderer *renderer);
		void renderToolRect(SDL_Renderer *renderer);
		void renderToolLine(void);

		SDL_Rect computeSrcRect(void) const;

		void handleInput(void);
		void resetTools(void);
		void handleClickImage(const App& app);

		void handleToolBrush(void);
		void handleToolSelect(void);
		void handleToolBucket(void);
		void handleToolInvert(void);
		void handleToolRect(void);
		void handleToolLine(void);
		void handleToolPicker(void);

		void handleInputPaste(void);
		void handleInputMove(void);

		ImVec2 getNormalPositionOnCanvas(void) const;
		ImVec2 getIntegerPositionOnViewer(void) const;
		ImVec2 getTilePositionOnViewer(void) const;

		void increaseZoom(void);
		void decreaseZoom(void);
		int getMaxOffsetXPerZoom(void) const;
		int getMaxOffsetYPerZoom(void) const;
		void doOffsetCorrection(void);

		int getTileSizeZoomed(void) const;
		bool isTileInsideSelection(size_t tile_id) const;

		/* Os parâmetros das funções abaixos PRECISAM estar em coordenadas do TileViewer. */
		bool putPixel(int x, int y, uint8_t selected_color, bool check_for_selection = false);
		void bresenhamLine(int x1, int y1, int x2, int y2, uint8_t selected_color, bool check_for_selection=false);
		void floodVisible(uint8_t selected_color, bool check_for_selection=false);
		void floodFill(int start_x, int start_y, uint8_t selected_color, bool check_for_selection=false);
		uint8_t getPixel(int x, int y) const;
		bool isPixelInsideCanvasView(int x, int y) const;
		void copyFromViewerToBuffer(int x, int y, int w, int h, TileBuffer& buffer) const;
		void copyFromBufferToViewer(int x, int y, const TileBuffer& buffer);
		PixelTile convertToPixelTile(int x, int y) const;

		void setDrawColorByMouseButton(void);
		static bool areMouseButtonsUp(void); 

		RomData rom;
		UndoSystem undo_system;

		int offset_tiles_x = 0;
		int offset_tiles_y = 0;
		int zoom_level = 1;
		std::string window_name;
		bool open = true;
		bool is_on_focus = false;
		bool wait_for_mouse_button_release = false;

		float horizontal_slider_width = -1.0f;

		SDL_Texture *texture = NULL;

		struct {
			struct {
				int old_x, old_y;
				bool active = false;
			} brush;

			struct {
				SDL_Rect tile_rect = {0, 0, 0, 0};
				int tile_start_x, tile_start_y, tile_end_x, tile_end_y;
				bool selected = false;
			} select;

			struct {
				int x, y;
			} paste;

			struct {
				SDL_Rect px_rect;
				int start_x, start_y;
				int end_x, end_y;
				bool active = false;
			} rect;

			struct {
				int start_x, start_y;
				int end_x, end_y;
				bool active = false;
			} line;

			struct {
				int offset_tiles_x, offset_tiles_y;
				ImVec2 pos_start;
				bool active = false;
			} move;
		} tools;

		TileBuffer tile_tmp_buffer;

		ImGuiWindowFlags window_flags = 0;

		ImageInputType image_input_type = IMAGE_INPUT_TOOL;

		static size_t unique_identifier;
		static TileBuffer tile_copy_buffer;
		static std::unique_ptr<TileViewer> viewer_copy;
		static std::unique_ptr<TileViewer> tile_viewer;

		uint8_t selected_color_fg = 0;
		uint8_t selected_color_bg = 1;
		uint8_t draw_color = 0;

		static Tool tool;
		static Tool old_tool;

		static constexpr int WIDTH = 512;
		static constexpr int HEIGHT = 512;

		static constexpr int canvas_view_ratio = WIDTH / TileViewer::WIDTH;
};

#endif
