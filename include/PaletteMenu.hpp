#ifndef PALETTE_MENU_HPP
#define PALETTE_MENU_HPP

#include "Palette.hpp"
#include "imgui.h"
#include "RomData.hpp"
#include "Canvas.hpp"

class PaletteMenu {
	public:
		void render(Canvas& canvas);

	private:
		void drawPaletteRects(Canvas& canvas, const Palette& palette, size_t num_colors);
		uint32_t drawMainPalette(void);

		static ImVec4 getVec4Color(uint32_t color);
		static ImVec4 getDimmedColor(const ImVec4& color);
		static ImVec4 getBrightColor(const ImVec4& color);

		bool open = true;
};

#endif
