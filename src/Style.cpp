#include "Style.hpp"

#include "imgui.h"
#include "inter.h"
#include "font_awesome.h"

namespace Style {
	static void setUpBasicStyle(ImGuiStyle& style) {
		// --- 1. Sizing & Spacing (Clean & Rigid) ---
		style.WindowPadding = ImVec2(12.0f, 12.0f);
		style.FramePadding = ImVec2(6.0f, 4.0f);
		style.CellPadding = ImVec2(6.0f, 4.0f);
		style.ItemSpacing = ImVec2(8.0f, 6.0f);
		style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
		style.ScrollbarSize = 14.0f;
		style.GrabMinSize = 12.0f;
	
		// --- 2. Borders & Rounding (Technical/Drafting feel) ---
		style.WindowRounding = 2.0f;
		style.ChildRounding = 2.0f;
		style.FrameRounding = 2.0f;
		style.PopupRounding = 2.0f;
		style.ScrollbarRounding = 12.0f;
		style.GrabRounding = 2.0f;
		style.TabRounding = 2.0f;
	
		style.WindowBorderSize = 1.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupBorderSize = 1.0f;
		style.FrameBorderSize = 1.0f;
		style.TabBorderSize = 1.0f;

	}

	void setUpFonts(void) {
		ImFontConfig font_cfg;
		font_cfg.FontDataOwnedByAtlas = false;
		ImGuiIO& io = ImGui::GetIO();

		static constexpr float font_size = 14.0f;

		ImFont *font = io.Fonts->AddFontFromMemoryTTF(
				(void *) inter_ttf,
				inter_ttf_len,
				font_size,
				&font_cfg
				);

		if(!font) {
			io.Fonts->AddFontDefault();
		}

		font_cfg.MergeMode = true;
		font_cfg.GlyphMinAdvanceX = font_size;

		static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
		io.Fonts->AddFontFromMemoryTTF(
				(void *) font_awesome_otf,
				font_awesome_otf_len,
				14.0f,
				&font_cfg,
				icons_ranges
				);
	}

	void setUpPaperAndInkStyle() {
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;
		
		setUpBasicStyle(style);
	
		// --- 3. Full Color Palette ---
	
		// Main Text & Background
		colors[ImGuiCol_Text] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f); // Deep Carbon Ink
		colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.96f, 0.96f, 0.94f, 1.00f); // Warm Paper
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.03f);
		colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f); // Clean White Popups
	
		// Borders & Separators
		colors[ImGuiCol_Border] = ImVec4(0.75f, 0.75f, 0.72f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.80f, 0.80f, 0.78f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.17f, 0.34f, 0.59f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.17f, 0.34f, 0.59f, 1.00f);
	
		// Frames (Inputs, Checkboxes, etc)
		colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.92f, 0.95f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.85f, 0.88f, 0.92f, 1.00f);
	
		// Titles & Menus
		colors[ImGuiCol_TitleBg] = ImVec4(0.92f, 0.92f, 0.90f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.88f, 0.88f, 0.86f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.92f, 0.92f, 0.90f, 0.75f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.92f, 0.92f, 0.90f, 1.00f);
	
		// Scrollbars
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.96f, 0.96f, 0.94f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.78f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.70f, 0.70f, 0.68f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.60f, 0.58f, 1.00f);
	
		// Interactables (Blueprint Blue)
		colors[ImGuiCol_CheckMark] = ImVec4(0.17f, 0.34f, 0.59f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.17f, 0.34f, 0.59f, 0.70f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.17f, 0.34f, 0.59f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.17f, 0.34f, 0.59f, 0.08f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.17f, 0.34f, 0.59f, 0.20f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.17f, 0.34f, 0.59f, 0.35f);
	
		// Header (Selection in lists/trees)
		colors[ImGuiCol_Header] = ImVec4(0.17f, 0.34f, 0.59f, 0.12f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.17f, 0.34f, 0.59f, 0.25f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.17f, 0.34f, 0.59f, 0.40f);
	
		// Tables (Crucial for Light Mode)
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.90f, 0.90f, 0.88f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.75f, 0.75f, 0.72f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.85f, 0.85f, 0.82f, 1.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.00f, 0.00f, 0.00f, 0.03f);
	
		// Tabs
		colors[ImGuiCol_Tab] = ImVec4(0.92f, 0.92f, 0.90f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.92f, 0.92f, 0.90f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.96f, 0.96f, 0.94f, 1.00f);
	
		// Misc
		colors[ImGuiCol_PlotLines] = ImVec4(0.17f, 0.34f, 0.59f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.17f, 0.34f, 0.59f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.17f, 0.34f, 0.59f, 0.25f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.17f, 0.34f, 0.59f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.17f, 0.34f, 0.59f, 1.00f);
	
	#ifdef IMGUI_HAS_DOCK
		colors[ImGuiCol_DockingPreview] = ImVec4(0.17f, 0.34f, 0.59f, 0.40f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.96f, 0.96f, 0.94f, 1.00f);
	#endif
	}

	void setUpDarkTheme(void) {
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::StyleColorsDark();
		ImVec4* colors = style.Colors;

		setUpBasicStyle(style);
	
		colors[ImGuiCol_Button] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	}

	void setUpDraculaTheme() {
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;
	
		setUpBasicStyle(style);
	
		// --- 3. The Dracula Color Palette ---
		// Background: #282a36 | Selection: #44475a | Foreground: #f8f8f2
		// Comment: #6272a4	| Cyan: #8be9fd	  | Green: #50fa7b
		// Orange: #ffb86c	 | Pink: #ff79c6	  | Purple: #bd93f9
		// Red: #ff5555		| Yellow: #f1fa8c
	
		// Text
		colors[ImGuiCol_Text] = ImVec4(0.97f, 0.97f, 0.95f, 1.00f); // #f8f8f2
		colors[ImGuiCol_TextDisabled] = ImVec4(0.38f, 0.45f, 0.64f, 1.00f); // #6272a4
	
		// Backgrounds
		colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.16f, 0.21f, 1.00f); // #282a36
		colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.21f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.16f, 0.16f, 0.21f, 0.96f);
	
		// Borders
		colors[ImGuiCol_Border] = ImVec4(0.27f, 0.28f, 0.35f, 1.00f); // #44475a
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	
		// Frames (Inputs, etc.)
		colors[ImGuiCol_FrameBg] = ImVec4(0.27f, 0.28f, 0.35f, 1.00f); // #44475a
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.45f, 0.64f, 1.00f); // #6272a4
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.48f, 0.55f, 0.74f, 1.00f);
	
		// Title Bars
		colors[ImGuiCol_TitleBg] = ImVec4(0.13f, 0.14f, 0.18f, 1.00f); // Darker
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);
	
		// Menus
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);
	
		// Scrollbars
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.38f, 0.45f, 0.64f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.48f, 0.55f, 0.74f, 1.00f);
	
		// Interactables
		colors[ImGuiCol_CheckMark] = ImVec4(0.31f, 0.98f, 0.48f, 1.00f); // #50fa7b (Green)
		colors[ImGuiCol_SliderGrab] = ImVec4(0.74f, 0.58f, 0.98f, 1.00f); // #bd93f9 (Purple)
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.84f, 0.68f, 1.00f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.47f, 0.78f, 1.00f); // #ff79c6 (Pink)
		colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.37f, 0.62f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.45f, 0.64f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.55f, 0.74f, 1.00f);
	
		// Tabs
		colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
	
		// Tables
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.38f, 0.45f, 0.64f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
	
		// Misc
		colors[ImGuiCol_PlotLines] = ImVec4(0.55f, 0.91f, 0.99f, 1.00f); // #8be9fd (Cyan)
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.74f, 0.58f, 0.98f, 1.00f);
	
	#ifdef IMGUI_HAS_DOCK
		colors[ImGuiCol_DockingPreview] = ImVec4(0.74f, 0.58f, 0.98f, 0.50f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
	#endif
	}

	void setUpCrimsonTheme(void) {
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		setUpBasicStyle(style);

		// Text
		colors[ImGuiCol_Text] = ImVec4(1.00f, 0.90f, 0.90f, 1.00f); // Slight pinkish tint to off-white
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.40f, 0.40f, 1.00f);
	
		// Backgrounds
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.07f, 0.07f, 1.00f); // Deep charcoal
		colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.09f, 0.09f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.07f, 0.07f, 0.96f);
	
		// Borders
		colors[ImGuiCol_Border] = ImVec4(0.25f, 0.15f, 0.15f, 0.80f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	
		// Frames
		colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.20f, 0.20f, 1.00f);
	
		// Title Bars
		colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.08f, 0.08f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.25f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
	
		// Menus
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.08f, 0.08f, 1.00f);
	
		// Scrollbars
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.12f, 0.12f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.45f, 0.20f, 0.20f, 1.00f);
	
		// Interactables (The High-Intensity Red)
		colors[ImGuiCol_CheckMark] = ImVec4(0.85f, 0.15f, 0.15f, 1.00f); // Sharp Red
		colors[ImGuiCol_SliderGrab] = ImVec4(0.60f, 0.12f, 0.12f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.85f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.30f, 0.12f, 0.12f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.18f, 0.18f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.30f, 0.12f, 0.12f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.18f, 0.18f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.70f, 0.25f, 0.25f, 1.00f);
	
		// Tabs
		colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.50f, 0.18f, 0.18f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.35f, 0.12f, 0.12f, 1.00f);
	
		// Misc
		colors[ImGuiCol_PlotLines] = ImVec4(0.85f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.85f, 0.15f, 0.15f, 0.35f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.85f, 0.15f, 0.15f, 1.00f);
	
	#ifdef IMGUI_HAS_DOCK
		colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.15f, 0.15f, 0.40f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.08f, 0.07f, 0.07f, 1.00f);
	#endif
	}


	void setUpCyberpunkTheme(void) {
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		setUpBasicStyle(style);
	
		// --- 3. The Neon Palette ---
		// Background: Pitch Black / Deep Navy
		// Neon Cyan: #00ff9f | Neon Pink: #ff003f | Neon Yellow: #fcee0a
	
		// Text
		colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 0.62f, 1.00f); // Neon Green/Cyan
		colors[ImGuiCol_TextDisabled] = ImVec4(0.20f, 0.40f, 0.35f, 1.00f);
	
		// Backgrounds
		colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.02f, 0.04f, 1.00f); // Near black
		colors[ImGuiCol_ChildBg] = ImVec4(0.02f, 0.02f, 0.04f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.02f, 0.02f, 0.04f, 0.98f);
	
		// Borders (The "Glow" look)
		colors[ImGuiCol_Border] = ImVec4(1.00f, 0.00f, 0.25f, 0.60f); // Neon Pink Border
		colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 0.00f, 0.25f, 0.20f);
	
		// Frames
		colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.10f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.00f, 0.25f, 0.20f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.00f, 0.25f, 0.40f);
	
		// Title Bars
		colors[ImGuiCol_TitleBg] = ImVec4(0.02f, 0.02f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.05f, 0.05f, 0.10f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.02f, 0.02f, 0.04f, 1.00f);
	
		// Menus
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.05f, 0.05f, 0.10f, 1.00f);
	
		// Scrollbars
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.04f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.00f, 0.93f, 0.04f, 0.60f); // Neon Yellow
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 0.93f, 0.04f, 0.80f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 0.93f, 0.04f, 1.00f);
	
		// Interactables
		colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.93f, 0.04f, 1.00f); // Yellow
		colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.00f, 0.25f, 0.80f); // Pink
		colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.00f, 0.25f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.00f, 1.00f, 0.62f, 0.20f); // Cyan Ghost
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 1.00f, 0.62f, 0.50f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 0.62f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(1.00f, 0.00f, 0.25f, 0.30f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.00f, 0.25f, 0.50f);
		colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.00f, 0.25f, 1.00f);
	
		// Tabs
		colors[ImGuiCol_Tab] = ImVec4(0.05f, 0.05f, 0.10f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(1.00f, 0.00f, 0.25f, 0.80f);
		colors[ImGuiCol_TabActive] = ImVec4(0.80f, 0.00f, 0.20f, 1.00f);
	
		// Misc
		colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.93f, 0.04f, 0.30f);
		colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.25f, 1.00f);
	
	#ifdef IMGUI_HAS_DOCK
		colors[ImGuiCol_DockingPreview] = ImVec4(0.00f, 1.00f, 0.62f, 0.40f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.02f, 0.02f, 0.04f, 1.00f);
	#endif
	}

	void setUpCatppuccinStyle() {
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		setUpBasicStyle(style);
	
		/*
		// --- 1. Sizing and Spacing (Soft & Modern) ---
		style.WindowPadding = ImVec2(12.0f, 12.0f);
		style.FramePadding = ImVec2(6.0f, 4.0f);
		style.ItemSpacing = ImVec2(8.0f, 6.0f);
		style.ScrollbarSize = 14.0f;
		style.GrabMinSize = 12.0f;
	
		// --- 2. Borders & Rounding ---
		//style.WindowRounding = 8.0f;
		style.WindowRounding = 0.0f;
		style.FrameRounding = 0.0f;
		style.PopupRounding = 0.0f;
		style.ScrollbarRounding = 0.0f;
		style.GrabRounding = 0.0f;
		style.TabRounding = 0.0f;
	
		style.WindowBorderSize = 1.0f;
		style.FrameBorderSize = 0.0f; // Minimalist look
		style.PopupBorderSize = 1.0f;
		*/
	
		// --- 3. The Catppuccin Mocha Palette ---
		// Base: #1e1e2e | Mantle: #181825 | Crust: #11111b
		// Text: #cdd6f4 | Subtext0: #a6adc8 | Surface0: #313244
		// Lavender: #b4befe | Sapphire: #74c7ec | Mauve: #cba6f7
	
		// Text
		colors[ImGuiCol_Text] = ImVec4(0.80f, 0.84f, 0.96f, 1.00f); // Text
		colors[ImGuiCol_TextDisabled] = ImVec4(0.42f, 0.45f, 0.55f, 1.00f); // Surface1
	
		// Backgrounds
		colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f); // Base
		colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f); // Mantle
		colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.11f, 0.96f); // Crust
	
		// Borders
		colors[ImGuiCol_Border] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f); // Surface0
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	
		// Frames (Inputs, etc.)
		colors[ImGuiCol_FrameBg] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f); // Surface0
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.26f, 0.35f, 1.00f); // Surface1
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f); // Surface2
	
		// Title Bars
		colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f); // Mantle
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f); // Base
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07f, 0.07f, 0.11f, 1.00f); // Crust
	
		// Menus
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
	
		// Scrollbars
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f); // Surface2
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.37f, 0.38f, 0.51f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.42f, 0.45f, 0.55f, 1.00f);
	
		// Interactables
		colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.75f, 1.00f, 1.00f); // Lavender
		colors[ImGuiCol_SliderGrab] = ImVec4(0.45f, 0.78f, 0.93f, 1.00f); // Sapphire
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.45f, 0.78f, 0.93f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.65f, 0.97f, 1.00f); // Mauve
		colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.55f, 0.87f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.26f, 0.35f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f);
	
		// Tabs
		colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
	
		// Misc
		colors[ImGuiCol_PlotLines] = ImVec4(0.94f, 0.72f, 0.42f, 1.00f); // Marigold
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.31f, 0.32f, 0.42f, 1.00f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.71f, 0.75f, 1.00f, 1.00f); // Lavender
	
	#ifdef IMGUI_HAS_DOCK
		colors[ImGuiCol_DockingPreview] = ImVec4(0.71f, 0.75f, 1.00f, 0.50f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
	#endif
	}
};

