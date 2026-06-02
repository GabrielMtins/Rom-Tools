#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "RomData.hpp"
#include "TileViewer.hpp"
#include "UndoSystem.hpp"

#include "imgui.h"

#define FOR_TOOL_LIST(DO) \
	DO(TOOL_BRUSH, handleToolBrush) \
	DO(TOOL_SELECT, handleToolSelect) \
	DO(TOOL_BUCKET, handleToolBucket)

class Canvas {
	public:
		#define EXPAND_AS_ENUM(type, function) type,

		enum Tool {
			FOR_TOOL_LIST(EXPAND_AS_ENUM)
		};

		#undef EXPAND_AS_ENUM

		static std::unique_ptr<Canvas> create(SDL_Renderer *renderer, const std::string& rom_path);
		void draw(SDL_Renderer *renderer, TileViewer& tile_viewer);
		void setTool(Tool tool);
		bool isOpen(void) const;

		~Canvas(void);

	private:
		struct PixelTile {
			size_t tile_id;
			int x;
			int y;
		};

		void renderToTexture(SDL_Renderer *renderer, TileViewer& tile_viewer);
		void renderSelectRect(SDL_Renderer *renderer);
		void renderLines(SDL_Renderer *renderer);

		void drawCanvasWindow(void);
		SDL_Rect computeSrcRect(void) const;

		void handleInput(void);
		void handleClickImage(void);

		void handleToolBrush(void);
		void handleToolSelect(void);
		void handleToolBucket(void);

		ImVec2 getNormalPositionOnCanvas(void) const;

		void increaseZoom(void);
		void decreaseZoom(void);
		int getMaxOffsetXPerZoom(void) const;
		int getMaxOffsetYPerZoom(void) const;

		int getTileSizeZoomed(void) const;
		bool isTileInsideSelection(size_t tile_id) const;

		/* x is between 0 and TileViewer::WIDTH 
		 * y is between 0 and TileViewer::HEIGHT */
		bool putPixel(int x, int y, int selected_color, bool check_for_selection = false);
		int getPixel(int x, int y) const;

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

		Tool tool = TOOL_SELECT;

		struct {
			struct {
				SDL_Rect rect = {0, 0, 0, 0};
				int start_x, start_y, end_x, end_y;
				bool selected = false;
			} select;
		} tools;


		static size_t unique_identifier;
		static constexpr int WIDTH = 512;
		static constexpr int HEIGHT = 512;
};

#endif
