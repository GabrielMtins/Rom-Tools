#ifndef CANVAS_LIST_HPP
#define CANVAS_LIST_HPP

#include "Canvas.hpp"

#include <unordered_map>

class CanvasList {
	public:
		bool loadCanvas(SDL_Renderer *renderer, const std::string& filename);
		void render(App& app, SDL_Renderer *renderer);

		Canvas * getLastActiveCanvas(void);
		bool isAnyCanvasOnFocus(void) const;

	private:
		void handleInput(void);
		void cleanUp(void);

		std::unordered_map<size_t, std::unique_ptr<Canvas>> list;
		std::vector<size_t> ids_to_remove;

		size_t next_id = 0;
		size_t last_active_id = 0xffffffff;
		bool is_any_canvas_on_focus = false;
};

#endif
