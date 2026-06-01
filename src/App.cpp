#include "App.hpp"

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

App::App(void) {
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
	//SetupImGuiPaperAndInkStyle();
	//SetupImGuiDarkStyle();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

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

void App::loop(void) {
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch(event.type) {
			case SDL_QUIT:
				quit = true;
				break;
		}
	}

	beginRender();

	ImGui::DockSpaceOverViewport();

	renderMenubar();
	renderToolbar();
	renderCanvasList();

	endRender();
}

void App::renderMenubar(void) {
	if(ImGui::BeginMainMenuBar()) {
		if(ImGui::BeginMenu("File")) {
			ImGui::MenuItem("New");
			ImGui::MenuItem("Open");
			ImGui::MenuItem("Save");
			ImGui::MenuItem("Save as");
			ImGui::MenuItem("Exit");
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

void App::renderToolbar(void) {
	ImGui::Begin("Toolbar");

	ImGui::Button("hi0");
	ImGui::Button("hi1");
	ImGui::Button("hi2");
	ImGui::Button("hi3");

	ImGui::End();
}

void App::renderCanvasList(void) {
	for(auto& canvas : canvas_list) {
		canvas->draw(renderer, *tile_viewer);
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
