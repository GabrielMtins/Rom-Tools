#include "TextureHandler.hpp"
#include "TileViewer.hpp"

std::unique_ptr<TextureHandler> TextureHandler::generate(SDL_Renderer *renderer, const TileBuffer& buffer) {
	auto handler = std::make_unique<TextureHandler>();

	/*
	SDL_Surface *surface = SDL_CreateRGBSurface(
			0,
			buffer.width * TileViewer::WIDTH
			);
			*/
}

SDL_Texture * TextureHandler::getTexture(void) {
	return texture;
}

TextureHandler::~TextureHandler(void) {
	if(texture != NULL) {
		SDL_DestroyTexture(texture);
	}
}
