#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "RomData.hpp"
#include "TileViewer.hpp"
#include "UndoSystem.hpp"
#include "TileBuffer.hpp"

#include "imgui.h"

#include "Tool.hpp"

class Canvas {
	public:
		enum Tool {
			FOR_TOOL_LIST(TOOL_LIST_EXPAND_AS_ENUM)
			COUNT
		};

		static std::unique_ptr<Canvas> create(SDL_Renderer *renderer, const std::string& rom_path);
		void draw(SDL_Renderer *renderer, TileViewer& tile_viewer);
		static void setTool(Tool new_tool);
		static Tool getTool(void);

		bool isOpen(void) const;

		~Canvas(void);

		static constexpr int NUM_TOOLS = COUNT;

	private:
		struct PixelTile {
			size_t tile_id;
			int x;
			int y;
		};

		void renderToTexture(SDL_Renderer *renderer, TileViewer& tile_viewer);
		void renderSelectRect(SDL_Renderer *renderer);
		void renderLines(SDL_Renderer *renderer);
		void renderPaste(SDL_Renderer *renderer);
		void renderToolRect(SDL_Renderer *renderer);
		void renderToolLine(TileViewer &tile_viewer);

		void drawCanvasWindow(void);
		SDL_Rect computeSrcRect(void) const;

		void handleInput(void);
		void handleClickImage(void);

		void handleToolBrush(void);
		void handleToolSelect(void);
		void handleToolBucket(void);
		void handleToolInvert(void);
		void handleToolPaste(void);
		void handleToolRect(void);
		void handleToolLine(void);
		void handleToolMove(void);
		void resetTools(void);

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
		bool putPixel(int x, int y, int selected_color, bool check_for_selection = false);
		void bresenhamLine(int x1, int y1, int x2, int y2, int selected_color, bool check_for_selection=false);
		void floodVisible(int selected_color, bool check_for_selection=false);
		void floodFill(int start_x, int start_y, int selected_color, bool check_for_selection=false);
		int getPixel(int x, int y) const;
		bool isPixelInsideCanvasView(int x, int y) const;
		void copyFromViewerToBuffer(int x, int y, int w, int h, TileBuffer& buffer) const;
		void copyFromBufferToViewer(int x, int y, const TileBuffer& buffer);
		PixelTile convertToPixelTile(int x, int y) const;

		RomData rom;
		UndoSystem undo_system;

		int offset_tiles_x = 0;
		int offset_tiles_y = 0;
		int zoom_level = 1;
		std::string window_name;
		bool open = true;
		bool focused = false;

		float horizontal_slider_width = -1.0f;

		int selected_color = 0;

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
				bool selected = false;
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

		static size_t unique_identifier;
		static TileBuffer tile_copy_buffer;
		static std::unique_ptr<TileViewer> viewer_copy;

		static Tool tool;
		static Tool old_tool;

		static constexpr int WIDTH = 512;
		static constexpr int HEIGHT = 512;

		static constexpr int canvas_view_ratio = WIDTH / TileViewer::WIDTH;
};

#endif
