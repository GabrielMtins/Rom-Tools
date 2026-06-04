#include "App.hpp"

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "Style.hpp"
#include "Localization.hpp"

App::App(void) {
	i18n::setLanguage(i18n::LANG_EN_US);

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << SDL_GetError() << '\n';
		exit(-1);
	}

	float main_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);

	window = SDL_CreateWindow(
			"neschgui",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			640,
			480,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_RESIZABLE
			);

	if(window == NULL) {
		std::cerr << SDL_GetError() << '\n';
		exit(-1);
	}

	renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
			);

	if(renderer == NULL) {
		std::cerr << SDL_GetError() << '\n';
		exit(-1);
	}

	SDL_MaximizeWindow(window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	Style::setUpFonts();
	//Style::setUpDarkTheme();
	//Style::setUpPaperAndInkStyle();
	Style::setUpCatppuccinStyle();

	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	tile_viewer = TileViewer::create(renderer);
	canvas_list.push_back(Canvas::create(renderer, "tools/roms/smb_og.nes"));
	canvas_list.push_back(Canvas::create(renderer, "tools/roms/dk.nes"));

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void App::run(void) {
	while(!quit) {
		loop();
	}
}

App::~App(void) {
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

bool App::isAnyCanvasOnFocus(void) const {
	return is_any_canvas_on_focus;
}

void App::loop(void) {
	SDL_Event event;
	uint32_t next_tick, last_tick, dt;
	last_tick = SDL_GetTicks();

	while(SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch(event.type) {
			case SDL_QUIT:
				quit = true;
				break;
		}
	}

	handleInput();

	beginRender();

	ImGui::DockSpaceOverViewport();

	renderMenubar();
	renderCanvasList();

	toolbar.render();

	endRender();

	next_tick = SDL_GetTicks();

	dt = next_tick - last_tick;

	if(dt < MIN_TIME_FRAME) {
		SDL_Delay(MIN_TIME_FRAME - dt);
	}
}

void App::renderMenubar(void) {
	if(ImGui::BeginMainMenuBar()) {
		if(ImGui::BeginMenu(i18n::File)) {
			ImGui::MenuItem(i18n::New);
			ImGui::MenuItem(i18n::Open);
			ImGui::MenuItem(i18n::Save);
			ImGui::MenuItem(i18n::SaveAs);
			ImGui::MenuItem(i18n::Quit);
			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Edit")) {
			ImGui::MenuItem("Undo");
			ImGui::MenuItem("Cut");
			ImGui::MenuItem("Copy");

			ImGui::Separator();

			ImGui::MenuItem("Preferences");

			ImGui::EndMenu();
		}
	
		ImGui::EndMainMenuBar();

	}
}

void App::renderCanvasList(void) {
	bool next_is_any_canvas_on_focus = false;

	for(auto& canvas : canvas_list) {
		canvas->renderFramebuffer(renderer, *tile_viewer);
		canvas->drawCanvasWindow(*this);

		next_is_any_canvas_on_focus |= canvas->isOnFocus();
	}

	canvas_list.erase(
			std::remove_if(
				canvas_list.begin(),
				canvas_list.end(),
				[](const std::unique_ptr<Canvas>& canvas) {
					return !canvas->isOpen();
				}
				),
			canvas_list.end()
			);

	is_any_canvas_on_focus = next_is_any_canvas_on_focus;
}

void App::handleInput(void) {
	FOR_TOOL_LIST(TOOL_LIST_EXPAND_AS_INPUT_CANVAS);
}

void App::beginRender(void) {
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void App::endRender(void) {
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::Render();
	SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
	SDL_RenderPresent(renderer);
}
