#ifndef PALETTE_MENU_HPP
#define PALETTE_MENU_HPP

#include "Palette.hpp"
#include "imgui.h"
#include "RomData.hpp"
#include "Canvas.hpp"

class PaletteMenu {
	public:
		static std::unique_ptr<PaletteMenu> create(SDL_Renderer *renderer);

		void drawPaleteTexture(Canvas& canvas, SDL_Renderer *renderer);
		void render(Canvas& canvas);
		~PaletteMenu(void);

	private:
		void drawPaletteRects(Canvas& canvas);
		uint32_t drawMainPalette(void);
		void drawColorPicker(Canvas& canvas);

		static ImVec4 getVec4Color(uint32_t color);
		static ImVec4 getDimmedColor(const ImVec4& color);
		static ImVec4 getBrightColor(const ImVec4& color);

		float fg_color[3] = {0.0f, 0.0f, 0.0f};
		SDL_Texture *palette_texture = NULL;

		static constexpr int TEXTURE_WIDTH = 160;
		static constexpr int TEXTURE_HEIGHT = 160;
		static constexpr int TILE_WIDTH = TEXTURE_WIDTH / 4;
		static constexpr int TILE_HEIGHT = TEXTURE_HEIGHT / 4;
};

#endif
