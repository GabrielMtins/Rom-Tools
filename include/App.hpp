#ifndef APP_HPP
#define APP_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <memory>

#include "TileViewer.hpp"
#include "RomData.hpp"
#include "Canvas.hpp"

class App {
	public:
		App(void);
		void run(void);
		~App(void);

	private:
		void loop(void);
		void renderMenubar(void);
		void renderToolbar(void);
		void renderCanvasList(void);
		void beginRender(void);
		void endRender(void);

		SDL_Window *window = NULL;
		SDL_Renderer *renderer = NULL;
		bool quit = false;

		std::unique_ptr<TileViewer> tile_viewer;
		std::vector<std::unique_ptr<Canvas>> canvas_list;
};

#endif
