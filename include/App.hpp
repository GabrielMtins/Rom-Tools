#ifndef APP_HPP
#define APP_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <memory>

#include "TileViewer.hpp"
#include "RomData.hpp"
#include "Canvas.hpp"

#include "TileBuffer.hpp"
#include "Toolbar.hpp"
#include "PaletteMenu.hpp"
#include "CanvasList.hpp"

class App {
	public:
		App(void);
		void run(void);
		~App(void);
		
		bool isAnyCanvasOnFocus(void) const;

	private:
		void loop(void);
		void renderMenubar(void);
		void handleInput(void);
		void beginRender(void);
		void endRender(void);

		SDL_Window *window = NULL;
		SDL_Renderer *renderer = NULL;
		bool quit = false;

		CanvasList canvas_list;
		Toolbar toolbar;

		std::unique_ptr<PaletteMenu> palette_menu;

		static constexpr uint32_t MIN_TIME_FRAME = 16;
};

#endif
