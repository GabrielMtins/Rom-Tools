#ifndef TOOLBAR_HPP
#define TOOLBAR_HPP

#include "Canvas.hpp"

class Toolbar {
	public:
		void render(Canvas *active_canvas);

	private:
		void renderIcons(void);
		void renderPaletteColors(Canvas *active_canvas);
		bool renderCurrentColors(uint32_t fg_color, uint32_t bg_color);
};

#endif
