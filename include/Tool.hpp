#ifndef TOOL_HPP
#define TOOL_HPP

#define FOR_TOOL_LIST(DO) \
	DO(TOOL_SELECT,   handleToolSelect,   ImGuiKey_V,   "\uf245") \
	DO(TOOL_BRUSH,    handleToolBrush,    ImGuiKey_B,   "\uf304") \
	DO(TOOL_PICKER,   handleToolPicker,   ImGuiKey_I,   "\uf1fb") \
	DO(TOOL_BUCKET,   handleToolBucket,   ImGuiKey_F,   "\uf576") \
	DO(TOOL_INVERT,   handleToolInvert,   ImGuiKey_T,   "\uf07e") \
	DO(TOOL_RECT,     handleToolRect,     ImGuiKey_R,   "\uf0c8") \
	DO(TOOL_LINE,     handleToolLine,     ImGuiKey_Q,   "\uf5ad") 

#define TOOL_LIST_EXPAND_AS_ENUM(type, ...) \
	type,

#define TOOL_LIST_EXPAND_AS_CASE_FUNC(type, function, ...) \
	case type: function(); break;

#define TOOL_LIST_EXPAND_AS_INPUT_CANVAS(type, function, key, ...) \
	if(ImGui::IsKeyPressed(key) && !ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) Canvas::setTool(Canvas::type);

#define TOOL_LIST_EXPAND_AS_ICON_LIST(type, function, key, icon) \
	icon,

#endif
