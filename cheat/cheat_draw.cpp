#include "cheat_draw.h"
#include "cheat_hooks.h"

void DrawBox(Vec3 position, float rotate, ImVec4 color, float tickness)
{
	float fullRotate = M_PI * 2.f;
	
	Vec3 pos1 = {0,0,0}, pos2 = {0,0,0}, pos3 = {0,0,0}, pos4 = {0,0,0};
	Vec3 scrPos1 = {0,0,0}, scrPos2 = {0,0,0}, scrPos3 = {0,0,0}, scrPos4 = {0,0,0};
	
	float x2 = position.x + (0.3f * cos(rotate));
	float y2 = position.y + (0.3f * sin(rotate));
	
	pos1 = Vec3(x2, y2, position.z - 0.8f);
	
	//
	if(WorldToScreen(&pos1, &scrPos1))
	{
		pos2 = Vec3(x2, y2, position.z + 0.8f);
		if(WorldToScreen(&pos2, &scrPos2))
		{
			x2 = position.x + (0.3f * cos(rotate + M_PI));
			y2 = position.y + (0.3f * sin(rotate + M_PI));
			pos3 = Vec3(x2, y2, position.z + 0.8f);
			if(WorldToScreen(&pos3, &scrPos3))
			{
				pos4 = Vec3(x2, y2, position.z - 0.8f);
				if(WorldToScreen(&pos4, &scrPos4))
				{
					if(scrPos1.z >= 1.f && scrPos2.z >= 1.f && scrPos3.z >= 1.f && scrPos4.z >= 1.f)
					{
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(scrPos1.x, scrPos1.y), ImVec2(scrPos2.x, scrPos2.y), ImGui::ColorConvertFloat4ToU32(color), tickness);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(scrPos2.x, scrPos2.y), ImVec2(scrPos3.x, scrPos3.y), ImGui::ColorConvertFloat4ToU32(color), tickness);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(scrPos3.x, scrPos3.y), ImVec2(scrPos4.x, scrPos4.y), ImGui::ColorConvertFloat4ToU32(color), tickness);
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(scrPos4.x, scrPos4.y), ImVec2(scrPos1.x, scrPos1.y), ImGui::ColorConvertFloat4ToU32(color), tickness);
					}
				}
			}
		}
	}
}

void TextWithOutline(ImDrawList* draw_list, ImVec2 pos, ImU32 text_color, const char* text, float font_size, ImU32 outline_color)
{
    // Получаем текущий шрифт ImGui
    ImFont* font = ImGui::GetFont();
    // left
    pos.x -= 2.f;
    draw_list->AddText(font, font_size, pos, outline_color, text);
    pos.x += 2.f;
    // right
    pos.x += 2.f;
    draw_list->AddText(font, font_size, pos, outline_color, text);
    pos.x -= 2.f;
    // above
    pos.y -= 2.f;
    draw_list->AddText(font, font_size, pos, outline_color, text);
    pos.y += 2.f;
    // below
    pos.y += 2.f;
    draw_list->AddText(font, font_size, pos, text_color, text, text);
    pos.y -= 3.f;
    draw_list->AddText(font, font_size, pos, text_color, text);
}
