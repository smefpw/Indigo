// [src] https://github.com/ocornut/imgui/issues/346
// v2.36
// TODO: try to integrate properly within ColorEdit4()
// TODO: expose size (how?)
// TODO: expose ID in api?
// TODO: massage, cleanup

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h" // ImSaturate

#define IM_F32_TO_INT8(_VAL) ((int)((_VAL) * 255.0f + 0.5f))


static const char*  FindTextDisplayEnd(const char* text, const char* text_end = NULL) {
	const char* text_display_end = text;
	if (!text_end)
		text_end = (const char*)-1;

	while (text_display_end < text_end && *text_display_end != '\0' && (text_display_end[0] != '#' || text_display_end[1] != '#'))
		text_display_end++;
	return text_display_end;
}

IMGUI_API bool ImGui::MyColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags) {
	float col4[4] = { col[0], col[1], col[2], 1.0f };
	if (!MyColorEdit4(label, col4, flags & ~ImGuiColorEditFlags_Alpha))
		return false;
	col[0] = col4[0]; col[1] = col4[1]; col[2] = col4[2];
	return true;
}


// Edit colors components (each component in 0.0f..1.0f range)
// Click on colored square to open a color picker (unless ImGuiColorEditFlags_NoPicker is set). Use CTRL-Click to input value and TAB to go to next item.
IMGUI_API bool ImGui::MyColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const float w_full = CalcItemWidth();
	const float square_sz_with_spacing = (flags & ImGuiColorEditFlags_NoColorSquare) ? 0.0f : (g.FontSize + style.FramePadding.y * 2.0f + style.ItemInnerSpacing.x);

	// If no mode is specified, defaults to RGB
	if (!(flags & ImGuiColorEditFlags_ModeMask_))
		flags |= ImGuiColorEditFlags_RGB;

	// If we're not showing any slider there's no point in querying color mode, nor showing the options menu, nor doing any HSV conversions
	if (flags & ImGuiColorEditFlags_NoSliders)
		flags = (flags & (~ImGuiColorEditFlags_ModeMask_)) | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_NoOptions;

	// Read back edit mode from persistent storage
	if (!(flags & ImGuiColorEditFlags_NoOptions))
		flags = (flags & (~ImGuiColorEditFlags_ModeMask_)) | (g.ColorEditModeStorage.GetInt(id, (flags & ImGuiColorEditFlags_ModeMask_)) & ImGuiColorEditFlags_ModeMask_);

	// Check that exactly one of RGB/HSV/HEX is set
	//IM_ASSERT(ImIsPowerOfTwo((int)(flags & ImGuiColorEditFlags_ModeMask_))); // 

	float f[4] = { col[0], col[1], col[2], col[3] };
	if (flags & ImGuiColorEditFlags_HSV)
		ImGui::ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

	int i[4] = { IM_F32_TO_INT8(f[0]), IM_F32_TO_INT8(f[1]), IM_F32_TO_INT8(f[2]), IM_F32_TO_INT8(f[3]) };

	bool alpha = (flags & ImGuiColorEditFlags_Alpha) != 0;
	bool value_changed = false;
	int components = alpha ? 4 : 3;

	ImGui::BeginGroup();
	ImGui::PushID(label);

	if ((flags & (ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV)) != 0 && (flags & ImGuiColorEditFlags_NoSliders) == 0) {
		// RGB/HSV 0..255 Sliders
		const float w_items_all = w_full - square_sz_with_spacing;
		const float w_item_one = ImMax(1.0f, (float)(int)((w_items_all - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
		const float w_item_last = ImMax(1.0f, (float)(int)(w_items_all - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

		const bool hide_prefix = (w_item_one <= CalcTextSize("M:999").x);
		const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
		const char* fmt_table[3][4] =
		{
			{ "%3.0f",   "%3.0f",   "%3.0f",   "%3.0f" }, // Short display
			{ "R:%3.0f", "G:%3.0f", "B:%3.0f", "A:%3.0f" }, // Long display for RGBA
			{ "H:%3.0f", "S:%3.0f", "V:%3.0f", "A:%3.0f" }  // Long display for HSVV
		};
		const char** fmt = hide_prefix ? fmt_table[0] : (flags & ImGuiColorEditFlags_HSV) ? fmt_table[2] : fmt_table[1];

		ImGui::PushItemWidth(w_item_one);
		for (int n = 0; n < components; n++) {
			if (n > 0)
				ImGui::SameLine(0, style.ItemInnerSpacing.x);
			if (n + 1 == components)
				ImGui::PushItemWidth(w_item_last);
			value_changed |= ImGui::DragInt(ids[n], &i[n], 1.0f, 0, 255, fmt[n]);
		}
		ImGui::PopItemWidth();
		ImGui::PopItemWidth();
	}
	else if ((flags & ImGuiColorEditFlags_HEX) != 0 && (flags & ImGuiColorEditFlags_NoSliders) == 0) {
		// RGB Hexadecimal Input
		const float w_slider_all = w_full - square_sz_with_spacing;
		char buf[64];
		if (alpha)
			ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", i[0], i[1], i[2], i[3]);
		else
			ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", i[0], i[1], i[2]);
		ImGui::PushItemWidth(w_slider_all);
		if (ImGui::InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase)) {
			value_changed |= true;
			char* p = buf;
			while (*p == '#' || ImCharIsSpace(*p))
				p++;
			i[0] = i[1] = i[2] = i[3] = 0;
			if (alpha)
				sscanf_s(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]); // Treat at unsigned (%X is unsigned)
			else
				sscanf_s(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
		}
		ImGui::PopItemWidth();
	}

	const char* label_display_end = FindTextDisplayEnd(label);

	bool picker_active = false;
	if (!(flags & ImGuiColorEditFlags_NoColorSquare)) {
		if (!(flags & ImGuiColorEditFlags_NoSliders))
			ImGui::SameLine(0, style.ItemInnerSpacing.x);

		const ImVec4 col_display(col[0], col[1], col[2], 1.0f);
		if (ImGui::ColorButton(col_display)) {
			if (!(flags & ImGuiColorEditFlags_NoPicker)) {
				ImGui::OpenPopup("picker");
				ImGui::SetNextWindowPos(window->DC.LastItemRect.GetBL() + ImVec2(-1, style.ItemSpacing.y));
			}
		}
		else if (!(flags & ImGuiColorEditFlags_NoOptions) && ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
			ImGui::OpenPopup("context");
		}

		if (ImGui::BeginPopup("picker")) {
			picker_active = true;
			if (label != label_display_end)
				ImGui::TextUnformatted(label, label_display_end);
			ImGui::PushItemWidth(256.0f + (alpha ? 2 : 1) * (style.ItemInnerSpacing.x));
			value_changed |= ImGui::MyColorPicker4("##picker", col, (flags & ImGuiColorEditFlags_Alpha) | (ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV | ImGuiColorEditFlags_HEX));
			ImGui::PopItemWidth();
			ImGui::EndPopup();
		}
		if (!(flags & ImGuiColorEditFlags_NoOptions) && ImGui::BeginPopup("context")) {
			// FIXME-LOCALIZATION
			if (ImGui::MenuItem("Edit as RGB", NULL, (flags & ImGuiColorEditFlags_RGB) ? 1 : 0)) g.ColorEditModeStorage.SetInt(id, (int)(ImGuiColorEditFlags_RGB));
			if (ImGui::MenuItem("Edit as HSV", NULL, (flags & ImGuiColorEditFlags_HSV) ? 1 : 0)) g.ColorEditModeStorage.SetInt(id, (int)(ImGuiColorEditFlags_HSV));
			if (ImGui::MenuItem("Edit as Hexadecimal", NULL, (flags & ImGuiColorEditFlags_HEX) ? 1 : 0)) g.ColorEditModeStorage.SetInt(id, (int)(ImGuiColorEditFlags_HEX));
			ImGui::EndPopup();
		}

		// Recreate our own tooltip over's ColorButton() one because we want to display correct alpha here
		//if (ImGui::IsItemHovered())
		//	ImGui::SetTooltip("Color:\n(%.2f,%.2f,%.2f,%.2f)\n#%02X%02X%02X%02X", col[0], col[1], col[2], col[3], IM_F32_TO_INT8(col[0]), IM_F32_TO_INT8(col[1]), IM_F32_TO_INT8(col[2]), IM_F32_TO_INT8(col[3]));
		//ImGui::SetTooltip("");
	}

	if (label != label_display_end) {
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		ImGui::TextUnformatted(label, label_display_end);
	}

	// Convert back
	if (!picker_active) {
		for (int n = 0; n < 4; n++)
			f[n] = i[n] / 255.0f;
		if (flags & ImGuiColorEditFlags_HSV)
			ImGui::ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
		if (value_changed) {
			col[0] = f[0];
			col[1] = f[1];
			col[2] = f[2];
			if (alpha)
				col[3] = f[3];
		}
	}

	ImGui::PopID();
	ImGui::EndGroup();

	return value_changed;
}

IMGUI_API bool ImGui::MyColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags) {
	float col4[4] = { col[0], col[1], col[2], 1.0f };
	if (!MyColorPicker4(label, col4, flags & ~ImGuiColorEditFlags_Alpha))
		return false;
	col[0] = col4[1]; col[1] = col4[1]; col[2] = col4[2];
	return true;
}

// ColorPicker v2.50 WIP 
// see https://github.com/ocornut/imgui/issues/346
// TODO: Missing color square
// TODO: English strings in context menu (see FIXME-LOCALIZATION)

bool ImGui::MyColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags) {
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImGui::PushID(label);
	ImGui::BeginGroup();

	// Setup
	bool alpha = (flags & ImGuiColorEditFlags_Alpha) != 0;
	ImVec2 picker_pos = ImGui::GetCursorScreenPos();
	float bars_width = ImGui::GetWindowFontSize() * 1.0f;                                                           // Arbitrary smallish width of Hue/Alpha picking bars
	float sv_picker_size = ImMax(bars_width * 2, ImGui::CalcItemWidth() - (alpha ? 2 : 1) * (bars_width + style.ItemInnerSpacing.x)); // Saturation/Value picking box
	float bar0_pos_x = picker_pos.x + sv_picker_size + style.ItemInnerSpacing.x;
	float bar1_pos_x = bar0_pos_x + bars_width + style.ItemInnerSpacing.x;

	float H, S, V;
	ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], H, S, V);

	// Color matrix logic
	bool value_changed = false, hsv_changed = false;
	//ImGui::BeginGroup();
	ImGui::InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size));
	if (ImGui::IsItemActive()) {
		S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size - 1));
		V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
		value_changed = hsv_changed = true;
	}

	// Hue bar logic
	ImGui::SetCursorScreenPos(ImVec2(bar0_pos_x, picker_pos.y));
	ImGui::InvisibleButton("hue", ImVec2(bars_width, sv_picker_size));
	if (ImGui::IsItemActive()) {
		H = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
		value_changed = hsv_changed = true;
	}

	// Alpha bar logic
	if (alpha) {
		ImGui::SetCursorScreenPos(ImVec2(bar1_pos_x, picker_pos.y));
		ImGui::InvisibleButton("alpha", ImVec2(bars_width, sv_picker_size));
		if (ImGui::IsItemActive()) {
			col[3] = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
			value_changed = true;
		}
	}

	const char* label_display_end = FindTextDisplayEnd(label);
	if (label != label_display_end) {
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		ImGui::TextUnformatted(label, label_display_end);
	}

	// Convert back color to RGB
	if (hsv_changed)
		ImGui::ColorConvertHSVtoRGB(H >= 1.0f ? H - 10 * 1e-6f : H, S > 0.0f ? S : 10 * 1e-6f, V > 0.0f ? V : 1e-6f, col[0], col[1], col[2]);

	// R,G,B and H,S,V slider color editor
	if (!(flags & ImGuiColorEditFlags_NoSliders)) {
		if ((flags & ImGuiColorEditFlags_ModeMask_) == 0)
			flags = ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV | ImGuiColorEditFlags_HEX;
		ImGui::PushItemWidth((alpha ? bar1_pos_x : bar0_pos_x) + bars_width - picker_pos.x);
		ImGuiColorEditFlags sub_flags = (alpha ? ImGuiColorEditFlags_Alpha : 0) | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoColorSquare;
		//if (flags & ImGuiColorEditFlags_RGB)
		//	value_changed |= ImGui::MyColorEdit4("##rgb", col, sub_flags | ImGuiColorEditFlags_RGB);
		//if (flags & ImGuiColorEditFlags_HSV)
		//	value_changed |= ImGui::MyColorEdit4("##hsv", col, sub_flags | ImGuiColorEditFlags_HSV);
		//if (flags & ImGuiColorEditFlags_HEX)
		//	value_changed |= ImGui::MyColorEdit4("##hex", col, sub_flags | ImGuiColorEditFlags_HEX);
		ImGui::PopItemWidth();
	}

	// Try to cancel hue wrap (after ColorEdit), if any
	if (value_changed) {
		float new_H, new_S, new_V;
		ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
		if (new_H <= 0 && H > 0) {
			if (new_V <= 0 && V != new_V)
				ImGui::ColorConvertHSVtoRGB(H, S, new_V <= 0 ? V * 0.5f : new_V, col[0], col[1], col[2]);
			else if (new_S <= 0)
				ImGui::ColorConvertHSVtoRGB(H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[0], col[1], col[2]);
		}
	}

	// Render hue bar
	ImVec4 hue_color_f(1, 1, 1, 1);
	ImGui::ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
	ImU32 hue_colors[] = { IM_COL32(255,0,0,255), IM_COL32(255,255,0,255), IM_COL32(0,255,0,255), IM_COL32(0,255,255,255), IM_COL32(0,0,255,255), IM_COL32(255,0,255,255), IM_COL32(255,0,0,255) };
	for (int i = 0; i < 6; ++i) {
		draw_list->AddRectFilledMultiColor(
			ImVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size / 6)),
			ImVec2(bar0_pos_x + bars_width, picker_pos.y + (i + 1) * (sv_picker_size / 6)),
			hue_colors[i], hue_colors[i], hue_colors[i + 1], hue_colors[i + 1]);
	}
	float bar0_line_y = (float)(int)(picker_pos.y + H * sv_picker_size + 0.5f);
	draw_list->AddLine(ImVec2(bar0_pos_x - 1, bar0_line_y), ImVec2(bar0_pos_x + bars_width + 1, bar0_line_y), IM_COL32_WHITE);

	// Render alpha bar
	if (alpha) {
		float alpha = ImSaturate(col[3]);
		float bar1_line_y = (float)(int)(picker_pos.y + (1.0f - alpha) * sv_picker_size + 0.5f);
		draw_list->AddRectFilledMultiColor(ImVec2(bar1_pos_x, picker_pos.y), ImVec2(bar1_pos_x + bars_width, picker_pos.y + sv_picker_size), IM_COL32_WHITE, IM_COL32_WHITE, IM_COL32_BLACK, IM_COL32_BLACK);
		draw_list->AddLine(ImVec2(bar1_pos_x - 1, bar1_line_y), ImVec2(bar1_pos_x + bars_width + 1, bar1_line_y), IM_COL32_WHITE);
	}

	// Render color matrix
	ImU32 hue_color32 = ImGui::ColorConvertFloat4ToU32(hue_color_f);
	draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), IM_COL32_WHITE, hue_color32, hue_color32, IM_COL32_WHITE);
	draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), IM_COL32_BLACK_TRANS, IM_COL32_BLACK_TRANS, IM_COL32_BLACK, IM_COL32_BLACK);

	// Render cross-hair
	const float CROSSHAIR_SIZE = 7.0f;
	ImVec2 p((float)(int)(picker_pos.x + S * sv_picker_size + 0.5f), (float)(int)(picker_pos.y + (1 - V) * sv_picker_size + 0.5f));
	draw_list->AddLine(ImVec2(p.x - CROSSHAIR_SIZE, p.y), ImVec2(p.x - 2, p.y), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x + CROSSHAIR_SIZE, p.y), ImVec2(p.x + 2, p.y), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x, p.y + CROSSHAIR_SIZE), ImVec2(p.x, p.y + 2), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x, p.y - CROSSHAIR_SIZE), ImVec2(p.x, p.y - 2), IM_COL32_WHITE);
	ImGui::EndGroup();
	ImGui::PopID();

	return value_changed;
}