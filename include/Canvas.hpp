#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "RomData.hpp"
#include "TileViewer.hpp"
#include "UndoSystem.hpp"

class Canvas {
	public:
		enum Tool {
			TOOL_BRUSH
		};

		static std::unique_ptr<Canvas> create(SDL_Renderer *renderer, const std::string& rom_path);
		void draw(SDL_Renderer *renderer, TileViewer& tile_viewer);
		bool isOpen(void) const;

		~Canvas(void);

	private:
		struct PixelTile {
			size_t tile_id;
			int x;
			int y;
		};

		void renderToTexture(SDL_Renderer *renderer, TileViewer& tile_viewer);
		void renderLines(SDL_Renderer *renderer);

		void drawCanvasWindow(void);
		SDL_Rect computeSrcRect(void) const;

		void handleInput(void);
		void handleClickImage(void);
		void increaseZoom(void);
		void decreaseZoom(void);
		int getMaxOffsetXPerZoom(void) const;
		int getMaxOffsetYPerZoom(void) const;

		/* x is between 0 and TileViewer::WIDTH 
		 * y is between 0 and TileViewer::HEIGHT */
		void putPixel(int x, int y, int selected_color);
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

		static size_t unique_identifier;
		static constexpr int WIDTH = 512;
		static constexpr int HEIGHT = 512;
};

#endif
