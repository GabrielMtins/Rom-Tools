#include "TileViewer.hpp"
#include <iostream>

std::unique_ptr<TileViewer> TileViewer::create(SDL_Renderer *renderer) {
	auto texture_viewer = std::make_unique<TileViewer>();

	texture_viewer->texture = SDL_CreateTexture(
			renderer,
			FORMAT,
			SDL_TEXTUREACCESS_STREAMING,
			WIDTH,
			HEIGHT
			);

	if(texture_viewer->texture == NULL) {
		return nullptr;
	}

	return texture_viewer;
}

void TileViewer::draw(const Rom_Viewer& viewer, const Palette& palette, int offset_tiles_y) {
	uint32_t *pixels;
	int pitch;

	SDL_LockTexture(texture, NULL, (void **) &pixels, &pitch);

	pitch /= 4;

	for(int j = 0; j < HEIGHT; j++) {
		for(int i = 0; i < WIDTH; i++) {
			int tile_id = (i / TILE_SIZE) + (j / TILE_SIZE + offset_tiles_y) * TILES_PER_ROW;

			int color_id = Rom_GetTilePixelColor(
					&viewer,
					tile_id,
					i % TILE_SIZE,
					j % TILE_SIZE
					);

			if(color_id >= PALETTE_SIZE || color_id < 0) {
				continue;
			} 

			pixels[i + j * pitch] = palette.at(color_id);
		}
	}

	SDL_UnlockTexture(texture);
}

void TileViewer::drawBuffer(const TileBuffer& buffer, const Palette& palette) {
	uint32_t *pixels;
	int pitch;

	SDL_LockTexture(texture, NULL, (void **) &pixels, &pitch);

	pitch /= 4;

	for(size_t j = 0; j < buffer.height * TILE_SIZE; j++) {
		for(size_t i = 0; i < buffer.width * TILE_SIZE; i++) {
			int color_id = buffer.raw_data.at(
					(i / TILE_SIZE) + (j / TILE_SIZE) * buffer.width
					).at(
						(i % TILE_SIZE) + (j % TILE_SIZE) * TILE_SIZE
						);

			pixels[(i) + (j) * pitch] = palette.at(color_id);
		}
	}

	SDL_UnlockTexture(texture);
}

SDL_Texture * TileViewer::getTexture(void) {
	return texture;
}

TileViewer::~TileViewer(void) {
	if(texture != NULL) {
		SDL_DestroyTexture(texture);
	}
}
