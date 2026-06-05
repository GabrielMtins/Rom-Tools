#include "CanvasList.hpp"
#include "App.hpp"

bool CanvasList::loadCanvas(SDL_Renderer *renderer, const std::string& filename) {
	auto new_canvas = Canvas::create(renderer, filename);

	if(new_canvas == nullptr) {
		return false;
	}
	
	new_canvas->renderFramebuffer(renderer);

	list.insert({next_id++, std::move(new_canvas)});

	return true;
}

void CanvasList::render(App& app, SDL_Renderer *renderer) {
	bool next_is_any_canvas_on_focus = false;

	handleInput();

	for(auto& [id, canvas] : list) {
		if(id == last_active_id) {
			canvas->renderFramebuffer(renderer);
		}

		canvas->drawCanvasWindow(app);

		if(!canvas->isOpen()) {
			ids_to_remove.push_back(id);
		}

		if(canvas->isOnFocus()) {
			last_active_id = id;
			next_is_any_canvas_on_focus = true;
		}
	}

	cleanUp();

	is_any_canvas_on_focus = next_is_any_canvas_on_focus;
}

Canvas * CanvasList::getLastActiveCanvas(void) {
	if(list.find(last_active_id) == list.end()) {
		return nullptr;
	}

	return list[last_active_id].get();
}

bool CanvasList::isAnyCanvasOnFocus(void) const {
	return is_any_canvas_on_focus;
}

void CanvasList::handleInput(void) {
	Canvas *active_canvas = getLastActiveCanvas();

	if(!active_canvas) return;

	if(ImGui::IsKeyPressed(ImGuiKey_X)) {
		active_canvas->swapColorsBgFg();
	}
}

void CanvasList::cleanUp(void) {
	while(!ids_to_remove.empty()) {
		list.erase(ids_to_remove.back());

		ids_to_remove.pop_back();
	}
}
