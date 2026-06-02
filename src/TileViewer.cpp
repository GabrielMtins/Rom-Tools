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

void TileViewer::drawLine(int x1, int y1, int x2, int y2, uint32_t color) {
	uint32_t *pixels;
	int pitch;

	SDL_LockTexture(texture, NULL, (void **) &pixels, &pitch);

	pitch /= 4;

	int dx = std::abs(x2 - x1);
	int dy = std::abs(y2 - y1);

	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;

	int err = dx - dy;
	int x = x1, y = y1;

	while(1) {
		pixels[x + y * pitch] = color;

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
