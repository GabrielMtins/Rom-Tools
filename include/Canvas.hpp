#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "RomData.hpp"
#include "TileViewer.hpp"

class Canvas {
	public:
		static std::unique_ptr<Canvas> create(SDL_Renderer *renderer, const std::string& rom_path);
		void draw(SDL_Renderer *renderer, TileViewer& tile_viewer);
		bool isOpen(void) const;

		~Canvas(void);

	private:
		void renderToTexture(SDL_Renderer *renderer, TileViewer& tile_viewer);
		void renderLines(SDL_Renderer *renderer);

		void drawCanvasWindow(void);
		SDL_Rect computeSrcRect(void) const;

		void handleInput(void);
		void increaseZoom(void);
		void decreaseZoom(void);
		int getMaxOffsetXPerZoom(void) const;
		int getMaxOffsetYPerZoom(void) const;

		RomData rom;

		int offset_tiles_x = 0;
		int offset_tiles_y = 0;
		int zoom_level = 1;
		std::string window_name;
		bool open = true;

		SDL_Texture *texture = NULL;

		static size_t unique_identifier;
		static constexpr int WIDTH = 512;
		static constexpr int HEIGHT = 512;
};

#endif
