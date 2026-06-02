#ifndef TEXTURE_HANDLER_HPP
#define TEXTURE_HANDLER_HPP

#include "TileBuffer.hpp"

#include <memory>
#include <SDL2/SDL.h>

class TextureHandler {
	public:
		static std::unique_ptr<TextureHandler> generate(SDL_Renderer *renderer);

		SDL_Texture * getTexture(void);
		~TextureHandler(void);

	private:
		SDL_Texture *texture = NULL;
};

#endif
