#ifndef TOOL_HPP
#define TOOL_HPP

#define FOR_TOOL_LIST(DO) \
	DO(TOOL_SELECT,   handleToolSelect,   ImGuiKey_V,       "\uf245") \
	DO(TOOL_BRUSH,    handleToolBrush,    ImGuiKey_B,       "\uf304") \
	DO(TOOL_BUCKET,   handleToolBucket,   ImGuiKey_F,       "\uf576") \
	DO(TOOL_INVERT,   handleToolInvert,   ImGuiKey_T,       "\uf07e") \
	DO(TOOL_PASTE,    handleToolPaste,    ImGuiKey_P,       "\uf0ea") \
	DO(TOOL_RECT,     handleToolRect,     ImGuiKey_R,       "\uf0c8") \
	DO(TOOL_LINE,     handleToolLine,     ImGuiKey_Q,       "\uf5ad") \
	DO(TOOL_MOVE,     handleToolMove,     ImGuiKey_Space,   "\uf0b2")
	/*
	DO(type,          function,           key,              icon)
	*/

#define TOOL_LIST_EXPAND_AS_ENUM(type, function, key, icon) \
	type,

#define TOOL_LIST_EXPAND_AS_CASE_FUNC(type, function, key, icon) \
	case type: function(); break;

#define TOOL_LIST_EXPAND_AS_INPUT_CANVAS(type, function, key, icon) \
	if(ImGui::IsKeyPressed(key) && !ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) Canvas::setTool(Canvas::type);

#define TOOL_LIST_EXPAND_AS_ICON_LIST(type, function, key, icon) \
	icon,

#endif
