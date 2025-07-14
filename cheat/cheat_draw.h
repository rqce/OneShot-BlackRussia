#pragma once

#include "stdint.h"
#include "interface/ImGui/imgui.h"
#include "vector/vector3.h"
#include <cmath>

void DrawBox(Vec3 position, float rotate, ImVec4 color, float tickness);
void TextWithOutline(ImDrawList* draw_list, ImVec2 pos, ImU32 text_color, const char* text, float font_size, ImU32 outline_color);
