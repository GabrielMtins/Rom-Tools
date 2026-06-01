#ifndef TILE_VIEWER_HPP
#define TILE_VIEWER_HPP

#include <SDL2/SDL.h>
#include <rom_tools.h>
#include <memory>

#include <array>

#include "Palette.hpp"
#include "RomData.hpp"

class TileViewer {
	public:
		static std::unique_ptr<TileViewer> create(SDL_Renderer *renderer);
		void draw(const Rom_Viewer& viewer, const Palette& palette, int offset_tiles_y);
		SDL_Texture * getTexture(void);

		~TileViewer(void);

		static constexpr int TILES_PER_ROW = 16;
		static constexpr int TILES_PER_COLUMN = 16;
		static constexpr int TILE_SIZE = 8;
		static constexpr int WIDTH = TILES_PER_ROW * TILE_SIZE;
		static constexpr int HEIGHT = TILES_PER_COLUMN * TILE_SIZE;
		static constexpr SDL_PixelFormatEnum FORMAT = SDL_PIXELFORMAT_ARGB8888;

	private:
		SDL_Texture *texture = NULL;
};

#endif
