#include "includes.h"

int libAddr = 0;
int glWidth, glHeight;
bool setup = false;

int gpeer = 0, ga7 = 0, ga8 = 0;
Vec3 start_shot = {0,0,0}, end_shot = {0,0,0};
Vec3 screen_start = Vec3(0,0,0), screen_end = Vec3(0,0,0);
int last_ms = 0;
bool bot_miner = false;
float crosshair_offset_x = 0.53f, crosshair_offset_y = 0.43f;
bool logga = false;
bool trianglesvh = false;

// Menu
int alphaWindowBg = 255;
int alphaChildBg = 255;
int icon_size = 130;
int start_touch_x = 0, start_touch_y = 0;
int current_menu_category = 0;
int current_player_category = 0;
int current_other_category = 0;
int damager_settings_active_tab = 0;

bool m_bNeedClearMousePos = false;
bool menuOpen = true;
bool hideIcon = false;
bool textureInited = false;
bool needShowText = false;
bool icon_is_hovered = false;
bool menu_is_hovered = false;
bool wallhack_menu_is_hovered = false;
bool game_speed_menu_is_hovered = false;
bool change_car_model_menu_is_hovered = false;
bool tp_toPeds_menu_is_hovered = false;

bool wh_window_not_inited = true;
bool tp_toPeds_inited = false;
bool game_speed_inited = false;

// Trash
void log1(int,int,int,int, float);

GLuint LoadTexture(const char* path);
GLuint icon_texture = 0;
GLuint icon_alpha_texture = 0;

ImVec4 rgba(int r, int g, int b, int a = 255)
{
	float retR = (float)r / 255.f;
	float retG = (float)g / 255.f;
	float retB = (float)b / 255.f;
	float retA = (float)a / 255.f;
	
	return ImVec4(retR, retG, retB, retA);
}

void startService(JNIEnv *env, jobject ctx){
    jclass native_context = env->GetObjectClass(ctx);
    jclass intentClass = env->FindClass(OBFUSCATE("android/content/Intent"));
    jclass actionString = env->FindClass(OBFUSCATE("wrapper/MyService"));
    jmethodID newIntent = env->GetMethodID(intentClass, OBFUSCATE("<init>"), OBFUSCATE("(Landroid/content/Context;Ljava/lang/Class;)V"));
    jobject intent = env->NewObject(intentClass,newIntent,ctx,actionString);
    jmethodID startActivityMethodId = env->GetMethodID(native_context, OBFUSCATE("startService"), OBFUSCATE("(Landroid/content/Intent;)Landroid/content/ComponentName;"));
    env->CallObjectMethod(ctx, startActivityMethodId, intent);
}

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
   eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
   eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);
   
	if (!setup) {
        IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)glWidth, (float)glHeight);
		ImGui::StyleColorsDark();
		ImGui_ImplOpenGL3_Init(OBFUSCATE("#version 100"));
		io.Fonts->AddFontFromMemoryTTF(custom_font, sizeof(custom_font), 30.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
		
		ImFontConfig config;
		config.MergeMode = true;
		config.GlyphMinAdvanceX = 0.f;
		config.GlyphOffset = ImVec2(0,5.f);
		static const ImWchar icon_ranges[] = { ICON_MIN, ICON_MAX, 0 };
		io.Fonts->AddFontFromMemoryTTF(boxicons_ttf, sizeof(boxicons_ttf), 32.f, &config, icon_ranges);
		
		ImGui::GetStyle().ScaleAllSizes(3.0f);
		
        setup = true;
    }
	
	if(!textureInited)
	{
		const char* path_to_icon = OBFUSCATE("/storage/emulated/0/Android/data/com.br.top/files/icon.png");
		const char* path_to_alpha_icon = OBFUSCATE("/storage/emulated/0/Android/data/com.br.top/files/alpha_icon.png");
		std::ofstream outFile1(path_to_icon, std::ios::binary);
		
		if(outFile1.is_open())
		{
			outFile1.write(reinterpret_cast<char*>(icon_image.data()), icon_image.size());
			outFile1.close();
			icon_texture = LoadTexture(OBFUSCATE("/storage/emulated/0/Android/data/com.br.top/files/icon.png"));
		}
		
		std::ofstream outFile2(path_to_alpha_icon, std::ios::binary);
		
		if(outFile2.is_open())
		{
			outFile2.write(reinterpret_cast<char*>(alpha_icon_image.data()), alpha_icon_image.size());
			outFile2.close();
			icon_alpha_texture = LoadTexture(OBFUSCATE("/storage/emulated/0/Android/data/com.br.top/files/alpha_icon.png"));
		}
		textureInited = true;
	}

    ImGuiIO &io = ImGui::GetIO();
	
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
	
	if(!hideIcon)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
	
		ImGui::Begin(OBFUSCATE("mod-icon"), NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);
		{
			if(textureInited)
			{
				ImGui::SetWindowSize(ImVec2(icon_size + 1, icon_size + 1));
				if(!icon_is_hovered) {
					ImGui::Image((void*)(intptr_t)icon_texture, ImVec2(icon_size, icon_size));
				} else {
					ImGui::Image((void*)(intptr_t)icon_alpha_texture, ImVec2(icon_size, icon_size));
				}
			}
		}
		ImGui::End();
		
		ImGui::PopStyleVar();
	}
	
	if(menuOpen)
	{
		// Build menu
    	//ImGui::ShowDemoWindow();
	
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Text] = rgba(255,255,255);
		style.Colors[ImGuiCol_WindowBg] = rgba(17,17,17,alphaWindowBg);
		style.Colors[ImGuiCol_ChildBg] = rgba(31,31,31,alphaChildBg);
		style.Colors[ImGuiCol_FrameBg] = rgba(51,51,51);
		style.Colors[ImGuiCol_PopupBg] = rgba(17,17,17);
		style.Colors[ImGuiCol_Text] = rgba(255,255,255);
		style.Colors[ImGuiCol_Button] = rgba(51,51,51);
		style.Colors[ImGuiCol_ButtonHovered] = rgba(255,70,70);
		style.Colors[ImGuiCol_ButtonActive] = rgba(255,55,55);
		style.Colors[ImGuiCol_SliderGrab] = rgba(255,55,55);
		style.Colors[ImGuiCol_SliderGrabActive] = rgba(255,55,55,200);
		style.Colors[ImGuiCol_CheckMark] = rgba(255,55,55);
		style.Colors[ImGuiCol_FrameBgHovered] = rgba(255,55,55,100);
		style.Colors[ImGuiCol_FrameBgActive] = rgba(255,55,55,100);
		style.Colors[ImGuiCol_Header] = rgba(80,80,80);
		style.Colors[ImGuiCol_HeaderHovered] = rgba(255,55,55);
		style.Colors[ImGuiCol_HeaderActive] = rgba(255,55,55);
		style.Colors[ImGuiCol_Tab] = rgba(51,51,51);
		style.Colors[ImGuiCol_TabHovered] = rgba(255,70,70);
		style.Colors[ImGuiCol_TabActive] = rgba(255,55,55);
		style.Colors[ImGuiCol_TitleBg] = rgba(17,17,17);
		style.Colors[ImGuiCol_TitleBgActive] = rgba(255,70,70);
		
		ImGui::GetStyle().WindowRounding = 15.f;
		ImGui::GetStyle().ChildRounding = 15.f;
		ImGui::GetStyle().FrameRounding = 15.f;
		ImGui::GetStyle().GrabRounding = 10.f;
		
	
		ImGui::Begin(OBFUSCATE("OneShot"), NULL, ImGuiWindowFlags_NoTitleBar);
		{
			float categoryButtonWidth = ImGui::CalcTextSize(OBFUSCATE("ХХХХХХХХХХ")).x * 2.f;
			float categoryButtonHeight = ImGui::CalcTextSize(OBFUSCATE("X")).y * 2.f;
		
			ImGui::PushStyleColor(ImGuiCol_ChildBg, rgba(17,17,17,0));
			ImGui::BeginChild(OBFUSCATE("#left-tab"), ImVec2(categoryButtonWidth, ImGui::CalcTextSize(OBFUSCATE("X")).y * 22.f), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			{
				ImGui::PushStyleColor(ImGuiCol_ChildBg, rgba(31,31,31));
				ImGui::BeginChild(OBFUSCATE("#expire-info"), ImVec2(categoryButtonWidth, ImGui::CalcTextSize(OBFUSCATE("X")).y * 5.f));
				{
					ImGui::Spacing();
					ImGui::Spacing();
					
					ImGui::SetCursorPosX((categoryButtonWidth - (ImGui::CalcTextSize(OBFUSCATE("ONESHOT 3.0.0")).x)) / 2);
					ImGui::Text(OBFUSCATE("ONESHOT 3.0.0"));
					
					ImGui::PushStyleColor(ImGuiCol_ChildBg, rgba(17,17,17));
					ImGui::SetCursorPosX((categoryButtonWidth - (categoryButtonWidth * 0.9f)) / 2);
					ImGui::BeginChild(OBFUSCATE("##settabs"), ImVec2(categoryButtonWidth * 0.9f, categoryButtonHeight * 1.2f));
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2));
						ImGui::SetCursorPosY((categoryButtonHeight * 1.2f - categoryButtonHeight) / 2);
						if(current_menu_category == -1)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							ImGui::Button(ICON_INFO_CIRCLE, ImVec2(((categoryButtonWidth * 0.9f) / 2) - (((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2), categoryButtonHeight));
							ImGui::PopStyleColor();
						}
						else
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(17,17,17));
							if(ImGui::Button(ICON_INFO_CIRCLE, ImVec2(((categoryButtonWidth * 0.9f) / 2) - (((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2), categoryButtonHeight)))
							{
								current_menu_category = -1;
							}
							ImGui::PopStyleColor();
						}
						
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2) * 2) + (((categoryButtonWidth * 0.9f) / 2) - (((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2)));
						ImGui::SetCursorPosY((categoryButtonHeight * 1.2f - categoryButtonHeight) / 2);
						if(current_menu_category == -2)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							ImGui::Button(ICON_BXS_COG, ImVec2(((categoryButtonWidth * 0.9f) / 2) - ((((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2) * 2), categoryButtonHeight));
							ImGui::PopStyleColor();
						}
						else
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(17,17,17));
							if(ImGui::Button(ICON_BXS_COG, ImVec2(((categoryButtonWidth * 0.9f) / 2) - ((((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2) * 2), categoryButtonHeight)))
							{
								current_menu_category = -2;
							}
							ImGui::PopStyleColor();
						}
					}
					ImGui::EndChild();
					ImGui::PopStyleColor();
				}
				ImGui::EndChild();
				
				ImGui::Spacing();
				
				ImGui::BeginChild(OBFUSCATE("#category"), ImVec2(categoryButtonWidth, ImGui::CalcTextSize(OBFUSCATE("X")).y * 16.f));
				{
					ImGui::Spacing();
					ImGui::Spacing();
					
					ImGui::SetCursorPosX((categoryButtonWidth - (categoryButtonWidth * 0.9f)) / 2);
					if(current_menu_category == 0) {
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255, 55, 55));
						ImGui::Button(OBFUSCATE(ICON_USER u8" Персонаж"), ImVec2(categoryButtonWidth * 0.9f, categoryButtonHeight));
						ImGui::PopStyleColor();
					} else {
						if(ImGui::Button(OBFUSCATE(ICON_USER u8" Персонаж"), ImVec2(categoryButtonWidth * 0.9f, categoryButtonHeight)))
						{
							current_menu_category = 0;
						}
					}
				
					ImGui::Spacing();
					
					ImGui::SetCursorPosX((categoryButtonWidth - (categoryButtonWidth * 0.9f)) / 2);
					if(current_menu_category == 1) {
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255, 55, 55));
						ImGui::Button(OBFUSCATE(ICON_TRUCK u8" Транспорт"), ImVec2(categoryButtonWidth * 0.9f, categoryButtonHeight));
						ImGui::PopStyleColor();
					} else {
						if(ImGui::Button(OBFUSCATE(ICON_TRUCK u8" Транспорт"), ImVec2(categoryButtonWidth * 0.9f, categoryButtonHeight)))
						{
							current_menu_category = 1;
						}
					}
					
					ImGui::Spacing();
					
					ImGui::SetCursorPosX((categoryButtonWidth - (categoryButtonWidth * 0.9f)) / 2);
					if(current_menu_category == 2) {
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255, 55, 55));
						ImGui::Button(OBFUSCATE(ICON_LOW_VISION u8" Визуалы"), ImVec2(categoryButtonWidth * 0.9f, categoryButtonHeight));
						ImGui::PopStyleColor();
					} else {
						if(ImGui::Button(OBFUSCATE(ICON_LOW_VISION u8" Визуалы"), ImVec2(categoryButtonWidth * 0.9f, categoryButtonHeight)))
						{
							current_menu_category = 2;
						}
					}
				
					ImGui::Spacing();
					
					ImGui::SetCursorPosX((categoryButtonWidth - (categoryButtonWidth * 0.9f)) / 2);
					if(current_menu_category == 3) {
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255, 55, 55));
						ImGui::Button(OBFUSCATE(ICON_MENU u8" Другое"), ImVec2(categoryButtonWidth * 0.9f, categoryButtonHeight));
						ImGui::PopStyleColor();
					} else {
						if(ImGui::Button(OBFUSCATE(ICON_MENU u8" Другое"), ImVec2(categoryButtonWidth * 0.9f, categoryButtonHeight)))
						{
							current_menu_category = 3;
						}
					}
				
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					
					ImGui::SetCursorPosX((categoryButtonWidth - (categoryButtonWidth * 0.9f)) / 2);
					if(ImGui::Button(OBFUSCATE(ICON_X u8" Закрыть"), ImVec2(categoryButtonWidth * 0.9f, categoryButtonHeight)))
					{
						menuOpen = false;
					}
				}
				ImGui::EndChild();
				ImGui::PopStyleColor();
			}
			ImGui::EndChild();
			ImGui::PopStyleColor();
			
			ImGui::SameLine();
			
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 10.f));
			ImGui::BeginChild(OBFUSCATE("#main-tab"), ImVec2(740, ImGui::CalcTextSize(OBFUSCATE("X")).y * 22.f));
			{
				if(current_menu_category == -1)
				{
					ImGui::BeginChild(OBFUSCATE("##info-menu"), ImVec2(720, ImGui::CalcTextSize(OBFUSCATE("X")).y * 21.f));
					{
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::Spacing();
						
						ImGui::SameLine();
						if (ImGui::CollapsingHeader(OBFUSCATE("Damager ( Дамагер )"), ImGuiTreeNodeFlags_None))
        				{
							ImGui::Spacing();
							ImGui::SameLine();
							ImGui::Text(OBFUSCATE("Дамагер – наносит урон всем на ближнем расстоянии"));
						}
						
						ImGui::Spacing();
						ImGui::SameLine();
						
						ImGui::Text(cheatClient->response.c_str());
						
						/*if (ImGui::CollapsingHeader(OBFUSCATE("Debug Info"), ImGuiTreeNodeFlags_None))
        				{
							ImGui::Spacing();
							ImGui::SameLine();
							ImGui::Text("Кол-во игроков на сервере: %d", cheatClient->ServerPlayersCount);
							ImGui::Spacing();
							ImGui::SameLine();
							ImGui::Text("Размер кастом пула игроков: %d", sizeof(cheatClient->ServerPlayers));
							ImGui::Spacing();
							ImGui::SameLine();
							
							for(int i = 0; i < sizeof(cheatClient->ServerPlayers); i++)
							{
								ImGui::Text("%s [%d]", cheatClient->ServerPlayers[i].nickname, cheatClient->ServerPlayers[i].id);
								
							}
						}*/
					}
					ImGui::EndChild();
				}
				
				if(current_menu_category == -2)
				{
					ImGui::BeginChild(OBFUSCATE("##settings-menu"), ImVec2(720, ImGui::CalcTextSize(OBFUSCATE("X")).y * 21.f));
					{
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::Spacing();
						
						ImGui::SameLine();
						if (ImGui::CollapsingHeader(OBFUSCATE("Настройки меню"), ImGuiTreeNodeFlags_None))
        				{
							ImGui::Spacing();
							
							ImGui::SameLine();
							
							ImGui::Checkbox(OBFUSCATE("Скрыть иконку"), &hideIcon);
							
							ImGui::Spacing();
							ImGui::Spacing();
							
							ImGui::SameLine();
							ImGui::Text(OBFUSCATE("Размер иконки"));
						
							ImGui::Spacing();
						
							ImGui::SameLine();
							ImGui::SliderInt("", &icon_size, 50, 200);
						
							ImGui::Spacing();
							ImGui::Spacing();
						
							ImGui::SameLine();
							ImGui::Text(OBFUSCATE("Прозрачность меню"));
							ImGui::Spacing();
							ImGui::SameLine();
							if(ImGui::SliderInt(OBFUSCATE("##menubgalpha"), &alphaWindowBg, 0, 255))
							{
								alphaChildBg = alphaWindowBg;
							}
							
							ImGui::Spacing();
							ImGui::Spacing();
						
							ImGui::SameLine();
							ImGui::Text(OBFUSCATE("Кастомный прицел"));
							ImGui::Spacing();
							ImGui::SameLine();
							/*if(ImGui::SliderFloat("##customsightwidth", &cross_offset, 0.0f, 0.5f))
							{
								
							}
							
							ImGui::Spacing();
							ImGui::SameLine();
							if(ImGui::SliderFloat("##customsightheight", &crosshair_offset_y, 0.350f, 0.7f))
							{
								
							}*/
						}
						
						ImGui::Spacing();
					
						ImGui::SameLine();
						if (ImGui::CollapsingHeader(OBFUSCATE("Настройка подкидывания"), ImGuiTreeNodeFlags_None))
        				{
							ImGui::Spacing();
							ImGui::SameLine();
							ImGui::Text(OBFUSCATE("Подкидывание вверх"));
							ImGui::Spacing();
							ImGui::SameLine();
							ImGui::SliderFloat(OBFUSCATE("##slapUp"), &cheatClient->PlayerCheat.Base.Slap.up, 1.f, 10.f);
							ImGui::Spacing();
							ImGui::SameLine();
							ImGui::Text(OBFUSCATE("Подкидывание вниз"));
							ImGui::Spacing();
							ImGui::SameLine();
							ImGui::SliderFloat(OBFUSCATE("##slapDown"), &cheatClient->PlayerCheat.Base.Slap.down, 1.f, 10.f);
							
						}
					
						ImGui::Spacing();
					
						ImGui::SameLine();
						if (ImGui::CollapsingHeader(OBFUSCATE("Настройки персонажа"), ImGuiTreeNodeFlags_None))
        				{
							ImGui::Spacing();
							ImGui::SameLine();
							ImGui::Text(OBFUSCATE("Скорость персонажа"));
							ImGui::Spacing();
							ImGui::SameLine();
							// ImGui::SliderFloat("##speedhackped", &speed_hack_ped, 1.f, 5.f);
							ImGui::Spacing();
							ImGui::Separator();
							ImGui::Spacing();
							ImGui::SameLine();
							ImGui::Text(OBFUSCATE("Длинный прыжок"));
							ImGui::Spacing();
							ImGui::SameLine();
							if(ImGui::SliderFloat(OBFUSCATE("##longjumpped"), &cheatClient->PlayerCheat.Base.Jumping.long__, 1.f, 3.f))
							{
								if(cheatClient->PlayerCheat.Base.Jumping.long_) {
									WriteMemoryFloat(libAddr + 0x392FDC, 0.17f * cheatClient->PlayerCheat.Base.Jumping.long__);
								}
							}
							ImGui::Spacing();
							ImGui::Separator();
							ImGui::Spacing();
							ImGui::SameLine();
							ImGui::Text(OBFUSCATE("Высокий прыжок"));
							ImGui::Spacing();
							ImGui::SameLine();
							if(ImGui::SliderFloat(OBFUSCATE("##highjumpped"), &cheatClient->PlayerCheat.Base.Jumping.high__, 1.f, 3.f))
							{
								if(cheatClient->PlayerCheat.Base.Jumping.high) {
									uintptr_t localplayer = cheatClient->FindLocalPlayer();
									if(localplayer)
									{
										CPed localped = CPed(localplayer);
										localped.SetHighJumpMult(70.f / cheatClient->PlayerCheat.Base.Jumping.high__);
									}									
								}
							}
							ImGui::Spacing();
							ImGui::Separator();
							ImGui::Spacing();
							ImGui::SameLine();
							ImGui::Text(OBFUSCATE("Скорость поворота"));
							ImGui::Spacing();
							ImGui::SameLine();
							if(ImGui::SliderFloat(OBFUSCATE("##headingrateped"), &cheatClient->PlayerCheat.Base.Control.rate, 1.f, 4.f))
							{
								if(cheatClient->PlayerCheat.Base.Control.fastRotate) {
									uintptr_t localplayer = cheatClient->FindLocalPlayer();
									if(localplayer)
									{
										CPed localped = CPed(localplayer);
										localped.SetHeadingRate(7.5f * cheatClient->PlayerCheat.Base.Control.rate);
									}									
								}
							}
						}
					
						ImGui::Spacing();
					
						ImGui::SameLine();
						if (ImGui::CollapsingHeader(OBFUSCATE("Настройки транспорта"), ImGuiTreeNodeFlags_None))
        				{
							ImGui::Spacing();
							ImGui::SameLine();
							ImGui::Text(OBFUSCATE("CarShot ( ускорение )"));
							ImGui::Spacing();
							ImGui::SameLine();
							ImGui::SliderFloat(OBFUSCATE("##carshotAccelerate"), &cheatClient->VehicleCheat.CarShot.accelerate, 1.f, 10.f);
							
						}
						
						ImGui::Spacing();
					
						ImGui::SameLine();
						
					}
					ImGui::EndChild();
				}
				
				if(current_menu_category == 0)
				{
					ImGui::Spacing();
					ImGui::Spacing();
					
					ImVec2 currentCursorPos = ImGui::GetCursorPos();
					
					ImGui::PushStyleColor(ImGuiCol_ChildBg, rgba(17,17,17));
					ImGui::SameLine();
					ImGui::BeginChild(OBFUSCATE("##player-category"), ImVec2(700, categoryButtonHeight * 1.2f));
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2));
						ImGui::SetCursorPosY((categoryButtonHeight * 1.2f - categoryButtonHeight) / 2);
						if(current_player_category == 0)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							ImGui::Button(OBFUSCATE(u8"Основное"), ImVec2((700 / 2) - (((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2), categoryButtonHeight));
							ImGui::PopStyleColor();
						}
						else
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(17,17,17));
							if(ImGui::Button(OBFUSCATE(u8"Основное"), ImVec2((700 / 2) - (((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2), categoryButtonHeight)))
							{
								current_player_category = 0;
							}
							ImGui::PopStyleColor();
						}
						
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2) * 2) + ((700 / 2) - (((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2)));
						ImGui::SetCursorPosY((categoryButtonHeight * 1.2f - categoryButtonHeight) / 2);
						if(current_player_category == 1)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							ImGui::Button(OBFUSCATE(u8"Боевой режим"), ImVec2((700 / 2) - ((((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2) * 2), categoryButtonHeight));
							ImGui::PopStyleColor();
						}
						else
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(17,17,17));
							if(ImGui::Button(OBFUSCATE(u8"Боевой режим"), ImVec2((700 / 2) - ((((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2) * 2), categoryButtonHeight)))
							{
								current_player_category = 1;
							}
							ImGui::PopStyleColor();
						}
					}
					ImGui::EndChild();
					ImGui::PopStyleColor();
					
					ImGui::Spacing();
					
					if(current_player_category == 0)
					{
						ImGui::SameLine();
						ImGui::SameLine();
						if(cheatClient->PlayerCheat.Base.Invulnerability.health)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Бессмертие"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
							{
								cheatClient->PlayerCheat.Base.Invulnerability.health =! cheatClient->PlayerCheat.Base.Invulnerability.health;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Бессмертие"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Base.Invulnerability.health =! cheatClient->PlayerCheat.Base.Invulnerability.health;
							}
						}
						/*if(ImGui::Button("Бессмертие", ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
						{
							cheatClient->gmped =! cheatClient->gmped;
							for(int i = 0; i < 1500; i++)
							{
								int playerpool = cheatClient->GetPlayerPool();
								if(!playerpool) { continue; }
								uintptr_t player = CPlayerPool_GetAt(playerpool, i);
								if(player)
								{
									player = *(int *)(player + 4);
									if(player)
									{
										int number = *(int *)(player + 80);
										number &= ~0x100;
										*(int *)(player + 80) = number;
									}
								}
							}
							
							BrNotification(TYPE_TEXT_GREEN, "Бессмертие работает только от физики игры!", 5);
						}*/
						ImGui::SameLine();
						if(cheatClient->PlayerCheat.Base.Invulnerability.endurance)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Бесконечный Бег"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Base.Invulnerability.endurance =! cheatClient->PlayerCheat.Base.Invulnerability.endurance;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Бесконечный Бег"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Base.Invulnerability.endurance =! cheatClient->PlayerCheat.Base.Invulnerability.endurance;
							}
						}
						
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::SameLine();
						ImGui::SameLine();
						
						if(cheatClient->PlayerCheat.Base.FlyPed.state)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Полет"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Base.FlyPed.state =! cheatClient->PlayerCheat.Base.FlyPed.state;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Полет"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Base.FlyPed.state =! cheatClient->PlayerCheat.Base.FlyPed.state;
							}
						}
						ImGui::SameLine();
						if(cheatClient->PlayerCheat.Base.Control.fastRotate)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Быстрый поворот"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Base.Control.fastRotate =! cheatClient->PlayerCheat.Base.Control.fastRotate;
								uintptr_t localplayer = cheatClient->FindLocalPlayer();
								if(localplayer)
								{
									CPed localped = CPed(localplayer);
									localped.SetHeadingRate(7.5f);
								}
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Быстрый поворот"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Base.Control.fastRotate =! cheatClient->PlayerCheat.Base.Control.fastRotate;
								uintptr_t localplayer = cheatClient->FindLocalPlayer();
								if(localplayer)
								{
									CPed localped = CPed(localplayer);
									localped.SetHeadingRate(7.5f * cheatClient->PlayerCheat.Base.Control.rate);
								}
							}
						}
						
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::SameLine();
						ImGui::SameLine();
						
						if(cheatClient->PlayerCheat.Base.Jumping.long_)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Длинный прыжок"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Base.Jumping.long_ =! cheatClient->PlayerCheat.Base.Jumping.long_;
								WriteMemoryFloat(libAddr + 0x392FDC, 0.17f);
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Длинный прыжок"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Base.Jumping.long_ =! cheatClient->PlayerCheat.Base.Jumping.long_;
								WriteMemoryFloat(libAddr + 0x392FDC, 0.17f * cheatClient->PlayerCheat.Base.Jumping.long__);
							}
						}
						ImGui::SameLine();
						if(cheatClient->PlayerCheat.Base.Jumping.high)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Высокий прыжок"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Base.Jumping.high =! cheatClient->PlayerCheat.Base.Jumping.high;
								uintptr_t localplayer = cheatClient->FindLocalPlayer();
								if(localplayer)
								{
									CPed localped = CPed(localplayer);
									localped.SetHighJumpMult(70.f);
								}
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Высокий прыжок"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Base.Jumping.high =! cheatClient->PlayerCheat.Base.Jumping.high;
								uintptr_t localplayer = cheatClient->FindLocalPlayer();
								if(localplayer)
								{
									CPed localped = CPed(localplayer);
									localped.SetHighJumpMult(70.f / cheatClient->PlayerCheat.Base.Jumping.high__);
								}
							}
						}
						
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::SameLine();
						ImGui::SameLine();
						
						if(ImGui::Button(OBFUSCATE("Умереть"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							uintptr_t localplayer = cheatClient->FindLocalPlayer();
							if(localplayer)
							{
								CPed localped = CPed(localplayer);
								localped.SetHealth(0.f);
								localped.SetArmour(0.f);
							}
						}
						ImGui::SameLine();
						if(cheatClient->PlayerCheat.Base.Invulnerability.walkUnderwater)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Ходьба под водой"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Base.Invulnerability.walkUnderwater =! cheatClient->PlayerCheat.Base.Invulnerability.walkUnderwater;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Ходьба под водой"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Base.Invulnerability.walkUnderwater =! cheatClient->PlayerCheat.Base.Invulnerability.walkUnderwater;
							}
						}
						/*ImGui::SameLine();
						if(ImGui::Button("Высокий прыжок", ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
						{
							BrNotification(TYPE_TEXT_GREEN, "CarShot", 5);
						}*/
					}
					
					if(current_player_category == 1)
					{
						ImGui::SameLine();
						ImGui::SameLine();
						if(cheatClient->PlayerCheat.Rage.AimBot.state)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("АимБот"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Rage.AimBot.state =! cheatClient->PlayerCheat.Rage.AimBot.state;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("АимБот"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Rage.AimBot.state =! cheatClient->PlayerCheat.Rage.AimBot.state;
							}
						}
						
						ImGui::SameLine();
						
						if(cheatClient->PlayerCheat.Rage.NoReload.state)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Стрельба без\nперезарядки"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Rage.NoReload.state =! cheatClient->PlayerCheat.Rage.NoReload.state;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Стрельба без\nперезарядки"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Rage.NoReload.state =! cheatClient->PlayerCheat.Rage.NoReload.state;
							}
						}
						
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::SameLine();
						ImGui::SameLine();
						
						if(cheatClient->PlayerCheat.Rage.Damager.state)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Дамагер"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Rage.Damager.state =! cheatClient->PlayerCheat.Rage.Damager.state;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Дамагер"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Rage.Damager.state =! cheatClient->PlayerCheat.Rage.Damager.state;
							}
						}
						ImGui::SameLine();
						if(cheatClient->PlayerCheat.Rage.Invulnerability.autoHeal)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Авто-аптечка"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Rage.Invulnerability.autoHeal =! cheatClient->PlayerCheat.Rage.Invulnerability.autoHeal;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Авто-аптечка"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Rage.Invulnerability.autoHeal =! cheatClient->PlayerCheat.Rage.Invulnerability.autoHeal;
							}
						}
						
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::SameLine();
						ImGui::SameLine();
						
						if(cheatClient->PlayerCheat.Rage.Invulnerability.autoArmour)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Авто-броня"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Rage.Invulnerability.autoArmour =! cheatClient->PlayerCheat.Rage.Invulnerability.autoArmour;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Авто-броня"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->PlayerCheat.Rage.Invulnerability.autoArmour =! cheatClient->PlayerCheat.Rage.Invulnerability.autoArmour;
							}
						}
						ImGui::SameLine();
						if(ImGui::Button(OBFUSCATE("Заспавнить себя"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							RakNet::BitStream bsSpawn, bsRequestSpawn;
							
							int requestSpawn = 129;
							int spawn = 52;
							
							//log_3(gpeer, &spawn, &bsSpawn, 1, 10, 0, ga7, ga8, 0, 0, -1, 0xFFFF, 0xFFFF, 0);
							log_3(gpeer, &requestSpawn, &bsRequestSpawn, 1, 8, 0, ga7, ga8, 0, 0, -1, 0xFFFF, 0xFFFF, 0);
						}
						
						/*ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();
						ImGui::SameLine();
						if(ImGui::Button("Test", ImVec2(ImGui::CalcTextSize("Test").x * 1.5f, ImGui::CalcTextSize("X").y * 2.f)))
						{
							uintptr_t localped = cheatClient->FindLocalPlayer();
							if(localped)
							{
								//SetSeek(localped, *(float *)(localped + 0x34) + 15.f, *(float *)(localped + 0x38) + 10.f, *(float *)(localped + 0x3C), 0.5f);
								
								*(int *)(localped + 592) = 31;
								*(int *)(localped + 596) = 0;
								*(int *)(localped + 600) = 4;
								*(int *)(localped + 0x3EC) = 0;
								
								/**(int *)(localped + 612) = 0;
								*(int *)(localped + 616) = 0;
								*/
								//last_ms = time(0);
								/*SetMoveAnim(localped);
							}
						}
						
						uintptr_t localped = cheatClient->FindLocalPlayer();
						if(localped)
						{
							ImGui::Text("PedState: %d\nStoredState: %d\nMoveState: %d", *(int *)(localped + 592), *(int *)(localped + 596), *(int *)(localped + 600));
							ImGui::Text("bUsesCollision: %d", (*(int *)(localped + 0x50) & 0x100));
						}
						if(ImGui::Button("idle", ImVec2(ImGui::CalcTextSize("Test").x * 1.5f, ImGui::CalcTextSize("X").y * 2.f)))
						{
							uintptr_t localped = cheatClient->FindLocalPlayer();
							if(localped)
							{
								*(int *)(localped + 592) = 1;
								*(int *)(localped + 596) = 0;
								*(int *)(localped + 600) = 1;
								/**(int *)(localped + 612) = 0;
								*(int *)(localped + 616) = 0;
								//last_ms = time(0);*//*
								SetMoveAnim(localped);
							}
						}*/ 
					}
				}
				
				// Транспорт
				if(current_menu_category == 1)
				{
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					
					if(cheatClient->VehicleCheat.Invulnerability.health)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
						if(ImGui::Button(OBFUSCATE("Бессмертие"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VehicleCheat.Invulnerability.health =! cheatClient->VehicleCheat.Invulnerability.health;
							BrNotification(TYPE_TEXT_GREEN, OBFUSCATE("Бессмертие работает только от физики игры!"), 5);
						}
						ImGui::PopStyleColor();
					}
					else
					{
						if(ImGui::Button(OBFUSCATE("Бессмертие"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VehicleCheat.Invulnerability.health =! cheatClient->VehicleCheat.Invulnerability.health;
							BrNotification(TYPE_TEXT_GREEN, OBFUSCATE("Бессмертие работает только от физики игры!"), 5);
						}
					}
					ImGui::SameLine();
					if(cheatClient->VehicleCheat.FlyCar.state)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
						if(ImGui::Button(OBFUSCATE("FlyCar"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VehicleCheat.FlyCar.state =! cheatClient->VehicleCheat.FlyCar.state;
						}
						ImGui::PopStyleColor();
					}
					else
					{
						if(ImGui::Button(OBFUSCATE("FlyCar"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VehicleCheat.FlyCar.state =! cheatClient->VehicleCheat.FlyCar.state;
						}
					}
					
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SameLine();
					
					if(ImGui::Button(OBFUSCATE("Лаунч-контроль"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
					{
						uintptr_t localped = cheatClient->FindLocalPlayer();
						if(localped)
						{
							CPed localplayer = CPed(localped);
							if(localplayer.GetVehicle() != 0)
							{
								CVehicle localvehicle = CVehicle(localplayer.GetVehicle());
								if(localvehicle.GetDriver() == localplayer.GetMe())
								{
									localvehicle.SetLaunchState(1);
								}
							}
						}
					}
					ImGui::SameLine();
					if(ImGui::Button(OBFUSCATE("Гидравлика"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
					{
						uintptr_t localped = cheatClient->FindLocalPlayer();
						if(localped)
						{
							CPed localplayer = CPed(localped);
							if(localplayer.GetVehicle() != 0)
							{
								CVehicle localvehicle = CVehicle(localplayer.GetVehicle());
								if(localvehicle.GetDriver() == localplayer.GetMe())
								{
									if(localvehicle.GetHydraulic()) {
										localvehicle.SetHydraulic(false);
									} else {
										localvehicle.SetHydraulic(true);
									}
								}
							}
						}
					}
						
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SameLine();
					
					if(cheatClient->VehicleCheat.CarShot.state)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
						if(ImGui::Button(OBFUSCATE("CarShot"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VehicleCheat.CarShot.state =! cheatClient->VehicleCheat.CarShot.state;
						}
						ImGui::PopStyleColor();
					}
					else
					{
						if(ImGui::Button(OBFUSCATE("CarShot"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VehicleCheat.CarShot.state =! cheatClient->VehicleCheat.CarShot.state;
						}
					}
					ImGui::SameLine();
					if(cheatClient->VehicleCheat.Invulnerability.engine)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
						if(ImGui::Button(OBFUSCATE("Вечный двигатель"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VehicleCheat.Invulnerability.engine =! cheatClient->VehicleCheat.Invulnerability.engine;
						}
						ImGui::PopStyleColor();
					}
					else
					{
						if(ImGui::Button(OBFUSCATE("Вечный двигатель"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VehicleCheat.Invulnerability.engine =! cheatClient->VehicleCheat.Invulnerability.engine;
						}
					}
					
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SameLine();
					
					if(ImGui::Button(OBFUSCATE("Сломать т/с"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
					{
						uintptr_t localped = cheatClient->FindLocalPlayer();
						if(localped)
						{
							CPed localplayer = CPed(localped);
							if(localplayer.GetVehicle() != 0)
							{
								CVehicle localvehicle = CVehicle(localplayer.GetVehicle());
								if(localvehicle.GetDriver() == localped)
								{
									localvehicle.SetHealth(269.f);
								}
							}
							else
							{
								BrNotification(TYPE_TEXT_RED, OBFUSCATE("Вы должны находиться в транспорте"), 5);
							}
						}
					}
					ImGui::SameLine();
					if(cheatClient->Tuning.menu)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
						if(ImGui::Button(OBFUSCATE("Тюнинг т/с"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->Tuning.menu =! cheatClient->Tuning.menu;
						}
						ImGui::PopStyleColor();
					}
					else
					{
						if(ImGui::Button(OBFUSCATE("Тюнинг т/с"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->Tuning.menu =! cheatClient->Tuning.menu;
						}
					}
					
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SameLine();
					
					if(cheatClient->PlayerCheat.Base.Invulnerability.noBike)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
						if(ImGui::Button(OBFUSCATE("Не падать\nс мотоцикла"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->PlayerCheat.Base.Invulnerability.noBike =! cheatClient->PlayerCheat.Base.Invulnerability.noBike;
						}
						ImGui::PopStyleColor();
					}
					else
					{
						if(ImGui::Button(OBFUSCATE("Не падать\nс мотоцикла"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->PlayerCheat.Base.Invulnerability.noBike =! cheatClient->PlayerCheat.Base.Invulnerability.noBike;
						}
					}
					ImGui::SameLine();
					if(cheatClient->VehicleCheat.Invulnerability.driveUnderwater)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
						if(ImGui::Button(OBFUSCATE("Езда под водой"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VehicleCheat.Invulnerability.driveUnderwater =! cheatClient->VehicleCheat.Invulnerability.driveUnderwater;
						}
						ImGui::PopStyleColor();
					}
					else
					{
						if(ImGui::Button(OBFUSCATE("Езда под водой"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VehicleCheat.Invulnerability.driveUnderwater =! cheatClient->VehicleCheat.Invulnerability.driveUnderwater;
						}
					}
					
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SameLine();
					
					if(cheatClient->VehicleCheat.CarChanger.menu)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
						if(ImGui::Button(OBFUSCATE("Поменять т/с"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VehicleCheat.CarChanger.menu =! cheatClient->VehicleCheat.CarChanger.menu;
						}
						ImGui::PopStyleColor();
					}
					else
					{
						if(ImGui::Button(OBFUSCATE("Поменять т/с"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							uintptr_t localped = cheatClient->FindLocalPlayer();
							if(localped)
							{
								CPed localplayer = CPed(localped);
								if(localplayer.GetVehicle() != 0)
								{
									CVehicle localvehicle = CVehicle(localplayer.GetVehicle());
									if(localvehicle.GetDriver() == localplayer.GetMe())
									{
										cheatClient->VehicleCheat.CarChanger.menu =! cheatClient->VehicleCheat.CarChanger.menu;
									}
									else
									{
										BrNotification(TYPE_TEXT_RED, OBFUSCATE("Вы должны находиться в транспорте!"), 5);
									}
								}
								else
								{
									BrNotification(TYPE_TEXT_RED, OBFUSCATE("Вы должны находиться в транспорте!"), 5);
								}
							}
						}
					}
					ImGui::SameLine();
					if(cheatClient->VehicleCheat.AntiDetachTrailer.state)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
						if(ImGui::Button(OBFUSCATE("Анти Отцеп"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VehicleCheat.AntiDetachTrailer.state =! cheatClient->VehicleCheat.AntiDetachTrailer.state;
						}
						ImGui::PopStyleColor();
					}
					else
					{
						if(ImGui::Button(OBFUSCATE("Анти Отцеп"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							uintptr_t localped = cheatClient->FindLocalPlayer();
							if(localped)
							{
								int trailer = cheatClient->GetAtVehiclePool(999);
								if(trailer)
								{
									uintptr_t veh = CPed(localped).GetVehicle();
									if(veh)
									{
										((void (*)(int,int,int))(libAddr + string2Offset(OBFUSCATE("0x2BA95F")) + 1))(trailer, veh, 1);
									}
								}
							}
							cheatClient->VehicleCheat.AntiDetachTrailer.state =! cheatClient->VehicleCheat.AntiDetachTrailer.state;
						}
					}
					
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SameLine();
					
				}
				
				// Визуал
				if(current_menu_category == 2)
				{
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					
					if(cheatClient->Wallhack.menu || cheatClient->Wallhack.Player.lines || cheatClient->Wallhack.Player.box || cheatClient->Wallhack.Player.collision || cheatClient->Wallhack.Player.skeleton || cheatClient->Wallhack.Player.model_id || cheatClient->Wallhack.Player.nametags
					|| cheatClient->Wallhack.Vehicle.lines || cheatClient->Wallhack.Vehicle.box || cheatClient->Wallhack.Vehicle.collision || cheatClient->Wallhack.Vehicle.model_id || cheatClient->Wallhack.Vehicle.model_name
					|| cheatClient->Wallhack.Object.lines || cheatClient->Wallhack.Object.box || cheatClient->Wallhack.Object.collision || cheatClient->Wallhack.Object.model_id || cheatClient->Wallhack.Object.model_name)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
						if(ImGui::Button(OBFUSCATE("Wallhack"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->Wallhack.menu =! cheatClient->Wallhack.menu;
						}
						ImGui::PopStyleColor();
					}
					else
					{
						if(ImGui::Button(OBFUSCATE("Wallhack"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->Wallhack.menu =! cheatClient->Wallhack.menu;
						}
					}
					ImGui::SameLine();
					if(cheatClient->CarSpawn.menu)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
						if(ImGui::Button(OBFUSCATE("Создать т/с"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->CarSpawn.menu =! cheatClient->CarSpawn.menu;
						}
						ImGui::PopStyleColor();
					}
					else
					{
						if(ImGui::Button(OBFUSCATE("Создать т/с"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->CarSpawn.menu =! cheatClient->CarSpawn.menu;
						}
					}
					
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SameLine();
					
					if(cheatClient->VisualCheat.SkinChanger.menu)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
						if(ImGui::Button(OBFUSCATE("Выдать скин"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VisualCheat.SkinChanger.menu =! cheatClient->VisualCheat.SkinChanger.menu;
						}
						ImGui::PopStyleColor();
					}
					else
					{
						if(ImGui::Button(OBFUSCATE("Выдать скин"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VisualCheat.SkinChanger.menu =! cheatClient->VisualCheat.SkinChanger.menu;
						}
					}
					ImGui::SameLine();
					if(cheatClient->VisualCheat.MoneyChanger.menu)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
						if(ImGui::Button(OBFUSCATE("Выдать деньги"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VisualCheat.MoneyChanger.menu =! cheatClient->VisualCheat.MoneyChanger.menu;
						}
						ImGui::PopStyleColor();
					}
					else
					{
						if(ImGui::Button(OBFUSCATE("Выдать деньги"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							cheatClient->VisualCheat.MoneyChanger.menu =! cheatClient->VisualCheat.MoneyChanger.menu;
						}
					}
					
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SameLine();
					
					ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.8f);
					if (ImGui::CollapsingHeader(OBFUSCATE("Игровой мир"), ImGuiTreeNodeFlags_None))
        			{
						ImGui::Spacing();
						ImGui::SameLine();
						if(ImGui::SliderInt(OBFUSCATE("Время"), &cheatClient->VisualCheat.TimeChanger.time, 0, 24))
						{
							*(int *)(libAddr + 0x4B6263D) = cheatClient->VisualCheat.TimeChanger.time;
						}
						ImGui::Spacing();
						ImGui::SameLine();
						ImGui::Checkbox(OBFUSCATE("Заморозить время"), &cheatClient->VisualCheat.TimeChanger.freeze);
						ImGui::Spacing();
						ImGui::SameLine();
					}
					
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SameLine();
					
					/*ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.8f);
					if (ImGui::CollapsingHeader("Field of View (угол обзора)", ImGuiTreeNodeFlags_None))
        			{
						ImGui::Spacing();
						ImGui::SameLine();
						ImGui::SliderFloat("FOV", &cheatClient->VisualCheat.Camera.FOV, 10.f, 170.f);
						ImGui::Spacing();
						ImGui::SameLine();
						if(ImGui::Button("Сбросить FOV", ImVec2(ImGui::CalcTextSize("Сбросить FOV").x * 2.5f, ImGui::CalcTextSize("W").y * 2.f)))
						{
							cheatClient->VisualCheat.Camera.FOV = 70.f;
						}
					}*/
				}
					
				// Другое
				if(current_menu_category == 3)
				{
					ImGui::Spacing();
					ImGui::Spacing();
					
					ImVec2 currentCursorPos = ImGui::GetCursorPos();
					
					ImGui::PushStyleColor(ImGuiCol_ChildBg, rgba(17,17,17));
					ImGui::SameLine();
					ImGui::BeginChild(OBFUSCATE("##player-category"), ImVec2(680, categoryButtonHeight * 1.2f));
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2));
						ImGui::SetCursorPosY((categoryButtonHeight * 1.2f - categoryButtonHeight) / 2);
						if(current_other_category == 0)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							ImGui::Button(OBFUSCATE(u8"Основное"), ImVec2((680 / 2) - (((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2), categoryButtonHeight));
							ImGui::PopStyleColor();
						}
						else
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(17,17,17));
							if(ImGui::Button(OBFUSCATE(u8"Основное"), ImVec2((680 / 2) - (((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2), categoryButtonHeight)))
							{
								current_other_category = 0;
							}
							ImGui::PopStyleColor();
						}
						
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2) * 2) + ((680 / 2) - (((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2)));
						ImGui::SetCursorPosY((categoryButtonHeight * 1.2f - categoryButtonHeight) / 2);
						if(current_other_category == 1)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							ImGui::Button(OBFUSCATE(u8"Авто-фарм"), ImVec2((680 / 2) - ((((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2) * 2), categoryButtonHeight));
							ImGui::PopStyleColor();
						}
						else
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(17,17,17));
							if(ImGui::Button(OBFUSCATE(u8"Авто-фарм"), ImVec2((680 / 2) - ((((categoryButtonHeight * 1.2f) - categoryButtonHeight) / 2) * 2), categoryButtonHeight)))
							{
								current_other_category = 1;
							}
							ImGui::PopStyleColor();
						}
					}
					ImGui::EndChild();
					ImGui::PopStyleColor();
					
					ImGui::Spacing();
					
					if(current_other_category == 0)
					{
						ImGui::SameLine();
						ImGui::SameLine();
						
						if(cheatClient->GameSpeed.menu || cheatClient->GameSpeed.state)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Скорость игры"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->GameSpeed.menu =! cheatClient->GameSpeed.menu;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Скорость игры"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->GameSpeed.menu =! cheatClient->GameSpeed.menu;
							}
						}
						ImGui::SameLine();
						if(cheatClient->Collision.menu || cheatClient->Collision.ped || cheatClient->Collision.vehicle || cheatClient->Collision.objects)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Колизии"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->Collision.menu =! cheatClient->Collision.menu;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Колизии"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->Collision.menu =! cheatClient->Collision.menu;
							}
						}
						
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::SameLine();
						ImGui::SameLine();
						
						/*if(cheatClient->FakeChat.menu || cheatClient->FakeChat.state)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button("Fake Chat", ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
							{
								cheatClient->FakeChat.menu =! cheatClient->FakeChat.menu;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button("Fake Chat", ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
							{
								cheatClient->FakeChat.menu =! cheatClient->FakeChat.menu;
							}
						}
						ImGui::SameLine();
						if(cheatClient->Flooder.menu || cheatClient->Flooder.working)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button("Флудер", ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
							{
								cheatClient->Flooder.menu =! cheatClient->Flooder.menu;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button("Флудер", ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
							{
								cheatClient->Flooder.menu =! cheatClient->Flooder.menu;
							}
						}
						
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::SameLine();
						ImGui::SameLine();*/
						
						if(ImGui::Button(OBFUSCATE("ТП к маркеру"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							if(cheatClient->Teleport.toCheckpoint)
							{
								cheatClient->Teleport.toMarker = true;
								cheatClient->Teleport.toCheckpoint = false;
							}
							else
							{
								cheatClient->Teleport.toMarker = true;
							}
						}
						ImGui::SameLine();
						if(ImGui::Button(OBFUSCATE("ТП к чекпоинту"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
						{
							if(cheatClient->Teleport.toMarker)
							{
								cheatClient->Teleport.toMarker = false;
								cheatClient->Teleport.OnFoot.speed = 0;
								cheatClient->Teleport.OnFoot.speed_max = 50.f;
								cheatClient->Teleport.toCheckpoint = true;
							}
							else
							{
								cheatClient->Teleport.toCheckpoint = true;
							}
						}
						
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::SameLine();
						ImGui::SameLine();
						
						if(cheatClient->Teleport.toPeds)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("ТП к ближ.\n   игрокам"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->Teleport.toPeds =! cheatClient->Teleport.toPeds;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("ТП к ближ.\n   игрокам"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->Teleport.toPeds =! cheatClient->Teleport.toPeds;
							}
						}
						ImGui::SameLine();
						if(cheatClient->Teleport.toCars)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("ТП к ближ. т/с"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->Teleport.toCars =! cheatClient->Teleport.toCars;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("ТП к ближ. т/с"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->Teleport.toCars =! cheatClient->Teleport.toCars;
							}
						}
						
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::SameLine();
						ImGui::SameLine();
						
					}
					
					if(current_other_category == 1)
					{
						ImGui::SameLine();
						ImGui::SameLine();
						if(cheatClient->AutoFarm.BotMiner.menu)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Бот Шахта"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->AutoFarm.BotMiner.menu =! cheatClient->AutoFarm.BotMiner.menu;
								uintptr_t localped = cheatClient->FindLocalPlayer();
								if(localped)
								{
									// Сброс анимации
									*(int *)(localped + 592) = 1;
									*(int *)(localped + 596) = 0;
									*(int *)(localped + 600) = 1;
									*(int *)(localped + 612) = 0;
									*(int *)(localped + 616) = 0;
									SetMoveAnim(localped);
								}
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Бот Шахта"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->AutoFarm.BotMiner.menu =! cheatClient->AutoFarm.BotMiner.menu;
							}
						}
						ImGui::SameLine();
						if(cheatClient->AutoFarm.BotFactory.menu)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Бот Завод"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->AutoFarm.BotFactory.menu =! cheatClient->AutoFarm.BotFactory.menu;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Бот Завод"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->AutoFarm.BotFactory.menu =! cheatClient->AutoFarm.BotFactory.menu;
							}
						}
						
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::SameLine();
						ImGui::SameLine();
						
						/*if(cheatClient->AutoFarm.BotFarm.menu)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button("Бот Ферма", ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
							{
								cheatClient->AutoFarm.BotFarm.menu =! cheatClient->AutoFarm.BotFarm.menu;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button("Бот Ферма", ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
							{
								cheatClient->AutoFarm.BotFarm.menu =! cheatClient->AutoFarm.BotFarm.menu;
							}
						}
						ImGui::SameLine();
						if(cheatClient->AutoFarm.BotCourier.menu)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button("Бот Курьер", ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
							{
								cheatClient->AutoFarm.BotCourier.menu =! cheatClient->AutoFarm.BotCourier.menu;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button("Бот Курьер", ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
							{
								cheatClient->AutoFarm.BotCourier.menu =! cheatClient->AutoFarm.BotCourier.menu;
							}
						}
						
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::SameLine();
						ImGui::SameLine();
						
						if(cheatClient->AutoFarm.BotBus.menu)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button("Бот Автобус", ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
							{
								cheatClient->AutoFarm.BotBus.menu =! cheatClient->AutoFarm.BotBus.menu;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button("Бот Автобус", ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
							{
								cheatClient->AutoFarm.BotBus.menu =! cheatClient->AutoFarm.BotBus.menu;
							}
						}
						ImGui::SameLine();
						if(cheatClient->AutoFarm.BotTrucker.menu)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button("Бот Дальнобойщик", ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
							{
								cheatClient->AutoFarm.BotTrucker.menu =! cheatClient->AutoFarm.BotTrucker.menu;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button("Бот Дальнобойщик", ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize("X").y * 3.f)))
							{
								cheatClient->AutoFarm.BotTrucker.menu =! cheatClient->AutoFarm.BotTrucker.menu;
							}
						}
						
						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::SameLine();
						ImGui::SameLine();*/
						
						if(cheatClient->AutoFarm.AntiAfk.state)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
							if(ImGui::Button(OBFUSCATE("Анти АФК"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								cheatClient->AutoFarm.AntiAfk.state =! cheatClient->AutoFarm.AntiAfk.state;
							}
							ImGui::PopStyleColor();
						}
						else
						{
							if(ImGui::Button(OBFUSCATE("Анти АФК"), ImVec2(ImGui::GetWindowWidth() * 0.425f, ImGui::CalcTextSize(OBFUSCATE("X")).y * 3.f)))
							{
								uintptr_t localped = cheatClient->FindLocalPlayer();
								*(int *)(localped + 592) = 5;
								*(int *)(localped + 596) = 0;
								*(int *)(localped + 600) = 4;
								*(int *)(localped + 612) = 0;
								*(int *)(localped + 616) = 0;
								SetMoveAnim(localped);
								cheatClient->AutoFarm.AntiAfk.state =! cheatClient->AutoFarm.AntiAfk.state;
							}
						}
					}
				}
			}
			ImGui::EndChild();
			ImGui::PopStyleVar();
		}
		ImGui::End();
	}
	
	if(cheatClient->GameSpeed.menu)
	{
		ImGui::GetStyle().WindowTitleAlign.y = 0.5f;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 15.f));
		if(!game_speed_inited)
		{
			ImGui::SetNextWindowSize(ImVec2(350, 250));
			game_speed_inited = true;
		}
		ImGui::Begin(OBFUSCATE("Скорость игры"), 0);
		{
			ImGui::SliderFloat(OBFUSCATE("##gamespeed"), &cheatClient->GameSpeed.speed, 1.f, 5.f);
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}
	
	if(cheatClient->VehicleCheat.CarChanger.menu)
	{
		uintptr_t localped = cheatClient->FindLocalPlayer();
		if(localped)
		{
			CPed localplayer = CPed(localped);
			if(localplayer.GetVehicle())
			{
				CVehicle localvehicle = CVehicle(localplayer.GetVehicle());
				if(localvehicle.GetDriver() ==  localplayer.GetMe())
				{
					ImGui::GetStyle().WindowTitleAlign.y = 0.5f;
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 15.f));
					ImGui::Begin(OBFUSCATE("Подмена т/с"), 0);
					{
						ImGui::BeginChild(OBFUSCATE("##changecarnumpadinput"), ImVec2(((ImGui::CalcTextSize(OBFUSCATE("1")).x * 7.f) * 3.f), ImGui::CalcTextSize(OBFUSCATE("1")).y * 2.5f));
						{
							ImGui::Spacing();
							ImGui::Spacing();
							ImGui::Spacing();
							ImGui::SameLine();
							ImGui::Text("%s", cheatClient->VehicleCheat.CarChanger.numpad.c_str());
						}
						ImGui::EndChild();
				
						ImGui::Spacing();
				
						if(ImGui::Button(OBFUSCATE("1"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
						{
							if(cheatClient->VehicleCheat.CarChanger.numpad.size() < 5)
							{
								cheatClient->VehicleCheat.CarChanger.numpad.append("1");
							}
						}
						ImGui::SameLine();
						if(ImGui::Button(OBFUSCATE("2"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
						{
							if(cheatClient->VehicleCheat.CarChanger.numpad.size() < 5)
							{
								cheatClient->VehicleCheat.CarChanger.numpad.append("2");
							}
						}
						ImGui::SameLine();
						if(ImGui::Button(OBFUSCATE("3"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
						{
							if(cheatClient->VehicleCheat.CarChanger.numpad.size() < 5)
							{
								cheatClient->VehicleCheat.CarChanger.numpad.append("3");
							}
						}
						ImGui::Spacing();
						if(ImGui::Button(OBFUSCATE("4"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
						{
							if(cheatClient->VehicleCheat.CarChanger.numpad.size() < 5)
							{
								cheatClient->VehicleCheat.CarChanger.numpad.append("4");
							}
						}
						ImGui::SameLine();
						if(ImGui::Button(OBFUSCATE("5"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
						{
							if(cheatClient->VehicleCheat.CarChanger.numpad.size() < 5)
							{
								cheatClient->VehicleCheat.CarChanger.numpad.append("5");
							}
						}
						ImGui::SameLine();
						if(ImGui::Button(OBFUSCATE("6"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
						{
							if(cheatClient->VehicleCheat.CarChanger.numpad.size() < 5)
							{
								cheatClient->VehicleCheat.CarChanger.numpad.append("6");
							}
						}
						ImGui::Spacing();
						if(ImGui::Button(OBFUSCATE("7"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
						{
							if(cheatClient->VehicleCheat.CarChanger.numpad.size() < 5)
							{
								cheatClient->VehicleCheat.CarChanger.numpad.append("7");
							}
						}
						ImGui::SameLine();
						if(ImGui::Button(OBFUSCATE("8"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
						{
							if(cheatClient->VehicleCheat.CarChanger.numpad.size() < 5)
							{
								cheatClient->VehicleCheat.CarChanger.numpad.append("8");
							}
						}
						ImGui::SameLine();
						if(ImGui::Button(OBFUSCATE("9"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
						{
							if(cheatClient->VehicleCheat.CarChanger.numpad.size() < 5)
							{
								cheatClient->VehicleCheat.CarChanger.numpad.append("9");
							}
						}
						ImGui::Spacing();
						if(ImGui::Button(OBFUSCATE("<<"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
						{
							if(cheatClient->VehicleCheat.CarChanger.numpad.size() > 0)
							{
								cheatClient->VehicleCheat.CarChanger.numpad.erase(cheatClient->VehicleCheat.CarChanger.numpad.size() - 1);
							}
						}
						ImGui::SameLine();
						if(ImGui::Button(OBFUSCATE("0"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
						{
							if(cheatClient->VehicleCheat.CarChanger.numpad.size() < 5 && cheatClient->VehicleCheat.CarChanger.numpad.size() > 0)
							{
								cheatClient->VehicleCheat.CarChanger.numpad.append("0");
							}
						}
						ImGui::SameLine();
						if(ImGui::Button(OBFUSCATE("OK"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
						{
							cheatClient->VehicleCheat.CarChanger.entered = true;
							cheatClient->VehicleCheat.CarChanger.menu = false;
						}
					}
					ImGui::End();
					ImGui::PopStyleVar();
				}				
			}
		}
	}
	
	if(cheatClient->CarSpawn.menu)
	{
		uintptr_t localped = cheatClient->FindLocalPlayer();
		if(localped)
		{
			ImGui::GetStyle().WindowTitleAlign.y = 0.5f;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 15.f));
			ImGui::Begin(OBFUSCATE("Создание т/с"), 0);
			{
				ImGui::BeginChild(OBFUSCATE("##createcarnumpadinput"), ImVec2(((ImGui::CalcTextSize("1").x * 7.f) * 3.f), ImGui::CalcTextSize("1").y * 2.5f));
				{
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::Text("%s", cheatClient->CarSpawn.numpad.c_str());
				}
				ImGui::EndChild();
				
				ImGui::Spacing();
				
				if(ImGui::Button(OBFUSCATE("1"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->CarSpawn.numpad.size() < 5)
					{
						cheatClient->CarSpawn.numpad.append("1");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("2"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->CarSpawn.numpad.size() < 5)
					{
						cheatClient->CarSpawn.numpad.append("2");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("3"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->CarSpawn.numpad.size() < 5)
					{
						cheatClient->CarSpawn.numpad.append("3");
					}
				}
				ImGui::Spacing();
				if(ImGui::Button(OBFUSCATE("4"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->CarSpawn.numpad.size() < 5)
					{
						cheatClient->CarSpawn.numpad.append("4");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("5"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->CarSpawn.numpad.size() < 5)
					{
						cheatClient->CarSpawn.numpad.append("5");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("6"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->CarSpawn.numpad.size() < 5)
					{
						cheatClient->CarSpawn.numpad.append("6");
					}
				}
				ImGui::Spacing();
				if(ImGui::Button(OBFUSCATE("7"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->CarSpawn.numpad.size() < 5)
					{
						cheatClient->CarSpawn.numpad.append("7");
					}
				}
				ImGui::SameLine();
					
				if(ImGui::Button(OBFUSCATE("8"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->CarSpawn.numpad.size() < 5)
					{
						cheatClient->CarSpawn.numpad.append("8");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("9"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->CarSpawn.numpad.size() < 5)
					{
						cheatClient->CarSpawn.numpad.append("9");
					}
				}
				ImGui::Spacing();
				if(ImGui::Button(OBFUSCATE("<<"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->CarSpawn.numpad.size() > 0)
					{
						cheatClient->CarSpawn.numpad.erase(cheatClient->CarSpawn.numpad.size() - 1);
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("0"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->CarSpawn.numpad.size() < 5 && cheatClient->CarSpawn.numpad.size() > 0)
					{
						cheatClient->CarSpawn.numpad.append("0");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("OK"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					cheatClient->CarSpawn.entered = true;
					cheatClient->CarSpawn.menu = false;
				}
			}
			ImGui::End();
			ImGui::PopStyleVar();
		}				
	}
	
	if(cheatClient->VisualCheat.SkinChanger.menu)
	{
		uintptr_t localped = cheatClient->FindLocalPlayer();
		if(localped)
		{
			ImGui::GetStyle().WindowTitleAlign.y = 0.5f;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 15.f));
			ImGui::Begin(OBFUSCATE("Выдача скина"), 0);
			{
				ImGui::BeginChild(OBFUSCATE("##giveskinnumpadinput"), ImVec2(((ImGui::CalcTextSize("1").x * 7.f) * 3.f), ImGui::CalcTextSize("1").y * 2.5f));
				{
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::Text("%s", cheatClient->VisualCheat.SkinChanger.numpad.c_str());
				}
				ImGui::EndChild();
				
				ImGui::Spacing();
				
				if(ImGui::Button(OBFUSCATE("1"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.SkinChanger.numpad.size() < 5)
					{
						cheatClient->VisualCheat.SkinChanger.numpad.append("1");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("2"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.SkinChanger.numpad.size() < 5)
					{
						cheatClient->VisualCheat.SkinChanger.numpad.append("2");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("3"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.SkinChanger.numpad.size() < 5)
					{
						cheatClient->VisualCheat.SkinChanger.numpad.append("3");
					}
				}
				ImGui::Spacing();
				if(ImGui::Button(OBFUSCATE("4"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.SkinChanger.numpad.size() < 5)
					{
						cheatClient->VisualCheat.SkinChanger.numpad.append("4");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("5"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.SkinChanger.numpad.size() < 5)
					{
						cheatClient->VisualCheat.SkinChanger.numpad.append("5");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("6"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.SkinChanger.numpad.size() < 5)
					{
						cheatClient->VisualCheat.SkinChanger.numpad.append("6");
					}
				}
				ImGui::Spacing();
				if(ImGui::Button(OBFUSCATE("7"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.SkinChanger.numpad.size() < 5)
					{
						cheatClient->VisualCheat.SkinChanger.numpad.append("7");
					}
				}
				ImGui::SameLine();
					
				if(ImGui::Button(OBFUSCATE("8"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.SkinChanger.numpad.size() < 5)
					{
						cheatClient->VisualCheat.SkinChanger.numpad.append("8");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("9"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.SkinChanger.numpad.size() < 5)
					{
						cheatClient->VisualCheat.SkinChanger.numpad.append("9");
					}
				}
				ImGui::Spacing();
				if(ImGui::Button(OBFUSCATE("<<"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.SkinChanger.numpad.size() > 0)
					{
						cheatClient->VisualCheat.SkinChanger.numpad.erase(cheatClient->VisualCheat.SkinChanger.numpad.size() - 1);
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("0"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.SkinChanger.numpad.size() < 5 && cheatClient->VisualCheat.SkinChanger.numpad.size() > 0)
					{
						cheatClient->VisualCheat.SkinChanger.numpad.append("0");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("OK"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					cheatClient->VisualCheat.SkinChanger.entered = true;
					cheatClient->VisualCheat.SkinChanger.menu = false;
				}
			}
			ImGui::End();
			ImGui::PopStyleVar();
		}				
	}
	
	if(cheatClient->VisualCheat.MoneyChanger.menu)
	{
		uintptr_t localped = cheatClient->FindLocalPlayer();
		if(localped)
		{
			ImGui::GetStyle().WindowTitleAlign.y = 0.5f;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 15.f));
			ImGui::Begin(OBFUSCATE("Выдача денег"), 0);
			{
				ImGui::BeginChild(OBFUSCATE("##givemoneynumpadinput"), ImVec2(((ImGui::CalcTextSize("1").x * 7.f) * 3.f), ImGui::CalcTextSize("1").y * 2.5f));
				{
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::Text("%s", cheatClient->VisualCheat.MoneyChanger.numpad.c_str());
				}
				ImGui::EndChild();
				
				ImGui::Spacing();
				
				if(ImGui::Button(OBFUSCATE("1"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.MoneyChanger.numpad.size() < 10)
					{
						cheatClient->VisualCheat.MoneyChanger.numpad.append("1");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("2"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.MoneyChanger.numpad.size() < 10)
					{
						cheatClient->VisualCheat.MoneyChanger.numpad.append("2");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("3"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.MoneyChanger.numpad.size() < 10)
					{
						cheatClient->VisualCheat.MoneyChanger.numpad.append("3");
					}
				}
				ImGui::Spacing();
				if(ImGui::Button(OBFUSCATE("4"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.MoneyChanger.numpad.size() < 10)
					{
						cheatClient->VisualCheat.MoneyChanger.numpad.append("4");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("5"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.MoneyChanger.numpad.size() < 10)
					{
						cheatClient->VisualCheat.MoneyChanger.numpad.append("5");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("6"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.MoneyChanger.numpad.size() < 10)
					{
						cheatClient->VisualCheat.MoneyChanger.numpad.append("6");
					}
				}
				ImGui::Spacing();
				if(ImGui::Button(OBFUSCATE("7"), ImVec2(ImGui::CalcTextSize("1").x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.MoneyChanger.numpad.size() < 10)
					{
						cheatClient->VisualCheat.MoneyChanger.numpad.append("7");
					}
				}
				ImGui::SameLine();
					
				if(ImGui::Button(OBFUSCATE("8"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.MoneyChanger.numpad.size() < 10)
					{
						cheatClient->VisualCheat.MoneyChanger.numpad.append("8");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("9"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.MoneyChanger.numpad.size() < 10)
					{
						cheatClient->VisualCheat.MoneyChanger.numpad.append("9");
					}
				}
				ImGui::Spacing();
				if(ImGui::Button(OBFUSCATE("<<"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.MoneyChanger.numpad.size() > 0)
					{
						cheatClient->VisualCheat.MoneyChanger.numpad.erase(cheatClient->VisualCheat.MoneyChanger.numpad.size() - 1);
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("0"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					if(cheatClient->VisualCheat.MoneyChanger.numpad.size() < 10 && cheatClient->VisualCheat.MoneyChanger.numpad.size() > 0)
					{
						cheatClient->VisualCheat.MoneyChanger.numpad.append("0");
					}
				}
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("OK"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("1")).x * 6.f, ImGui::CalcTextSize(OBFUSCATE("1")).y * 3.f)))
				{
					cheatClient->VisualCheat.MoneyChanger.entered = true;
					cheatClient->VisualCheat.MoneyChanger.menu = false;
				}
			}
			ImGui::End();
			ImGui::PopStyleVar();
		}				
	}
	
	if(cheatClient->Teleport.toPeds)
	{
		ImGui::GetStyle().WindowTitleAlign.y = 0.5f;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 15.f));
		if(!tp_toPeds_inited)
		{
			ImGui::SetNextWindowSize(ImVec2(1000, 400));
			tp_toPeds_inited = true;
		}
		ImGui::PushStyleColor(ImGuiCol_WindowBg, rgba(17,17,17,175));
		ImGui::Begin(OBFUSCATE("ТП к ближайшим игрокам"), 0);
		{
			for(int i = 0; i <= 1500; i++)
			{
				int player = cheatClient->GetAtPlayerPool(i);
				if(player)
				{
					player = *(int *)(player + 4);
					if(player)
					{
						const char* nickname = cheatClient->ServerPlayers[i].nickname;
						int id = i;
						
						for(int a = 0; a < sizeof(cheatClient->ServerPlayers); a++)
						{
							if(cheatClient->ServerPlayers[a].id == i)
							{
								nickname = cheatClient->ServerPlayers[a].nickname;
								id = cheatClient->ServerPlayers[a].id;
								break;
							}
						}
						
						uintptr_t localped = cheatClient->FindLocalPlayer();
						if(localped)
						{
							Vec3 localcoors = cheatClient->GetPos(localped);
							Vec3 targetcoors = cheatClient->GetPos(player);
							
							float dist = Get3dDistance(*(float *)(localped + 0x34), *(float *)(localped + 0x38), *(float *)(localped + 0x3C), *(float *)(player + 0x34), *(float *)(player + 0x38), *(float *)(player + 0x3C));
							
							ImGui::Text(OBFUSCATE("%s [%d] - Дистанция: %.2f"), nickname, id, dist);
							ImGui::SameLine();
							float offset = ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x - ImGui::GetStyle().ItemInnerSpacing.x - ImGui::CalcTextSize("Телепортация").x;
							ImGui::Dummy(ImVec2(offset, 0));
							ImGui::SameLine();
							ImGui::PushID(i);
							if (ImGui::Button(OBFUSCATE("Телепортация")))
							{
    							// Телепорт
								cheatClient->SetPos(localped, targetcoors);
							}
							ImGui::PopID();
						}
					}
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}
	
	if(cheatClient->Teleport.toCars)
	{
		ImGui::GetStyle().WindowTitleAlign.y = 0.5f;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 15.f));
		if(!tp_toPeds_inited)
		{
			ImGui::SetNextWindowSize(ImVec2(900, 400));
			tp_toPeds_inited = true;
		}
		ImGui::PushStyleColor(ImGuiCol_WindowBg, rgba(17,17,17,175));
		ImGui::Begin(OBFUSCATE("ТП к ближайшему транспорту"), 0);
		{
			for(int i = 0; i < 1500; i++)
			{
				int vehicle = cheatClient->GetAtVehiclePool(i);
				if(vehicle)
				{
					unsigned short car_model_id = *(unsigned short *)(vehicle + 0x5A);
					
					CVehicle veh(vehicle);
					uintptr_t driver = veh.GetDriver();
					
					Vec3 local_coors = cheatClient->GetPos(cheatClient->FindLocalPlayer());
					Vec3 car_coors = cheatClient->GetPos(vehicle);
					float dist = Get2dDistance(local_coors, car_coors);
					
					if(driver)
					{
						int driver_id = NULL;
						for(int i = 0; i < 1500; i++)
						{
							uintptr_t player = cheatClient->GetAtPlayerPool(i);
							if(player)
							{
								player = *(int *)(player + 4);
								if(player)
								{
									if(player == driver)
									{
										driver_id = i;
										break;
									}
								}
							}
						}
						
						char buff[64];
						sprintf(buff, OBFUSCATE("Водитель - %s [%d], ID модели: %d, Дистанция: %.2f"), cheatClient->ServerPlayers[driver_id].nickname, driver_id, car_model_id, dist);
						ImGui::Text(buff);
						ImGui::SameLine();
						float offset = ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x - ImGui::GetStyle().ItemInnerSpacing.x - ImGui::CalcTextSize("Телепортация").x;
						ImGui::Dummy(ImVec2(offset, 0));
						ImGui::SameLine();
						ImGui::PushID(i);
						if (ImGui::Button(OBFUSCATE("Телепортация")))
						{
    						// Телепорт
							cheatClient->SetPos(cheatClient->FindLocalPlayer(), car_coors);
						}
						ImGui::PopID();
					}
					else
					{
						char buff[64];
						sprintf(buff, OBFUSCATE("Водитель - NULL, ID модели: %d, Дистанция: %.2f"), car_model_id, dist);
						ImGui::Text(buff);
						ImGui::SameLine();
						float offset = ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x - ImGui::GetStyle().ItemInnerSpacing.x - ImGui::CalcTextSize("Телепортация").x;
						ImGui::Dummy(ImVec2(offset, 0));
						ImGui::SameLine();
						ImGui::PushID(i);
						if (ImGui::Button(OBFUSCATE("Телепортация")))
						{
    						// Телепорт
							cheatClient->SetPos(cheatClient->FindLocalPlayer(), car_coors);
						}
						ImGui::PopID();
					}
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}
	
	if(cheatClient->Flooder.menu)
	{
		ImGui::GetStyle().WindowTitleAlign.y = 0.5f;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 15.f));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, rgba(17,17,17,225));
		ImGui::Begin(OBFUSCATE("Флудер"), 0);
		{
			ImGui::Text(OBFUSCATE(u8"Текст: %s"), cheatClient->Flooder.message.c_str());
			ImGui::SameLine();
			float offset = ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x - ImGui::GetStyle().ItemInnerSpacing.x - ImGui::CalcTextSize("Телепортация").x;
			ImGui::Dummy(ImVec2(offset, 0));
			ImGui::SameLine();
			ImGui::PushID(71817);
			if (ImGui::Button(OBFUSCATE("Изменить")))
			{
				cheatClient->Flooder.wait_text_input = true;
				cheatClient->Flooder.menu = false;
				BrNotification(TYPE_TEXT_GREEN, OBFUSCATE("Введите сообщение для флуда в чат"), 10);
			}
			
			ImGui::Text(OBFUSCATE("Задержка: %d мс"), cheatClient->Flooder.delay_after_message);
			ImGui::SameLine();
			offset = ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x - ImGui::GetStyle().ItemInnerSpacing.x - ImGui::CalcTextSize("Телепортация").x;
			ImGui::Dummy(ImVec2(offset, 0));
			ImGui::SameLine();
			ImGui::PushID(7187);
			if (ImGui::Button(OBFUSCATE("Изменить")))
			{
    			// Телепорт
				cheatClient->Flooder.wait_delay_input = true;
				cheatClient->Flooder.menu = false;
				BrNotification(TYPE_TEXT_GREEN, OBFUSCATE("Введите в чат желаемую задержку в миллисекундах.\n1000 миллисекунд равны 1 секунде!"), 10);
			}
			
			if(cheatClient->Flooder.working)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, rgba(255,55,55));
				if(ImGui::Button(OBFUSCATE("Стоп"), ImVec2(ImGui::GetWindowWidth() * 0.9f, ImGui::CalcTextSize("i").y * 3.5f)))
				{
					cheatClient->Flooder.working = false;
				}
				ImGui::PopStyleColor();
			}
			else
			{
				if(ImGui::Button(OBFUSCATE("Старт"), ImVec2(ImGui::GetWindowWidth() * 0.9f, ImGui::CalcTextSize("i").y * 3.5f)))
				{
					cheatClient->Flooder.working = true;
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}
	
	if(cheatClient->Tuning.menu)
	{
		ImGui::GetStyle().WindowTitleAlign.y = 0.5f;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 15.f));
		ImGui::Begin(OBFUSCATE("Тюнинг меню"), 0);
		{
			ImGui::BeginChild(OBFUSCATE("##tuning-menu"), ImVec2(1200, ImGui::CalcTextSize("X").y * 21.f));
			{
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
						
				ImGui::SameLine();
				if (ImGui::CollapsingHeader(OBFUSCATE("Настройка цвета"), ImGuiTreeNodeFlags_None))
        		{
					ImGui::SetNextItemWidth(500);
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y));
					ImGui::ColorEdit3(OBFUSCATE("Кузов"), cheatClient->Tuning.body_color);
					ImGui::PopStyleVar();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y));
					ImGui::SetNextItemWidth(500);
					ImGui::ColorEdit3(OBFUSCATE("Диски"), cheatClient->Tuning.wheels_color);
					ImGui::PopStyleVar();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y));
					ImGui::SetNextItemWidth(500);
					ImGui::ColorEdit3(OBFUSCATE("Фары"), cheatClient->Tuning.lights_color);
					ImGui::PopStyleVar();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y));
					ImGui::SetNextItemWidth(500);
					ImGui::ColorEdit3(OBFUSCATE("Свечение под авто"), cheatClient->Tuning.glow_color);
					ImGui::PopStyleVar();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y));
					ImGui::SetNextItemWidth(500);
					ImGui::ColorEdit3(OBFUSCATE("Свечение под авто (справа)"), cheatClient->Tuning.right_glow_color);
					ImGui::PopStyleVar();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y));
					ImGui::SetNextItemWidth(500);
					ImGui::ColorEdit3(OBFUSCATE("Свечение под авто (слева)"), cheatClient->Tuning.left_glow_color);
					ImGui::PopStyleVar();
				}
				ImGui::Spacing();
				ImGui::SameLine();
				if (ImGui::CollapsingHeader(OBFUSCATE("Настройка фар"), ImGuiTreeNodeFlags_None))
        		{
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::Checkbox(OBFUSCATE("Страбоскопы"), &cheatClient->Tuning.stroboscopes);
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SetNextItemWidth(500);
					ImGui::SliderInt(OBFUSCATE("Тип страбоскопов (0 - отсутствуют)"), &cheatClient->Tuning.stroboscopesType, 0, 4);
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::Checkbox(OBFUSCATE("Дальний свет"), &cheatClient->Tuning.longLight);
					ImGui::Spacing();
				}
				ImGui::Spacing();
				ImGui::SameLine();
				if (ImGui::CollapsingHeader(OBFUSCATE("Настройка подвески"), ImGuiTreeNodeFlags_None))
        		{
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::Checkbox(OBFUSCATE("Общий клиренс"), &cheatClient->Tuning.totalClearance);
					ImGui::SameLine();
					ImGui::Checkbox(OBFUSCATE("Отдельный клиренс"), &cheatClient->Tuning.separateClearance);
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SetNextItemWidth(700);
					ImGui::SliderInt(OBFUSCATE("Общий клиренс##1"), &cheatClient->Tuning.total_clearance_val, 0, 100);
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SetNextItemWidth(700);
					ImGui::SliderInt(OBFUSCATE("Отдельный клиренс##1"), &cheatClient->Tuning.separate_clearance_val, 0, 100);
					ImGui::Spacing();
				}
				ImGui::Spacing();
				ImGui::SameLine();
				if (ImGui::CollapsingHeader(OBFUSCATE("Настройка колес"), ImGuiTreeNodeFlags_None))
        		{
					ImGui::SetNextItemWidth(500);
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SliderInt(OBFUSCATE("Радиус колес"), &cheatClient->Tuning.wheels_radius, 0, 100);
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SetNextItemWidth(500);
					ImGui::SliderInt(OBFUSCATE("Вылет передних колес"), &cheatClient->Tuning.departure_front_wheels, 0, 100);
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SetNextItemWidth(500);
					ImGui::SliderInt(OBFUSCATE("Вылет задних колес"), &cheatClient->Tuning.departure_back_wheels, 0, 100);
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SetNextItemWidth(500);
					ImGui::SliderInt(OBFUSCATE("Развал передних колес"), &cheatClient->Tuning.collapse_front_wheels, 0, 100);
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SetNextItemWidth(500);
					ImGui::SliderInt(OBFUSCATE("Развал задних колес"), &cheatClient->Tuning.collapse_back_wheels, 0, 100);
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SetNextItemWidth(500);
					ImGui::SliderInt(OBFUSCATE("Ширина передних колес"), &cheatClient->Tuning.width_front_wheels, 0, 100);
					ImGui::Spacing();
					ImGui::SameLine();
					ImGui::SetNextItemWidth(500);
					ImGui::SliderInt(OBFUSCATE("Ширина задних колес"), &cheatClient->Tuning.width_back_wheels, 0, 100);
					ImGui::Separator();
				}
				ImGui::Spacing();
				ImGui::SameLine();
				if(ImGui::Button(OBFUSCATE("Применить"), ImVec2(ImGui::CalcTextSize(OBFUSCATE("Применить")).x * 2.5f, ImGui::CalcTextSize(OBFUSCATE("I")).y * 2.f)))
				{
					uintptr_t localped = cheatClient->FindLocalPlayer();
					if(localped)
					{
						CPed localplayer = CPed(localped);
						if(localplayer.InVehicle())
						{
							uintptr_t vehicle = localplayer.GetVehicle();
							if(vehicle)
							{
								// цвет кузова
								*(char *)(vehicle + 0x7DE) = cheatClient->Tuning.body_color[0];
								*(char *)(vehicle + 0x7DD) = cheatClient->Tuning.body_color[1];
								*(char *)(vehicle + 0x7DC) = cheatClient->Tuning.body_color[2];
								// цвет дисков
								*(char *)(vehicle + 0x7E2) = cheatClient->Tuning.wheels_color[0];
								*(char *)(vehicle + 0x7E1) = cheatClient->Tuning.wheels_color[1];
								*(char *)(vehicle + 0x7E0) = cheatClient->Tuning.wheels_color[2];
								// цвет фар
								*(char *)(vehicle + 0x7B2) = cheatClient->Tuning.lights_color[0];
								*(char *)(vehicle + 0x7B1) = cheatClient->Tuning.lights_color[1];
								*(char *)(vehicle + 0x7B0) = cheatClient->Tuning.lights_color[2];
								// цвет свечения под авто
								*(char *)(vehicle + 0x7EE) = cheatClient->Tuning.glow_color[0];
								*(char *)(vehicle + 0x7ED) = cheatClient->Tuning.glow_color[1];
								*(char *)(vehicle + 0x7EC) = cheatClient->Tuning.glow_color[2];
								// справа
								*(char *)(vehicle + 0x7F2) = cheatClient->Tuning.right_glow_color[0];
								*(char *)(vehicle + 0x7F1) = cheatClient->Tuning.right_glow_color[1];
								*(char *)(vehicle + 0x7F0) = cheatClient->Tuning.right_glow_color[2];
								// слева
								*(char *)(vehicle + 0x7F6) = cheatClient->Tuning.left_glow_color[0];
								*(char *)(vehicle + 0x7F5) = cheatClient->Tuning.left_glow_color[1];
								*(char *)(vehicle + 0x7F4) = cheatClient->Tuning.left_glow_color[2];
								
								// фары
								*(int *)(vehicle + 0x7FC) = cheatClient->Tuning.stroboscopesType;
								*(bool *)(vehicle + 0x7F8) = cheatClient->Tuning.stroboscopes;
								*(bool *)(vehicle + 0x2BF) = cheatClient->Tuning.longLight;
								
								// подвеска и колеса
								
								*(float *)(vehicle + 0x77C) = 0.2f + (0.018f * cheatClient->Tuning.total_clearance_val);
								*(float *)(vehicle + 0x780) = 0.1f + (0.008f * cheatClient->Tuning.separate_clearance_val);
								
								*(float *)(vehicle + 0x7CC) = 0.7f + (0.004f * cheatClient->Tuning.wheels_radius);
								
								*(float *)(vehicle + 0x7BC) = -0.125f + (0.0025f * cheatClient->Tuning.departure_front_wheels);
								*(float *)(vehicle + 0x7C0) = -0.125f + (0.0025f * cheatClient->Tuning.departure_back_wheels);
								
								*(float *)(vehicle + 0x7B4) = -0.5f + (0.005f * cheatClient->Tuning.collapse_front_wheels);
								*(float *)(vehicle + 0x7B8) = -0.5f + (0.005f * cheatClient->Tuning.collapse_back_wheels);
								
								*(float *)(vehicle + 0x7C4) = 0.5f + (0.0125f * cheatClient->Tuning.width_front_wheels);
								*(float *)(vehicle + 0x7C8) = 0.5f + (0.0125f * cheatClient->Tuning.width_back_wheels);
								
							}
						}
					}
				}
				ImGui::Spacing();
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}
	
	if(cheatClient->PlayerCheat.Rage.AimBot.state)
	{
		ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(cheatClient->screen_width * crosshair_offset_x, cheatClient->screen_height * crosshair_offset_y), 3.f, IM_COL32(255, 0, 0, 255), 32);
	}
	
	if(cheatClient->Wallhack.menu)
	{
		ImGui::GetStyle().WindowTitleAlign.y = 0.5f;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 15.f));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, rgba(17,17,17,200));
		ImGui::Begin(OBFUSCATE("Wallhack"), 0);
		{
			if (ImGui::BeginTabBar(OBFUSCATE("##wallhacktabs"))) {
    			// Создание первой вкладки
    			if (ImGui::BeginTabItem(OBFUSCATE("Игроки"))) {
        			// Код для содержимого первой вкладки
        			ImGui::Spacing();
					ImGui::SameLine();
					
					ImGui::Checkbox(OBFUSCATE("Линии"), &cheatClient->Wallhack.Player.lines);
					
					ImGui::SameLine();
					ImGui::SameLine();
					
					ImGui::Checkbox(OBFUSCATE("Коробка"), &cheatClient->Wallhack.Player.box);
					
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					
					ImGui::Checkbox(OBFUSCATE("Скелет"), &cheatClient->Wallhack.Player.skeleton);
					
					ImGui::SameLine();
					ImGui::SameLine();
					
					/*ImGui::Checkbox(OBFUSCATE("3D Коробка"), &cheatClient->Wallhack.Player.collision);
					
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					*/
					ImGui::Checkbox(OBFUSCATE("ID скина"), &cheatClient->Wallhack.Player.model_id);
					
					/*ImGui::SameLine();
					ImGui::SameLine();*/
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					
					ImGui::Checkbox(OBFUSCATE("Никнеймы"), &cheatClient->Wallhack.Player.nametags);
					
					ImGui::EndTabItem();
   				}
				
    			// Создание второй вкладки
    			if (ImGui::BeginTabItem(OBFUSCATE("Транспорт"))) {
        		// Код для содержимого второй вкладки
        			ImGui::Spacing();
					ImGui::SameLine();
					
					ImGui::Checkbox(OBFUSCATE("Линии"), &cheatClient->Wallhack.Vehicle.lines);
					
					ImGui::SameLine();
					ImGui::SameLine();
					
					//ImGui::Checkbox(OBFUSCATE("Коробка"), &cheatClient->Wallhack.Vehicle.box);
					
					/*ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					
					/*ImGui::Checkbox(OBFUSCATE("3D Коробка"), &cheatClient->Wallhack.Vehicle.collision);
					
					ImGui::SameLine();
					ImGui::SameLine();
					*/
					ImGui::Checkbox(OBFUSCATE("ID транспорта"), &cheatClient->Wallhack.Vehicle.model_id);
					
					/*ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();*/
    			}
				
				if (ImGui::BeginTabItem(OBFUSCATE("Объекты"))) {
        		// Код для содержимого второй вкладки
        			ImGui::Spacing();
					ImGui::SameLine();
					
					ImGui::Checkbox(OBFUSCATE("Линии"), &cheatClient->Wallhack.Object.lines);
					
					ImGui::SameLine();
					ImGui::SameLine();
					
					/*ImGui::Checkbox(OBFUSCATE("Коробка"), &cheatClient->Wallhack.Object.box);
					
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
					
					/*ImGui::Checkbox(OBFUSCATE("3D Коробка"), &cheatClient->Wallhack.Object.collision);
					
					ImGui::SameLine();
					ImGui::SameLine();*/
					
					ImGui::Checkbox(OBFUSCATE("ID объекта"), &cheatClient->Wallhack.Object.model_id);
					
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::SameLine();
    			}

    			// Закрытие контейнера вкладок
    			ImGui::EndTabBar();
			}
			if(wh_window_not_inited)
			{
				ImGui::SetWindowSize(ImVec2(720, 450));
				wh_window_not_inited = false;
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}
	
	if(cheatClient->Collision.menu)
	{
		ImGui::GetStyle().WindowTitleAlign.y = 0.5f;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 15.f));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, rgba(17,17,17,200));
		ImGui::Begin(OBFUSCATE("Колизии"), 0);
		{
			ImGui::SameLine();
			ImGui::SameLine();
			ImGui::Checkbox(OBFUSCATE("Игроки"), &cheatClient->Collision.ped);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::SameLine();
			ImGui::Checkbox(OBFUSCATE("Транспорт"), &cheatClient->Collision.vehicle);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::SameLine();
			/*ImGui::Checkbox("Объекты", &cheatClient->Collision.objects);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::SameLine();*/
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}
	
	// Wallhack
	if(cheatClient->Wallhack.Player.lines)
	{
		for(int i = 0; i < 1500; i++)
		{
			uintptr_t player = cheatClient->GetAtPlayerPool(i);
			if(player)
			{
				player = *(int *)(player + 4);
				if(player)
				{
					Vec3 ped_coors = cheatClient->GetPos(player);
					Vec3 local_coors = cheatClient->GetPos(cheatClient->FindLocalPlayer());
					
					ped_coors.z -= 0.85f;
					local_coors.z -= 0.8f;
					
					Vec3 local_2d_coors = {0,0,0};
					Vec3 ped_2d_coors = {0,0,0};
					
					if(WorldToScreen(&ped_coors, &ped_2d_coors))
					{
						if(ped_2d_coors.z >= 1.f)
						{
							if(WorldToScreen(&local_coors, &local_2d_coors))
							{
								if(local_2d_coors.z >= 1.f)
								{
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(local_2d_coors.x, local_2d_coors.y), ImVec2(ped_2d_coors.x, ped_2d_coors.y), ImGui::ColorConvertFloat4ToU32(cheatClient->Wallhack.Player.Config.Lines.color), cheatClient->Wallhack.Player.Config.Lines.tickness);
								}
							}
						}
					}
				}
			}
		}
	}
	
	if(cheatClient->Wallhack.Player.box)
	{
		for(int i = 0; i < 1500; i++)
		{
			uintptr_t player = cheatClient->GetAtPlayerPool(i);
			if(player)
			{
				player = *(int *)(player + 4);
				if(player)
				{
					Vec3 ped_coors = cheatClient->GetPos(player);
					
					DrawBox(ped_coors, *(float *)(player + 0x380), cheatClient->Wallhack.Player.Config.Box.color, cheatClient->Wallhack.Player.Config.Box.tickness);
				}
			}
		}
	}
	
	if(cheatClient->Wallhack.Player.skeleton)
	{
		if(!cheatClient->Wallhack.Player.Config.Skeleton.rendered)
		{
			for(int i = 0; i < cheatClient->Wallhack.Player.Config.Skeleton.buffer.size(); i++)
			{
				ImGui::GetBackgroundDrawList()->AddLine(cheatClient->Wallhack.Player.Config.Skeleton.buffer[i].start, cheatClient->Wallhack.Player.Config.Skeleton.buffer[i].end, ImGui::ColorConvertFloat4ToU32(cheatClient->Wallhack.Player.Config.Skeleton.color), cheatClient->Wallhack.Player.Config.Skeleton.tickness);
			}
			cheatClient->Wallhack.Player.Config.Skeleton.buffer.clear();
			cheatClient->Wallhack.Player.Config.Skeleton.rendered = true;
		}
	}
	
	if(cheatClient->Wallhack.Player.model_id)
	{
		for(int i = 0; i < 1500; i++)
		{
			uintptr_t player = cheatClient->GetAtPlayerPool(i);
			if(player)
			{
				player = *(int *)(player + 4);
				if(player)
				{
					Vec3 ped_coors = cheatClient->GetPos(player);
					Vec3 ped_2d_coors = {0,0,0};
					unsigned short ped_skin_id = *(unsigned short *)(player + 0x5A);
					
					if(WorldToScreen(&ped_coors, &ped_2d_coors))
					{
						if(ped_2d_coors.z >= 1.f)
						{
							char buff[20];
							sprintf(buff, OBFUSCATE("ID скина: %hu"), ped_skin_id);

							ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    						TextWithOutline(draw_list, ImVec2(ped_2d_coors.x, ped_2d_coors.y), IM_COL32(255, 255, 255, 255), buff, 30.0f, IM_COL32(0, 0, 0, 255));
						}
					}
				}
			}
		}
	}

	if(cheatClient->Wallhack.Vehicle.lines)
	{
		for(int i = 0; i < 1500; i++)
		{
			uintptr_t vehicle = cheatClient->GetAtVehiclePool(i);
			if(vehicle)
			{
				Vec3 vehicle_coors = cheatClient->GetPos(vehicle);
				Vec3 vehicle_screen_coors = {0,0,0};

				Vec3 local_coors = cheatClient->GetPos(cheatClient->FindLocalPlayer());
				Vec3 local_2d_coors = {0,0,0};
				local_coors.z -= 0.8f;
				
				if(WorldToScreen(&vehicle_coors, &vehicle_screen_coors))
				{
					if(vehicle_screen_coors.z >= 1.f)
					{
						if(WorldToScreen(&local_coors, &local_2d_coors))
						{
							if(local_2d_coors.z >= 1.f)
							{
								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(local_2d_coors.x, local_2d_coors.y), ImVec2(vehicle_screen_coors.x, vehicle_screen_coors.y), ImGui::ColorConvertFloat4ToU32(cheatClient->Wallhack.Vehicle.Config.Lines.color), cheatClient->Wallhack.Vehicle.Config.Lines.tickness);
							}
						}
					}
				}
			}
		}
	}

	if(cheatClient->Wallhack.Vehicle.model_id)
	{
		for(int i = 0; i < 1500; i++)
		{
			uintptr_t vehicle = cheatClient->GetAtVehiclePool(i);
			if(vehicle)
			{
				Vec3 vehicle_coors = cheatClient->GetPos(vehicle);
				Vec3 vehicle_2d_coors = {0,0,0};
				unsigned short vehicle_model_id = *(unsigned short *)(vehicle + 0x5A);
					
				if(WorldToScreen(&vehicle_coors, &vehicle_2d_coors))
				{
					if(vehicle_2d_coors.z >= 1.f)
					{
						char buff[20];
						sprintf(buff, OBFUSCATE("ID модели: %hu"), vehicle_model_id);
						ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    					TextWithOutline(draw_list, ImVec2(vehicle_2d_coors.x, vehicle_2d_coors.y), IM_COL32(255, 255, 255, 255), buff, 30.0f, IM_COL32(0, 0, 0, 255));
					}
				}
			}
		}
	}

	if(cheatClient->Wallhack.Object.lines)
	{
		for(int i = 0; i < 1500; i++)
		{
			uintptr_t object = cheatClient->GetAtObjectPool(i);
			if(object)
			{
				Vec3 object_coors = cheatClient->GetPos(object);
				Vec3 object_screen_coors = {0,0,0};

				Vec3 local_coors = cheatClient->GetPos(cheatClient->FindLocalPlayer());
				Vec3 local_2d_coors = {0,0,0};
				local_coors.z -= 0.8f;
				
				if(WorldToScreen(&object_coors, &object_screen_coors))
				{
					if(object_screen_coors.z >= 1.f)
					{
						if(WorldToScreen(&local_coors, &local_2d_coors))
						{
							if(local_2d_coors.z >= 1.f)
							{
								ImGui::GetBackgroundDrawList()->AddLine(ImVec2(local_2d_coors.x, local_2d_coors.y), ImVec2(object_screen_coors.x, object_screen_coors.y), ImGui::ColorConvertFloat4ToU32(cheatClient->Wallhack.Object.Config.Lines.color), cheatClient->Wallhack.Object.Config.Lines.tickness);
							}
						}
					}
				}
			}
		}
	}

	if(cheatClient->Wallhack.Object.model_id)
	{
		for(int i = 0; i < 1500; i++)
		{
			uintptr_t object = cheatClient->GetAtObjectPool(i);
			if(object)
			{
				Vec3 object_coors = cheatClient->GetPos(object);
				Vec3 object_2d_coors = {0,0,0};
				unsigned short object_model_id = *(unsigned short *)(object + 0x5A);
					
				if(WorldToScreen(&object_coors, &object_2d_coors))
				{
					if(object_2d_coors.z >= 1.f)
					{
						char buff[20];
						sprintf(buff, OBFUSCATE("ID объекта: %hu"), object_model_id);
						ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    					TextWithOutline(draw_list, ImVec2(object_2d_coors.x, object_2d_coors.y), IM_COL32(255, 255, 255, 255), buff, 30.0f, IM_COL32(0, 0, 0, 255));
					}
				}
			}
		}
	}
	
	/*for(int i = 0; i < 1500; i++)
	{
		uintptr_t player = cheatClient->GetAtPlayerPool(i);
		if(player)
		{
			player = *(int *)(player + 4);
			if(player)
			{
				CPed _player = CPed(player);
				Vec3 ped_coors = cheatClient->GetPos(player);
				Vec3 scr_coors = {0,0,0};
				if(WorldToScreen(&ped_coors, &scr_coors))
				{
					if(scr_coors.z >= 1.f)
					{
						int collision = -1;
						if((*(int *)(player + 80) & 0x100) == 0)
						{
							collision = 0;
						} else { collision = 1; }
						char buff[20];
						sprintf(buff, "Collision: %d", collision);
						TextWithOutline(ImGui::GetBackgroundDrawList(), ImVec2(scr_coors.x, scr_coors.y), IM_COL32(255, 255, 255, 255), buff, 30.0f, IM_COL32(0, 0, 0, 255));
					}
				}
			}
		}
	}*/
	//////////////////
	
    // Rendering
    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if(m_bNeedClearMousePos)
    {
        io.MousePos = ImVec2(-1, -1);
        m_bNeedClearMousePos = false;
    }

    return old_eglSwapBuffers(dpy, surface);
}

void *hack_thread(void *) {
    sleep(5);
	
	if(cheatClient == nullptr) {
		cheatClient = new Cheat();
	}
	
    //Check if target lib is loaded
    do {
        sleep(1);
		libAddr = findLibrary(OBFUSCATE("libblackrussia-client.so"));
    } while (libAddr == 0);
	
	gameLibAddr = libAddr;

	cheatClient->SetLibAddr(libAddr);
	cheatClient->screen_width = 2214;
	cheatClient->screen_height = 1080;
	cheatClient->timer = new CTimer();

    // Hook eglSwapBuffers
    auto addr = (uintptr_t)dlsym(RTLD_NEXT, OBFUSCATE("eglSwapBuffers"));
    DobbyHook((void *)addr, (void *)hook_eglSwapBuffers, (void **)&old_eglSwapBuffers);
	
	AddChatMessage = (void(*)(char*,...))(libAddr + string2Offset(OBFUSCATE("0x3BC300")) + 1);
	SendChatMessage = (void(*)(char*))(libAddr + string2Offset(OBFUSCATE("0x3447C1")));
	SendChatCommand = (void(*)(char*))(libAddr + string2Offset(OBFUSCATE("0x34487D")));
	FindGroundZ = (float(*)(float,float))(libAddr + string2Offset(OBFUSCATE("0x2F2689")));
	CPlayerPool_GetAt = (int(*)(int, unsigned short))(libAddr + string2Offset(OBFUSCATE("0x368703")));
	CVehiclePool_GetAt = (int(*)(int , unsigned short))(libAddr + string2Offset(OBFUSCATE("0x374379")));
	GetBonePosition = (int(*)(int,int))(libAddr + string2Offset(OBFUSCATE("0x3AC93B")));
	////SetSeek = (void(*)(int,float,float,float,float))(libAddr + 0x1DED50 + 1);
	SetMoveAnim = (void(*)(int))(libAddr + string2Offset(OBFUSCATE("0x37DA35")));
	CVehiclePool_New = (void(*)(int,unsigned short*))(libAddr + string2Offset(OBFUSCATE("0x374389")));

	DobbyHook((void *)(libAddr + string2Offset(OBFUSCATE("0x41AD5C")) + 1), (void *)Java_com_blackhub_bronline_game_core_JNILib_multiTouchEvent, (void **)&old_Java_com_blackhub_bronline_game_core_JNILib_multiTouchEvent);
    MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x3CC6BD"))), (void *)ChatWindowInputHandler, (void **)&old_ChatWindowInputHandler);
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x2C9F19"))), (void *)CGame_Process, (void **)&old_CGame_Process);
	//MSHookFunction((void*)(libAddr + 0x1BD7D0 + 1), (void *)SetVehicleVelocityRPC, (void **)&old_SetVehicleVelocityRPC);*/
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x2EFC25"))), (void *)GetIsLineOfSightClear, (void **)&old_GetIsLineOfSightClear);
	//MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x32301D"))), (void *)CPlayerBubbleChatPool_New, (void **)&old_CPlayerBubbleChatPool_New);
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x3B0B45"))), (void *)CSprintCalculator, (void **)&old_CSprintCalculator);
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x3A8CF9"))), (void *)CPedDamageCalculator, (void **)&old_CPedDamageCalculator);
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x4610B5"))), (void *)CVehicleDamageCalculator, (void **)&old_CVehicleDamageCalculator);
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x44F511"))), (void *)NoBikeCalculator, (void **)&old_NoBikeCalculator);
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x380D5D"))), (void *)CPed_ProcessBuoyancy, (void **)&old_CPed_ProcessBuoyancy);
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x44C4F1"))), (void *)CBike_ProcessBuoyancy, (void **)&old_CBike_ProcessBuoyancy);
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x43C9A1"))), (void *)CAutomobile_ProcessBuoyancy, (void **)&old_CAutomobile_ProcessBuoyancy);
	//MSHookFunction((void*)(libAddr + 0x203FF0 + 1), (void *)log_1, (void **)&old_log1); // cvehiclepool_new
	//MSHookFunction((void*)(libAddr + 0x28D808 + 1), (void *)log_2, (void **)&old_log2);
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x482BF5"))), (void *)log_3, (void **)&old_log3);
	//MSHookFunction((void*)(libAddr + 0x1B3910 + 1), (void *)log_4, (void **)&old_log4);
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x3695B1"))), (void *)ScrServerJoin, (void **)&old_ScrServerJoin);
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x369675"))), (void *)ScrServerQuit, (void **)&old_ScrServerQuit);
	////MSHookFunction((void*)(libAddr + 0x1FA270 + 1), (void *)ScrConnectionRejected, (void **)&old_ScrConnectionRejected);
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x368713"))), (void *)ScrServerNewPlayer, (void **)&old_ScrServerNewPlayer);
	//MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x1F9329"))), (void *)ScrServerDeletePlayer, (void **)&old_ScrServerDeletePlayer);
	//MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x"))), (void *)WorldVehicleDelete, (void **)&old_WorldVehicleDelete);
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x3691C1"))), (void *)ScrInitGame, (void **)&old_ScrInitGame);
	//MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x36979D"))), (void *)ScrRequestSpawn, (void **)&old_ScrRequestSpawn);
	//MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x"))), (void *)ScrSetSpawnInfo, (void **)&old_ScrSetSpawnInfo);
	//MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x"))), (void *)LocalSetSpawnInfo, (void **)&old_LocalSetSpawnInfo);
	MSHookFunction((void*)(libAddr + string2Offset(OBFUSCATE("0x3BC301"))), (void *)AddChatMessage_hook, (void **)&old_AddChatMessage);
	
    pthread_exit(nullptr);
    return nullptr;
}

void log_4(int a1, int a2, int a3, float a4, float a5, float a6)
{
	//AddChatMessage("a1: %d, a2: %d, a3: %d, a4: %f, a5: %f, a6: %f", a1,a2,a3,a4,a5,a6);
	old_log4(a1,a2,a3,a4,a5,a6);
}

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);
	pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL Java_com_blackhub_bronline_game_core_JNILib_multiTouchEvent(JNIEnv* env, jclass cls, int actionMasked, int pointerId, int x1, int y1, int x2, int y2, int x3, int y3)
{
	const int ACTION_DOWN = 0;
	const int ACTION_POINTER_DOWN = 5;
	const int ACTION_UP = 1;
	const int ACTION_POINTER_UP = 6;
	const int ACTION_MOVE_UNK = 7;
	const int ACTION_MOVE = 2;
	
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindow* icon_window = ImGui::FindWindowByName(OBFUSCATE("mod-icon"));
	ImGuiWindow* menu_window = ImGui::FindWindowByName(OBFUSCATE("OneShot"));
	ImGuiWindow* wallhack_window = ImGui::FindWindowByName(OBFUSCATE("Wallhack"));
	ImGuiWindow* change_car_model_window = ImGui::FindWindowByName(OBFUSCATE("Подмена т/с"));
	ImGuiWindow* game_speed_window = ImGui::FindWindowByName(OBFUSCATE("Скорость игры"));
	ImGuiWindow* tp_toPeds_window = ImGui::FindWindowByName(OBFUSCATE("ТП к ближайшим игрокам"));
	
	if(actionMasked == ACTION_DOWN || actionMasked == ACTION_POINTER_DOWN)
	{
		// ImGui
		
		if(icon_window != nullptr && !hideIcon)
		{
			ImVec2 icon_window_pos = icon_window->Pos;
			ImVec2 icon_window_size = icon_window->Size;
				
			if(x1 > icon_window_pos.x && x1 < icon_window_pos.x + icon_window_size.x
			&& y1 > icon_window_pos.y && y1 < icon_window_pos.y + icon_window_size.y)
			{
				icon_is_hovered = true;
				start_touch_x = x1;
				start_touch_y = y1;
				io.MousePos = ImVec2(x1, y1);
            	io.MouseDown[0] = true;
				return;
			}
			else if(x2 > icon_window_pos.x && x2 < icon_window_pos.x + icon_window_size.x
			&& y2 > icon_window_pos.y && y2 < icon_window_pos.y + icon_window_size.y)
			{
				icon_is_hovered = true;
				start_touch_x = x2;
				start_touch_y = y2;
				io.MousePos = ImVec2(x2, y2);
            	io.MouseDown[0] = true;
				return;
			}
			else if(x3 > icon_window_pos.x && x3 < icon_window_pos.x + icon_window_size.x
			&& y3 > icon_window_pos.y && y3 < icon_window_pos.y + icon_window_size.y)
			{
				icon_is_hovered = true;
				start_touch_x = x3;
				start_touch_y = y3;
				io.MousePos = ImVec2(x3, y3);
            	io.MouseDown[0] = true;
				return;
			}
		}
			
		if(menu_window != nullptr && menuOpen)
		{
			ImVec2 menu_window_pos = menu_window->Pos;
			ImVec2 menu_window_size = menu_window->Size;
				
			if(x1 > menu_window_pos.x && x1 < menu_window_pos.x + menu_window_size.x
			&& y1 > menu_window_pos.y && y1 < menu_window_pos.y + menu_window_size.y)
			{
				menu_is_hovered = true;
				io.MousePos = ImVec2(x1, y1);
           		io.MouseDown[0] = true;
				return;
			}
			else if(x2 > menu_window_pos.x && x2 < menu_window_pos.x + menu_window_size.x
			&& y2 > menu_window_pos.y && y2 < menu_window_pos.y + menu_window_size.y)
			{
				menu_is_hovered = true;
				io.MousePos = ImVec2(x2, y2);
           		io.MouseDown[0] = true;
				return;
			}
			else if(x3 > menu_window_pos.x && x3 < menu_window_pos.x + menu_window_size.x
			&& y3 > menu_window_pos.y && y3 < menu_window_pos.y + menu_window_size.y)
			{
				menu_is_hovered = true;
				io.MousePos = ImVec2(x3, y3);
           		io.MouseDown[0] = true;
				return;
			}
		}
		
		if(cheatClient->Wallhack.menu && wallhack_window != nullptr)
		{
			ImVec2 menu_window_pos = wallhack_window->Pos;
			ImVec2 menu_window_size = wallhack_window->Size;
				
			if(x1 > menu_window_pos.x && x1 < menu_window_pos.x + menu_window_size.x
			&& y1 > menu_window_pos.y && y1 < menu_window_pos.y + menu_window_size.y)
			{
				wallhack_menu_is_hovered = true;
				io.MousePos = ImVec2(x1, y1);
           		io.MouseDown[0] = true;
				return;
			}
			else if(x2 > menu_window_pos.x && x2 < menu_window_pos.x + menu_window_size.x
			&& y2 > menu_window_pos.y && y2 < menu_window_pos.y + menu_window_size.y)
			{
				wallhack_menu_is_hovered = true;
				io.MousePos = ImVec2(x2, y2);
           		io.MouseDown[0] = true;
				return;
			}
			else if(x3 > menu_window_pos.x && x3 < menu_window_pos.x + menu_window_size.x
			&& y3 > menu_window_pos.y && y3 < menu_window_pos.y + menu_window_size.y)
			{
				wallhack_menu_is_hovered = true;
				io.MousePos = ImVec2(x3, y3);
           		io.MouseDown[0] = true;
				return;
			}
		}
		
		if(cheatClient->GameSpeed.menu && game_speed_window != nullptr)
		{
			ImVec2 menu_window_pos = game_speed_window->Pos;
			ImVec2 menu_window_size = game_speed_window->Size;
				
			if(x1 > menu_window_pos.x && x1 < menu_window_pos.x + menu_window_size.x
			&& y1 > menu_window_pos.y && y1 < menu_window_pos.y + menu_window_size.y)
			{
				game_speed_menu_is_hovered = true;
				io.MousePos = ImVec2(x1, y1);
           		io.MouseDown[0] = true;
				return;
			}
			else if(x2 > menu_window_pos.x && x2 < menu_window_pos.x + menu_window_size.x
			&& y2 > menu_window_pos.y && y2 < menu_window_pos.y + menu_window_size.y)
			{
				game_speed_menu_is_hovered = true;
				io.MousePos = ImVec2(x2, y2);
           		io.MouseDown[0] = true;
				return;
			}
			else if(x3 > menu_window_pos.x && x3 < menu_window_pos.x + menu_window_size.x
			&& y3 > menu_window_pos.y && y3 < menu_window_pos.y + menu_window_size.y)
			{
				game_speed_menu_is_hovered = true;
				io.MousePos = ImVec2(x3, y3);
           		io.MouseDown[0] = true;
				return;
			}
		}
		
		if(cheatClient->VehicleCheat.CarChanger.menu && change_car_model_window != nullptr)
		{
			ImVec2 menu_window_pos = change_car_model_window->Pos;
			ImVec2 menu_window_size = change_car_model_window->Size;
				
			if(x1 > menu_window_pos.x && x1 < menu_window_pos.x + menu_window_size.x
			&& y1 > menu_window_pos.y && y1 < menu_window_pos.y + menu_window_size.y)
			{
				change_car_model_menu_is_hovered = true;
				io.MousePos = ImVec2(x1, y1);
           		io.MouseDown[0] = true;
				return;
			}
			else if(x2 > menu_window_pos.x && x2 < menu_window_pos.x + menu_window_size.x
			&& y2 > menu_window_pos.y && y2 < menu_window_pos.y + menu_window_size.y)
			{
				change_car_model_menu_is_hovered = true;
				io.MousePos = ImVec2(x2, y2);
           		io.MouseDown[0] = true;
				return;
			}
			else if(x3 > menu_window_pos.x && x3 < menu_window_pos.x + menu_window_size.x
			&& y3 > menu_window_pos.y && y3 < menu_window_pos.y + menu_window_size.y)
			{
				change_car_model_menu_is_hovered = true;
				io.MousePos = ImVec2(x3, y3);
           		io.MouseDown[0] = true;
				return;
			}
		}
		
		if(cheatClient->Teleport.toPeds && tp_toPeds_window != nullptr)
		{
			ImVec2 menu_window_pos = tp_toPeds_window->Pos;
			ImVec2 menu_window_size = tp_toPeds_window->Size;
				
			if(x1 > menu_window_pos.x && x1 < menu_window_pos.x + menu_window_size.x
			&& y1 > menu_window_pos.y && y1 < menu_window_pos.y + menu_window_size.y)
			{
				tp_toPeds_menu_is_hovered = true;
				io.MousePos = ImVec2(x1, y1);
           		io.MouseDown[0] = true;
				return;
			}
			else if(x2 > menu_window_pos.x && x2 < menu_window_pos.x + menu_window_size.x
			&& y2 > menu_window_pos.y && y2 < menu_window_pos.y + menu_window_size.y)
			{
				tp_toPeds_menu_is_hovered = true;
				io.MousePos = ImVec2(x2, y2);
           		io.MouseDown[0] = true;
				return;
			}
			else if(x3 > menu_window_pos.x && x3 < menu_window_pos.x + menu_window_size.x
			&& y3 > menu_window_pos.y && y3 < menu_window_pos.y + menu_window_size.y)
			{
				tp_toPeds_menu_is_hovered = true;
				io.MousePos = ImVec2(x3, y3);
           		io.MouseDown[0] = true;
				return;
			}
		}
		
		return old_Java_com_blackhub_bronline_game_core_JNILib_multiTouchEvent(env,cls,actionMasked,pointerId,x1,y1,x2,y2,x3,y3);
			
		// Other
		// ...
	}
	
	if(actionMasked == ACTION_MOVE || actionMasked == ACTION_MOVE_UNK)
	{
		// ImGui
		if(icon_is_hovered || menu_is_hovered || wallhack_menu_is_hovered || game_speed_menu_is_hovered || change_car_model_menu_is_hovered
		|| tp_toPeds_menu_is_hovered)
		{
			if(pointerId == 0) {
				io.MousePos = ImVec2(x1, y1);
			} else if (pointerId == 1) {
				io.MousePos = ImVec2(x2, y2);
			} else if (pointerId == 2) {
				io.MousePos = ImVec2(x3, y3);
			}
			return;
		}
			
		return old_Java_com_blackhub_bronline_game_core_JNILib_multiTouchEvent(env,cls,actionMasked,pointerId,x1,y1,x2,y2,x3,y3);

		// Other
		// ...
	}
	if(actionMasked == ACTION_UP || actionMasked == ACTION_POINTER_UP)
	{
		// ImGui
		io.MouseDown[0] = false;
        m_bNeedClearMousePos = true;
		
		int rawX = 0;
		int rawY = 0;
		
		if(icon_is_hovered)
		{
			if(pointerId == 0) {
				rawX = x1 - start_touch_x;
				rawY = y1 - start_touch_y;
			} else if (pointerId == 1) {
				rawX = x2 - start_touch_x;
				rawY = y2 - start_touch_y;
			} else if (pointerId == 2) {
				rawX = x3 - start_touch_x;
				rawY = y3 - start_touch_y;
			}
		
			if (rawX < 10 && rawY < 10)
			{
				menuOpen =! menuOpen;
			}
		}
		
		icon_is_hovered = false;
		menu_is_hovered = false;
		wallhack_menu_is_hovered = false;
		game_speed_menu_is_hovered = false;
		change_car_model_menu_is_hovered = false;
		tp_toPeds_menu_is_hovered = false;
			
		start_touch_x = 0;
		start_touch_y = 0;
			
		// Other
		// ...
		return old_Java_com_blackhub_bronline_game_core_JNILib_multiTouchEvent(env,cls,actionMasked,pointerId,x1,y1,x2,y2,x3,y3);
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_com_oneshot_br_Touch_sendTouch(JNIEnv* env, jobject obj, jint type, jint x, jint y)
{
}

extern "C"
JNIEXPORT void JNICALL
Java_wrapper_Wrapper_next(JNIEnv* env, jobject obj, jobject mActivity)
{
	BrNotificationUpdate(env);
}

extern "C"
JNIEXPORT void JNICALL
Java_wrapper_Wrapper_initbaby(JNIEnv* env, jclass cls, jobject context)
{
	startService(env, context);
}

// Загрузка изображения с использованием stb_image.h
GLuint LoadTexture(const char* path) {
    int width, height, channels;
    unsigned char* image_data = stbi_load(path, &width, &height, &channels, 0);
    if (!image_data) {
        // Обработка ошибки загрузки изображения
        return 0;
    }

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image_data);

    return texture_id;
}

void AddChatMessage_hook(char* msg)
{
	/*if(cheatClient->trigger_gevent && strstr(msg, "/gevent"))
	{
		SendChatCommand("/gevent");
	}*/
	
	if(cheatClient->VehicleCheat.CarChanger.entered)
	{
		return;
	}
	
	old_AddChatMessage(msg);
}

void ChatWindowInputHandler(char* input)
{
	if(!strcmp(input, OBFUSCATE("/oneshot")))
	{
		menuOpen =! menuOpen;
		return;
	}
	
	if(!strcmp(input, OBFUSCATE("/slap")))
	{
		uintptr_t localped = cheatClient->FindLocalPlayer();
		if(localped)
		{
			*(float *)(localped + string2Offset(OBFUSCATE("0x3C"))) += cheatClient->PlayerCheat.Base.Slap.up;
		}
		return;
	}
	
	if(!strcmp(input, OBFUSCATE("/down")))
	{
		uintptr_t localped = cheatClient->FindLocalPlayer();
		if(localped)
		{
			*(float *)(localped + string2Offset(OBFUSCATE("0x3C"))) -= cheatClient->PlayerCheat.Base.Slap.down;
		}
		return;
	}
	
	if(cheatClient->Flooder.wait_text_input)
	{
		cheatClient->Flooder.wait_text_input = false;
		cheatClient->Flooder.message = input;
		cheatClient->Flooder.menu = true;
		return;
	}
	
	if(cheatClient->Flooder.wait_delay_input)
	{
		cheatClient->Flooder.wait_delay_input = false;
		cheatClient->Flooder.delay_after_message = std::stoi(input);
		cheatClient->Flooder.menu = true;
		return;
	}
	
	old_ChatWindowInputHandler(input);
}

void CDraw_SetFOV(float val)
{
	old_CDraw_SetFOV(val);
}

void CGame_Process()
{
	old_CGame_Process();
	
	cheatClient->timer->Update();
	uintptr_t localped = cheatClient->FindLocalPlayer();
	if(localped)
	{
		CPed localplayer = CPed(localped);

		Vec3 local_coors = cheatClient->GetPos(localped);
		
		*(float *)(libAddr + string2Offset(OBFUSCATE("0x464508C"))) = cheatClient->GameSpeed.speed;
		
		if(cheatClient->VisualCheat.TimeChanger.freeze)
		{
			*(int *)(gameLibAddr + string2Offset(OBFUSCATE("0x4B6263D"))) = cheatClient->VisualCheat.TimeChanger.time;
		}

		// Быстрый поворот
		if(cheatClient->PlayerCheat.Base.Control.fastRotate)
		{
			localplayer.SetHeadingRate(7.5f * cheatClient->PlayerCheat.Base.Control.rate);
		}
		else
		{
			localplayer.SetHeadingRate(7.5f);
		}
		
		// Авто аптечка
		if(cheatClient->PlayerCheat.Rage.Invulnerability.autoHeal)
		{
			if(localplayer.GetHealth() < 40.f)
			{
				SendChatCommand(OBFUSCATE("/healme"));
			}
		}
		
		// Fly Ped
		if(cheatClient->PlayerCheat.Base.FlyPed.state)
		{
			uintptr_t localped = cheatClient->FindLocalPlayer();
			if(localped)
			{
				if(*(char *)(localped + string2Offset(OBFUSCATE("0x150"))))
				{
					cheatClient->PlayerCheat.Base.FlyPed.speed = 0.f;
				}
				else
				{
					Vec3 localpos = cheatClient->GetPos(localped);
					Vec3 camPos = cheatClient->GetCameraCoors();
					float warpX = localpos.x - camPos.x;
					float warpY = localpos.y - camPos.y;
					float warpZ = localpos.z - camPos.z;
					cheatClient->PlayerCheat.Base.FlyPed.speed += 0.1f;
					if(cheatClient->PlayerCheat.Base.FlyPed.speed > 10.f) {
						cheatClient->PlayerCheat.Base.FlyPed.speed = 10.f;
					}
					*(float *)(localped + string2Offset(OBFUSCATE("0x7C"))) = warpX * (cheatClient->PlayerCheat.Base.FlyPed.speed * 0.01f);
					*(float *)(localped + string2Offset(OBFUSCATE("0x80"))) = warpY * (cheatClient->PlayerCheat.Base.FlyPed.speed * 0.01f);
					*(float *)(localped + string2Offset(OBFUSCATE("0x84"))) = warpZ * (cheatClient->PlayerCheat.Base.FlyPed.speed * 0.01f);
				}
			}
		}
		
		if(cheatClient->Flooder.working)
		{
			if(cheatClient->timer->GetTimeInMilliseconds() - cheatClient->Flooder.last_sended_message_time > cheatClient->Flooder.delay_after_message)
			{
				if(cheatClient->Flooder.message[0] == '/')
				{
					SendChatCommand(strdup(cheatClient->Flooder.message.c_str()));
				}
				else
				{
					SendChatMessage(strdup(cheatClient->Flooder.message.c_str()));
				}
				cheatClient->Flooder.last_sended_message_time = cheatClient->timer->GetTimeInMilliseconds();
			}
		}
		// NameTags
		if(cheatClient->Wallhack.Player.nametags)
		{
			if(cheatClient->GetNameTagsDrawDistance() != 2000.f)
			{
				cheatClient->SetNameTagsDrawDistance(2000.f);
			}
		}
		else
		{
			cheatClient->SetNameTagsDrawDistance(10.f);
		}
		
		// Вечный двигатель
		if(cheatClient->VehicleCheat.Invulnerability.engine)
		{
			if(localplayer.GetVehicle())
			{
				CVehicle veh = CVehicle(localplayer.GetVehicle());
				if(veh.GetDriver() == localplayer.GetMe())
				{
					veh.SetEngineState(true);
				}
			}
		}
		
		if(cheatClient->PlayerCheat.Rage.NoReload.state)
		{
			// deagle
			cheatClient->PlayerCheat.Rage.NoReload.deagleClip = *(int *)(localped + string2Offset(OBFUSCATE("0x444")));
			if(cheatClient->PlayerCheat.Rage.NoReload.deagleClip == 1)
			{
				*(int *)(localped + string2Offset(OBFUSCATE("0x444"))) += 1;
				cheatClient->PlayerCheat.Rage.NoReload.prev_deagleClip = cheatClient->PlayerCheat.Rage.NoReload.deagleClip;
			}
			
			// ak47
			cheatClient->PlayerCheat.Rage.NoReload.AK47Clip = *(int *)(localped + string2Offset(OBFUSCATE("0x474")));
			if(cheatClient->PlayerCheat.Rage.NoReload.AK47Clip == 1)
			{
				*(int *)(localped + string2Offset(OBFUSCATE("0x474"))) += 1;
				cheatClient->PlayerCheat.Rage.NoReload.prev_AK47Clip = cheatClient->PlayerCheat.Rage.NoReload.AK47Clip;
			}
			
			// M4
			cheatClient->PlayerCheat.Rage.NoReload.m4Clip = *(int *)(localped + string2Offset(OBFUSCATE("0x48C")));
			
			if(cheatClient->PlayerCheat.Rage.NoReload.m4Clip == 1)
			{
				*(int *)(localped + string2Offset(OBFUSCATE("0x48C"))) += 1;
				cheatClient->PlayerCheat.Rage.NoReload.prev_m4Clip = cheatClient->PlayerCheat.Rage.NoReload.m4Clip;
			}
		}
		
		// CarShoot
		if(cheatClient->VehicleCheat.CarShot.state)
		{
			if(localplayer.GetVehicle())
			{
				CVehicle veh = CVehicle(localplayer.GetVehicle());
				if(veh.GetDriver() == localplayer.GetMe())
				{
					uintptr_t vehicle = localplayer.GetVehicle();
					uintptr_t vehicleMatrix = vehicle + 4;
					float cameraAngle = atan2f(*(float *)(cheatClient->GetCameraMatrix() + 4 + 16), *(float *)(cheatClient->GetCameraMatrix() + 4 + 20));
					
					cheatClient->SetRotate(vehicleMatrix, 0, 0, -cameraAngle);
					//cheatClient->SetVehicleSpeed(vehicle, cheatClient->CarShoot.speed);
					
					if(cheatClient->Accelerate.isPressed)
					{
						cheatClient->VehicleCheat.CarShot.speed += cheatClient->VehicleCheat.CarShot.accelerate;
						if(cheatClient->VehicleCheat.CarShot.speed >= 100.f)
						{
							cheatClient->VehicleCheat.CarShot.speed = 100.f;
						}
					}
					else
					{
						cheatClient->VehicleCheat.CarShot.speed = 0;
					}
				}
			}
		}
		
		/*for(int i = 0; i < 1500; i++)
		{
			int playerpool = cheatClient->GetPlayerPool();
			if(!playerpool) { continue; }
			uintptr_t player = CPlayerPool_GetAt(playerpool, i);
			if(player)
			{
				player = *(int *)(player + 4);
				if(player)
				{
					if(player == contact_entity)
					{
						int number = *(int *)(player + 80);
						number &= ~0x100;
						*(int *)(player + 80) = number;
					}
					else
					{
						int number = *(int *)(player + 80);
						number |= 0x100;
						*(int *)(player + 80) = number;
					}
				}
			}
		}*/
		
		// АимБот
		if(cheatClient->Aimbot.state)
		{
			Vec3 camera_pos = cheatClient->GetCameraCoors();
			auto bestFov = 180.f;
			auto bestAngle = Vec3(0,0,0);
			// Если игрок атакует
			if(*(int *)(localped + std::stoi(OBFUSCATE("592"))) == PEDSTATE_AIMGUN || *(int *)(localped + 592) == PEDSTATE_FIGHT || *(int *)(localped + 592) == PEDSTATE_ATTACK)
			{
				// Получить текущее оружие
				if(cheatClient->Aimbot.target_id != -1)
				{
					uintptr_t target = cheatClient->GetAtPlayerPool(cheatClient->Aimbot.target_id);
					if(target) {
						target = *(int *)(target + 4);
						if(target) {
							Vec3 target_coors = cheatClient->GetPos(target);
							Vec3 viewangle = Vec3(*(float *)(libAddr + 0x825CFC), *(float *)(libAddr + 0x825CEC), 0);
							float angle = GetAngleBetweenTwoPoints(camera_pos.x, camera_pos.y, target_coors.x, target_coors.y);
							
							float				ax, az, fz, fx;
							Vec3				vect = {0,0,0};
							float				screenAspectRatio = 0.457f;
							float				crosshairOffset[2] = {0.f,0.f};
							
							// calculate distance vector
							vect = local_coors - target_coors;
			
							//if (pCam->Mode == 53 || pCam->Mode == 55) // 3rd person mode
							{
								// weird shit
								float mult = tan(67.f * 0.5f * 0.017453292f);
								fz = ( M_PI * 2.f) - atan2(1.0f, mult * ((0.5f - crosshairOffset[0] + 0.5f - crosshairOffset[0]) * (1.0f / screenAspectRatio)));
								fx = M_PI - atan2(1.0f, mult * (crosshairOffset[1] - 0.5f + crosshairOffset[1] - 0.5f));
							}
							//else
							{
								//fx = fz = M_PI / 2;
							}
							// x angle
							float dist = sqrt(vect.x * vect.x + vect.y * vect.y);
							az = atan2f(dist, vect.z);

							// z angle
							ax = atan2f(vect.y, -vect.x) - M_PI / 2;

							*(float *)(libAddr + 0x825CEC) = az - fz;
							//*(float *)(libAddr + 0x59D8C4) = -(ax - fx);
							/*pCam->AlphaSpeed = 0.0f;
							pCam->BetaSpeed = 0.0f;*/
			
							/*Vec3 target_on_screen = {0,0,0};
							if(WorldToScreen(&target_coors, &target_on_screen))
							{
								if(target_on_screen.z >= 1.f)
								{
									if(target_on_screen.x >= (cheatClient->screen_width * crosshair_offset_x) - 5 &&
									target_on_screen.x <= (cheatClient->screen_width * crosshair_offset_x) + 5 &&
									target_on_screen.y >= (cheatClient->screen_height * crosshair_offset_y) - 5 &&
									target_on_screen.y <= (cheatClient->screen_height * crosshair_offset_y) + 5)
									{
										
									}
									else if(target_on_screen.x >= (cheatClient->screen_width * crosshair_offset_x) + 10)
									{
										cheatClient->Aimbot.diffX -= 1.f;
									}
									else if(target_on_screen.x <= (cheatClient->screen_width * crosshair_offset_x) - 10)
									{
										cheatClient->Aimbot.diffX += 1.f;
									}
									else if(target_on_screen.y >= (cheatClient->screen_height * crosshair_offset_y) + 10)
									{
										cheatClient->Aimbot.diffY -= 0.5f;
									}
									else if(target_on_screen.y <= (cheatClient->screen_height * crosshair_offset_y) - 10)
									{
										cheatClient->Aimbot.diffY += 0.5f;
									}
								}
								else
								{
									cheatClient->Aimbot.diffX += 1.f;
								}
							}
							
							*(float *)(libAddr + 0x59D8C4) = angle + ((cheatClient->Aimbot.diffX * M_PI) / 180.f);
							*(float *)(libAddr + 0x59D8B4) = (cheatClient->Aimbot.diffY * M_PI) / 180.f;*/
						}
					}
				}
			}
			else
			{
				cheatClient->Aimbot.diffX = 0;
				cheatClient->Aimbot.diffY = 0;
				
				int target_id = cheatClient->FindNearestPlayer();
				if(target_id != -1) {
					uintptr_t target = cheatClient->GetAtPlayerPool(target_id);
					if(target) {
						target = *(int *)(target + 4);
						if(target) {
							cheatClient->Aimbot.target_id = target_id;
							cheatClient->Aimbot.target = target;
						}
						else
						{
							cheatClient->Aimbot.target_id = -1;
							cheatClient->Aimbot.target = 0;
						}
					}
					else
					{
						cheatClient->Aimbot.target_id = -1;
						cheatClient->Aimbot.target = 0;
					}
				} else {
					cheatClient->Aimbot.target_id = target_id;
					//cheatClient->target = cheatClient->GetAtPlayerPool(target_id);
				}
			}
		}
		
		if(cheatClient->PlayerCheat.Rage.Damager.state)
		{
			int now_time = cheatClient->timer->GetTimeInMilliseconds();
			int weaponId = cheatClient->GetWeaponId();
			
				if(now_time - cheatClient->PlayerCheat.Rage.Damager.last_shot > 500)
				{
					if(gpeer != 0 && ga7 != 0 && ga8 != 0)
					{
						int near_id = cheatClient->FindNearestPlayer();
						if(near_id != -1)
						{
							uintptr_t player = cheatClient->GetAtPlayerPool(near_id);
							if(player) {
								player = *(int *)(player + 4);
								if(player) {
									RakNet::BitStream bitStream;

    								bitStream.Write((bool)false);
    								bitStream.Write((unsigned short)cheatClient->FindNearestPlayer());
    								bitStream.Write((float)1000.f);
    								bitStream.Write((int)9);
    								bitStream.Write((int)3);
									
									int playergivetakedamage = 115;
								
									log_3(gpeer, &playergivetakedamage, &bitStream, 1, 9, 0, ga7, ga8, 0, 0, -1, 0xFFFF, 0xFFFF, 0);
								}
							}
						}
					}
					cheatClient->PlayerCheat.Rage.Damager.last_shot = now_time;
				}
			
		}
		
		if(cheatClient->AutoFarm.BotMiner.state)
		{
			// Настройка маршрута
			if(!cheatClient->AutoFarm.BotMiner.haveStone)
			{
				cheatClient->AutoFarm.BotMiner.prev_point = cheatClient->AutoFarm.BotMiner.start_point;
			}
			else
			{
				cheatClient->AutoFarm.BotMiner.prev_point = cheatClient->AutoFarm.BotMiner.end_point;
			}
			
			*(float *)(localped + string2Offset(OBFUSCATE("0x384"))) = (((GetRadianAngleBetweenPoints( local_coors.x, local_coors.y, cheatClient->AutoFarm.BotMiner.prev_point.x, cheatClient->AutoFarm.BotMiner.prev_point.y) * (180.f / M_PI)) - 90.f) * M_PI) / 180.f;
			
			// Настройка анимаций
			if(!cheatClient->AutoFarm.BotMiner.goingToPoint && !cheatClient->AutoFarm.BotMiner.mining && !cheatClient->AutoFarm.BotMiner.haveStone)
			{
				// Сброс анимации
				*(int *)(localped + 592) = 1;
				*(int *)(localped + 596) = 0;
				*(int *)(localped + 600) = 1;
				*(int *)(localped + 612) = 0;
				*(int *)(localped + 616) = 0;
				SetMoveAnim(localped);
				
				// Запуск нужной анимации
				*(int *)(localped + 592) = 31;
				*(int *)(localped + 596) = 0;
				*(int *)(localped + 600) = 5;
				SetMoveAnim(localped);
				cheatClient->AutoFarm.BotMiner.goingToPoint = true;
			} else if(cheatClient->AutoFarm.BotMiner.goingToPoint && cheatClient->AutoFarm.BotMiner.mining) {
				// Сброс анимации
				*(int *)(localped + 592) = 1;
				*(int *)(localped + 596) = 0;
				*(int *)(localped + 600) = 1;
				*(int *)(localped + 612) = 0;
				*(int *)(localped + 616) = 0;
				SetMoveAnim(localped);
				cheatClient->AutoFarm.BotMiner.goingToPoint = false;
			} else if(!cheatClient->AutoFarm.BotMiner.goingToPoint && cheatClient->AutoFarm.BotMiner.haveStone) {
				if(cheatClient->timer->GetTimeInMilliseconds() - cheatClient->AutoFarm.BotMiner.takingStoneLastTime > 3000)
				{
					// Сброс анимации
					*(int *)(localped + 592) = 1;
					*(int *)(localped + 596) = 0;
					*(int *)(localped + 600) = 1;
					*(int *)(localped + 612) = 0;
					*(int *)(localped + 616) = 0;
					SetMoveAnim(localped);
				
					// Запуск нужной анимации
					*(int *)(localped + 592) = 31;
					*(int *)(localped + 596) = 0;
					*(int *)(localped + 600) = 4;
					SetMoveAnim(localped);
					cheatClient->AutoFarm.BotMiner.goingToPoint = true;
				}
			}
			
			// Детект: добывает ли бот руду
			if(localplayer.GetAnimationID() == 249)
			{
				cheatClient->AutoFarm.BotMiner.mining = true;
			}
			else
			{
				cheatClient->AutoFarm.BotMiner.mining = false;
			}
			
			// Детект: бот взял руду
			if(localplayer.GetAnimationID() == 273)
			{
				cheatClient->AutoFarm.BotMiner.takingStoneLastTime = cheatClient->timer->GetTimeInMilliseconds();
				cheatClient->AutoFarm.BotMiner.haveStone = true;
			}
			
			if(localplayer.GetAnimationID() == 274)
			{
				cheatClient->AutoFarm.BotMiner.haveStone = false;
				cheatClient->AutoFarm.BotMiner.goingToPoint = false;
			}
			
		}
		
		if(cheatClient->VehicleCheat.CarChanger.entered)
		{
			uintptr_t vehicle = localplayer.GetVehicle();
			if(vehicle)
			{
				CVehicle vehCls = CVehicle(vehicle);
				int vehicle_network_id = 0;
				cheatClient->VehicleCheat.CarChanger.prev_health = vehCls.GetHealth();
				int rw_object = *(int *)(vehicle + string2Offset(OBFUSCATE("0x44")));
				cheatClient->VehicleCheat.CarChanger.prev_angle = atan2f(*(float *)(rw_object + 16), *(float *)(rw_object + 20));
				
				for(int i = 0; i < 1500; i++)
				{
					uintptr_t veh = cheatClient->GetAtVehiclePool(i);
					if(veh == vehicle)
					{
						cheatClient->VehicleCheat.CarChanger.prev_network_id = i;
						break;
					}
				}
				
				// Выкидываем из транспорта
				if(localplayer.GetVehicle())
				{
					cheatClient->VehicleCheat.CarChanger.prev_vehicle = vehicle;
					Vec3 coors = {0,0,0};
					((void (*)(Vec3*))(libAddr + string2Offset(OBFUSCATE("0x2D7B21"))))(&coors);
					((void (*)(int,float,float,float))(libAddr + string2Offset(OBFUSCATE("0x38CA41"))))(localped, coors.x, coors.y, coors.z);
					cheatClient->VehicleCheat.CarChanger.removedFromVehicle = true;
				}
			}
			if(cheatClient->VehicleCheat.CarChanger.removedFromVehicle)
			{
				localplayer.Update();
				// Удаляем транспорт
				if(!localplayer.GetVehicle())
				{
					unsigned short* structure = new unsigned short[13];
					*(int *)(structure + 1) = std::stoi(cheatClient->VehicleCheat.CarChanger.numpad);
					*structure = cheatClient->VehicleCheat.CarChanger.prev_network_id;
					structure[11] = 65535;
					*((float *)structure + 6) = cheatClient->VehicleCheat.CarChanger.prev_health;
						
					*(int *)(structure + 3) = *(int *)(localped + string2Offset(OBFUSCATE("0x34")));
					*((int *)(structure + 3) + 1) = *(int *)(localped + string2Offset(OBFUSCATE("0x38")));
					*((int *)(structure + 3) + 2) = *(int *)(localped + string2Offset(OBFUSCATE("0x3C")));
					
					CVehiclePool_New(cheatClient->GetVehiclePool(), structure);
					delete[] structure;
					
					// Выравниваем угол машины как было до этого
				((void (*)(int,float))(libAddr + string2Offset(OBFUSCATE("0x316C37"))))(cheatClient->VehicleCheat.CarChanger.prev_vehicle + 4, cheatClient->VehicleCheat.CarChanger.prev_angle);
						((void (*)(int))(libAddr + string2Offset(OBFUSCATE("0x3169C9"))))(cheatClient->VehicleCheat.CarChanger.prev_vehicle + 4);
						
					cheatClient->VehicleCheat.CarChanger.entered = false;
					cheatClient->VehicleCheat.CarChanger.numpad.clear();
					
					// Помещаем игрока в тачку
					((void (*)(int,int,int))(libAddr + string2Offset(OBFUSCATE("0x391085"))))(localped, cheatClient->GetAtVehiclePool(cheatClient->VehicleCheat.CarChanger.prev_network_id), 0);
					
					// Установка камеры на игрока
					*(char *)(libAddr + 0x8264BA) = 0;
				((void (*)(int))(libAddr + 0x2A6B66 + 1))(libAddr + 0x825AB8);
				((void (*)(int))(libAddr + 0x2AA6C8 + 1))(libAddr + 0x825AB8);
				((void (*)(int))(libAddr + 0x5D9E1C + 1))(libAddr + 0x825AB8);

					cheatClient->VehicleCheat.CarChanger.removedFromVehicle = false;
				}
				else if(localplayer.GetVehicle())
				{
					CVehicle vehClass = CVehicle(localplayer.GetVehicle());
					if(vehClass.GetDriver() != localped)
					{
						unsigned short* structure = new unsigned short[13];
						*(int *)(structure + 1) = std::stoi(cheatClient->VehicleCheat.CarChanger.numpad);
						*structure = cheatClient->VehicleCheat.CarChanger.prev_network_id;
						structure[11] = 65535;
						*((float *)structure + 6) = cheatClient->VehicleCheat.CarChanger.prev_health;
							
						*(int *)(structure + 3) = *(int *)(localped + string2Offset(OBFUSCATE("0x34")));
						*((int *)(structure + 3) + 1) = *(int *)(localped + string2Offset(OBFUSCATE("0x38")));
						*((int *)(structure + 3) + 2) = *(int *)(localped + string2Offset(OBFUSCATE("0x3C")));
						
						CVehiclePool_New(cheatClient->GetVehiclePool(), structure);
						delete[] structure;
						
						// Выравниваем угол машины как было до этого
						((void (*)(int,float))(libAddr + string2Offset(OBFUSCATE("0x316C37"))))(cheatClient->VehicleCheat.CarChanger.prev_vehicle + 4, cheatClient->VehicleCheat.CarChanger.prev_angle);
						((void (*)(int))(libAddr + string2Offset(OBFUSCATE("0x3169C9"))))(cheatClient->VehicleCheat.CarChanger.prev_vehicle + 4);
						
						cheatClient->VehicleCheat.CarChanger.entered = false;
						cheatClient->VehicleCheat.CarChanger.numpad.clear();
						
						// Помещаем игрока в тачку
						((void (*)(int,int,int))(libAddr + string2Offset(OBFUSCATE("0x391085"))))(localped, cheatClient->GetAtVehiclePool(cheatClient->VehicleCheat.CarChanger.prev_network_id), 0);
						
						// Установка камеры на игрока
						*(char *)(libAddr + 0x8264BA) = 0;
				((void (*)(int))(libAddr + 0x2A6B66 + 1))(libAddr + 0x825AB8);
				((void (*)(int))(libAddr + 0x2AA6C8 + 1))(libAddr + 0x825AB8);
				((void (*)(int))(libAddr + 0x5D9E1C + 1))(libAddr + 0x825AB8);

						cheatClient->VehicleCheat.CarChanger.removedFromVehicle = false;
					}
				}
			}
		}

		if(cheatClient->VehicleCheat.AntiDetachTrailer.state)
		{
			uintptr_t truck = localplayer.GetVehicle();
			if(truck)
			{
				CVehicle _truck = CVehicle(truck);
				if(_truck.GetDriver() == localped)
				{
					if(*(int *)(truck + 696) != TRAILER)
					{
						uintptr_t trailer = *(uintptr_t *)(truck + string2Offset(OBFUSCATE("0x21C")));
						if(trailer)
						{
							cheatClient->VehicleCheat.AntiDetachTrailer.trailer = trailer;
						}
						else if(!trailer && cheatClient->VehicleCheat.AntiDetachTrailer.trailer)
						{
							int rw_object = *(int *)(truck + string2Offset(OBFUSCATE("0x44")));
							float truck_angle = atan2f(*(float *)(rw_object + 16), *(float *)(rw_object + 20));
							
							//((void (*)(int,float))(libAddr + 0x1AA01A + 1))(cheatClient->VehicleCheat.AntiDetachTrailer.trailer + 4, truck_angle);
							//((void (*)(int))(libAddr + 0x1A9EEE + 1))(cheatClient->VehicleCheat.AntiDetachTrailer.trailer + 4);
					
							((void (*)(int,int,int))(libAddr + string2Offset(OBFUSCATE("0x2BA95F")) + 1))(cheatClient->VehicleCheat.AntiDetachTrailer.trailer, truck, 1);
						}
					}
				}
			}
		}
		
		if(cheatClient->Wallhack.Player.skeleton)
		{
			if(cheatClient->Wallhack.Player.Config.Skeleton.rendered)
			{
				for(int a = 0; a < 1500; a++)
				{
					uintptr_t player = cheatClient->GetAtPlayerPool(a);
					if(player)
					{
						player = *(int *)(player + 4);
						if(player)
						{
							Vec3 worldPos1 = {0,0,0};
							Vec3 worldPos2 = {0,0,0};
					
							Vec3 screenPos1 = {0,0,0};
							Vec3 screenPos2 = {0,0,0};
					
							int t[14] = { 3, 4, 5, 51, 52, 41, 42, 31, 32, 33, 21, 22, 23, 2 };
                    		for (int i = 0; i < 14; i++)
                    		{
                       			int bone1 = GetBonePosition(player, t[i]);
                        		int bone2 = GetBonePosition(player, t[i] + 1);
								
								if(bone1 && bone2)
								{
									worldPos1 = {*(float *)(bone1 + 48), *(float *)(bone1 + 52), *(float *)(bone1 + 56)};
									worldPos2 = {*(float *)(bone2 + 48), *(float *)(bone2 + 52), *(float *)(bone2 + 56)};
									
									if(WorldToScreen(&worldPos1, &screenPos1))
									{
										if(screenPos1.z < 1.0f) continue;
										if(WorldToScreen(&worldPos2, &screenPos2))
										{
											if(screenPos2.z < 1.0f) continue;
											DrawBuffer draw_element = {ImVec2(screenPos1.x, screenPos1.y), ImVec2(screenPos2.x, screenPos2.y)};
											cheatClient->Wallhack.Player.Config.Skeleton.buffer.push_back(draw_element);
											//AddChatMessage("added element");
											//ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screenPos1.x,screenPos1.y), ImVec2(screenPos2.x,screenPos2.y), ImGui::ColorConvertFloat4ToU32(cheatClient->Wallhack.Player.Config.Skeleton.color), cheatClient->Wallhack.Player.Config.Skeleton.tickness);
										}
									}
								}
							}
					
							for (int i = 4; i < 6; i++)
                    		{
                        		int bone = GetBonePosition(player, i * 10 + 1);
								
								if(bone)
								{
									worldPos2 = {*(float *)(bone + 48), *(float *)(bone + 52), *(float *)(bone + 56)};
									if(WorldToScreen(&worldPos2, &screenPos2))
									{
										if(screenPos2.z < 1.0f) continue;
										DrawBuffer draw_element = {ImVec2(screenPos1.x, screenPos1.y), ImVec2(screenPos2.x, screenPos2.y)};
										cheatClient->Wallhack.Player.Config.Skeleton.buffer.push_back(draw_element);
										//AddChatMessage("added element 2");
									}
								}
                        	}
						}
					}
				}
				cheatClient->Wallhack.Player.Config.Skeleton.rendered = false;
			}
		}
		
		if(cheatClient->Collision.ped)
		{
			for(int i = 0; i < 1500; i++)
			{
				uintptr_t player = cheatClient->GetAtPlayerPool(i);
				if(player)
				{
					player = *(int *)(player + 4);
					if(player)
					{
						CPed _player = CPed(player);
						if(_player.InVehicle()) { continue; }
						
						Vec3 local_coors = cheatClient->GetPos(localped);
						Vec3 ped_coors = cheatClient->GetPos(player);
						
						float dist = Get2dDistance(local_coors, ped_coors);
						
						float radius = 0.8f;
						
						if(localplayer.InVehicle())
						{
							int colmodel = *(int *)(*(int *)(libAddr + string2Offset(OBFUSCATE("0x496F61C")) + (4 * *(unsigned short *)(localplayer.GetVehicle() + string2Offset(OBFUSCATE("0x5A"))))) + 28);
							if(colmodel)
							{
								float forward_width = (float)(*(float *)(colmodel + 32) - *(float *)(colmodel + 20));
								dist = Get2dDistance(cheatClient->GetPos(localplayer.GetVehicle()), ped_coors);
								if(dist < forward_width)
								{
									int number = *(int *)(player + 80);
									number &= ~string2Offset(OBFUSCATE("0x100"));
									*(int *)(player + 80) = number;
								}
								else
								{
									int number = *(int *)(player + 80);
									number |= string2Offset(OBFUSCATE("0x100"));
									*(int *)(player + 80) = number;
								}
							}
						}
						else
						{
							if(dist < radius)
							{
								int number = *(int *)(player + 80);
								number &= ~string2Offset(OBFUSCATE("0x100"));
								*(int *)(player + 80) = number;
							}
							else
							{
								int number = *(int *)(player + 80);
								number |= string2Offset(OBFUSCATE("0x100"));
								*(int *)(player + 80) = number;
							}
						}
					}
				}
			}
		}
		else
		{
			for(int i = 0; i < 1500; i++)
			{
				uintptr_t player = cheatClient->GetAtPlayerPool(i);
				if(player)
				{
					player = *(int *)(player + 4);
					if(player)
					{
						CPed _player = CPed(player);
						if(_player.InVehicle()) { 
							int number = *(int *)(player + 80);
							number &= ~string2Offset(OBFUSCATE("0x100"));
							*(int *)(player + 80) = number;
						}
						else
						{
							int number = *(int *)(player + 80);
							number |= string2Offset(OBFUSCATE("0x100"));
							*(int *)(player + 80) = number;
						}
					}
				}
			}
		}
		
		if(cheatClient->Collision.vehicle)
		{
			for(int i = 0; i < 1500; i++)
			{
				uintptr_t vehicle = cheatClient->GetAtVehiclePool(i);
				if(vehicle)
				{
					if(vehicle == localplayer.GetVehicle()) {
						int number = *(int *)(vehicle + 80);
						number |= string2Offset(OBFUSCATE("0x100"));
						*(int *)(vehicle + 80) = number;
						continue;
					}
					
					Vec3 local_coors = cheatClient->GetPos(localped);
					Vec3 veh_coors = cheatClient->GetPos(vehicle);
						
					float dist = Get2dDistance(local_coors, veh_coors);
					
					if(localplayer.InVehicle())
					{
						int local_colmodel = *(int *)(*(int *)(libAddr + string2Offset(OBFUSCATE("0x496F61C")) + (4 * *(unsigned short *)(localplayer.GetVehicle() + string2Offset(OBFUSCATE("0x5A"))))) + 28);
						int target_colmodel = *(int *)(*(int *)(libAddr + string2Offset(OBFUSCATE("0x496F61C")) + (4 * *(unsigned short *)(vehicle + string2Offset(OBFUSCATE("0x5A"))))) + 28);
						
						if(local_colmodel && target_colmodel)
						{
							float my_forward_width = (float)(*(float *)(local_colmodel + 32) - *(float *)(local_colmodel + 20));
							float target_forward_width = (float)(*(float *)(target_colmodel + 32) - *(float *)(target_colmodel + 20));
							
							dist = Get2dDistance(cheatClient->GetPos(localplayer.GetVehicle()), veh_coors);
							if(dist < my_forward_width + target_forward_width)
							{
								int number = *(int *)(vehicle + 80);
								number &= ~string2Offset(OBFUSCATE("0x100"));
								*(int *)(vehicle + 80) = number;
							}
							else
							{
								int number = *(int *)(vehicle + 80);
								number |= string2Offset(OBFUSCATE("0x100"));
								*(int *)(vehicle + 80) = number;
							}
						}
					}
					else
					{
						int target_colmodel = *(int *)(*(int *)(libAddr + string2Offset(OBFUSCATE("0x496F61C")) + (4 * *(unsigned short *)(vehicle + string2Offset(OBFUSCATE("0x5A"))))) + 28);
						
						if(target_colmodel)
						{
							float target_forward_width = (float)(*(float *)(target_colmodel + 32) - *(float *)(target_colmodel + 20));
							
							if(dist < target_forward_width + 0.8f)
							{
								int number = *(int *)(vehicle + 80);
								number &= ~string2Offset(OBFUSCATE("0x100"));
								*(int *)(vehicle + 80) = number;
							}
							else
							{
								int number = *(int *)(vehicle + 80);
								number |= string2Offset(OBFUSCATE("0x100"));
								*(int *)(vehicle + 80) = number;
							}
						}
					}
				}
			}
		}
		else
		{
			for(int i = 0; i < 1500; i++)
			{
				uintptr_t vehicle = cheatClient->GetAtVehiclePool(i);
				if(vehicle)
				{
					int number = *(int *)(vehicle + 80);
					number |= string2Offset(OBFUSCATE("0x100"));
					*(int *)(vehicle + 80) = number;
				}
			}
		}
		
		if(cheatClient->CarSpawn.entered)
		{
			uintptr_t vehpool = cheatClient->GetVehiclePool();
			uintptr_t localped = cheatClient->FindLocalPlayer();
						
			if(vehpool)
			{
					unsigned short* structure = new unsigned short[13];
					*(int *)(structure + 1) = std::stoi(cheatClient->CarSpawn.numpad);
					*structure = 999;
					structure[11] = 65535;
					*((float *)structure + 6) = 1000.f;
								
					*(int *)(structure + 3) = *(int *)(localped + string2Offset(OBFUSCATE("0x34")));
					*((int *)(structure + 3) + 1) = *(int *)(localped + string2Offset(OBFUSCATE("0x38")));
					*((int *)(structure + 3) + 2) = *(int *)(localped + string2Offset(OBFUSCATE("0x3C")));
						
					CVehiclePool_New(vehpool, structure);
					delete[] structure;
								
					int vehicle = CVehiclePool_GetAt(cheatClient->GetVehiclePool(), 999);
					if(vehicle)
					{
						// sub_28D808
						/**(char *)(vehicle + 0x7A8) = -1;
						*(int *)(vehicle + 0x7A4) = 0;
						*(int *)(vehicle + 0x794) = -16777216;
						*(int *)(vehicle + 0x798) = 0;
						*(int *)(vehicle + 0x79C) = 0;
						*(int *)(vehicle + 0x7A0) = 0;
						*/
						((void (*) (int,int,int,int,int))(libAddr + string2Offset(OBFUSCATE("0x45D485"))))(vehicle, -16777216, 0, 0, 0);
						// цвет кузова
						*(char *)(vehicle + string2Offset(OBFUSCATE("0x7D8"))) = -1;
						*(char *)(vehicle + string2Offset(OBFUSCATE("0x7D9"))) = 0;
						*(char *)(vehicle + string2Offset(OBFUSCATE("0x7D8"))) = 0;
								
						// цвет дисков
						*(char *)(vehicle + string2Offset(OBFUSCATE("0x7E0"))) = 0;
						*(char *)(vehicle + string2Offset(OBFUSCATE("0x7E1"))) = -1;
						*(char *)(vehicle + string2Offset(OBFUSCATE("0x7E2"))) = 0;
						
						*(float *)(vehicle + string2Offset(OBFUSCATE("0x34"))) = *(float *)(localped + string2Offset(OBFUSCATE("0x34")));
						*(float *)(vehicle + string2Offset(OBFUSCATE("0x38"))) = *(float *)(localped + string2Offset(OBFUSCATE("0x38")));
						*(float *)(vehicle + string2Offset(OBFUSCATE("0x3C"))) = *(float *)(localped + string2Offset(OBFUSCATE("0x3C")));
						
						// unknown
						/*int v13 = *(int *)(vehicle + 0x12C);
								
						// общий клиренс
						*(float *)(vehicle + 0x778) = 1.f;
								
						// дальний свет
						*(char *)(vehicle + 0x2BF) = 0;
						
						// отдельный клиренс
						*(float *)(vehicle + 0x77C) = 1.f;
								
						// цвет фар
						*(char *)(vehicle + 0x7AC) = 0;
						*(char *)(vehicle + 0x7AD) = -1;
						*(char *)(vehicle + 0x7AE) = 0;
								
						// unused
						*(int *)(vehicle + 0x790) = 0.005f;
								
						*(char *)(vehicle + 0x774) = 0;
						*(char *)(vehicle + 0x78C) = 0;
						*(float *)(vehicle + 0x780) = 0;
						*(float *)(vehicle + 0x784) = 0.0004f;
						*(int *)(vehicle + 0x788) = -16777216;
						*(float *)(vehicle + 0x7C0) = 1.f;
						*(float *)(vehicle + 0x7C4) = 1.f;
						*(int *)(vehicle + 0x7B8) = -16777216;
						*(int *)(vehicle + 0x7BC) = -16777216;
						*(int *)(vehicle + 0x7B0) = 0;
						*(int *)(vehicle + 0x7C8) = 0;
								
						*(int *)(vehicle + 0x7E8) = 0;
						*(int *)(vehicle + 0x7EC) = 0;
						*(int *)(vehicle + 0x7F0) = 0;
								
						*(char *)(vehicle + 0x75C) = 0;
						*(short *)(vehicle + 0x75E) = 0;
								
						*(char *)(vehicle + 0x814) = 0;
						*(char *)(vehicle + 0x7CC) = 0;
						*/
						*(char *)(vehicle + string2Offset(OBFUSCATE("0x20D"))) = 16;
						*(char *)(vehicle + string2Offset(OBFUSCATE("0x212"))) = 1;
						
						cheatClient->CarSpawn.entered = false;
						cheatClient->CarSpawn.numpad.clear();
				}
			}
		}
		
		if(cheatClient->VisualCheat.SkinChanger.entered)
		{
			int v6 = 1001;
			int skinID = std::stoi(cheatClient->VisualCheat.SkinChanger.numpad);
			int localped = cheatClient->FindLocalPlayer();
			
			if(localped)
			{
				if(((int *)(cheatClient->GetLibAddr() + string2Offset(OBFUSCATE("0x496F61C"))))[skinID])
				{
					if(*((unsigned char*)(cheatClient->GetLibAddr() + string2Offset(OBFUSCATE("0x459E298")) + 28 * skinID + 8)) == 1)
					{
						(( void (*)(int))(cheatClient->GetLibAddr() + string2Offset(OBFUSCATE("0x20CDA7"))))(localped);
						*(int *)(localped + 180) = -1;
						(( void (*)(int, int))(cheatClient->GetLibAddr() + string2Offset(OBFUSCATE("0x20CC59"))))(localped, skinID);
						cheatClient->VisualCheat.SkinChanger.entered = false;
						cheatClient->VisualCheat.SkinChanger.numpad.clear();
					}
					else
					{
						// Загружаем модель скина
						// в отдельном потоке
						while(true)
						{
							(( void (*)(int, int))(cheatClient->GetLibAddr() + string2Offset(OBFUSCATE("0x2E712D"))))(skinID, 8);
							(( void (*)(int))(cheatClient->GetLibAddr() + string2Offset(OBFUSCATE("0x2E99FD"))))(1);
							v6 -= 1;
							if(!v6)
							{
								break;
							}
							if(*((unsigned char*)(cheatClient->GetLibAddr() + string2Offset(OBFUSCATE("0x459E298")) + 28 * skinID + 8)) == 1)
							{
								(( void (*)(int))(cheatClient->GetLibAddr() + string2Offset(OBFUSCATE("0x20CDA7"))))(localped);
								*(int *)(localped + 180) = -1;
								(( void (*)(int, int))(cheatClient->GetLibAddr() + string2Offset(OBFUSCATE("0x20CC59"))))(localped, skinID);
								cheatClient->VisualCheat.SkinChanger.entered = false;
								cheatClient->VisualCheat.SkinChanger.numpad.clear();
								break;
							}
						}
					}
				}
				else
				{
					AddChatMessage(OBFUSCATE("{ff5252}[ONESHOT] |{ffffff}Скин не найден."));
				}
			}
		}
		
		if(cheatClient->VisualCheat.MoneyChanger.entered)
		{
			int localped;
			int* result;
	
			localped = cheatClient->FindLocalPlayer();
			result = (( int* (*)(int))(cheatClient->GetLibAddr() + 0x3B0A0C + 1))(localped);
			result[20] += std::stoi(cheatClient->VisualCheat.MoneyChanger.numpad);
			char buff[32];
			sprintf(buff, OBFUSCATE("Вы получили %d рублей"), std::stoi(cheatClient->VisualCheat.MoneyChanger.numpad));
			std::string str(buff);
			BrNotification(TYPE_MONEY_GREEN, str.c_str(), 3);
			
			cheatClient->VisualCheat.MoneyChanger.entered = false;
			cheatClient->VisualCheat.MoneyChanger.numpad.clear();
		}
		
		if(cheatClient->Teleport.toMarker)
		{
			// InCar
			if(localplayer.InVehicle())
			{
				CVehicle vehicle(localplayer.GetVehicle());
				if(vehicle.GetDriver() == localped)
				{
					// InCar
					// Установка камеры на игрока
					*(char *)(libAddr + 0x8264BA) = 0;
				((void (*)(int))(libAddr + 0x2A6B66 + 1))(libAddr + 0x825AB8);
				((void (*)(int))(libAddr + 0x2AA6C8 + 1))(libAddr + 0x825AB8);
				((void (*)(int))(libAddr + 0x5D9E1C + 1))(libAddr + 0x825AB8);

					// Направляем игрока к маркеру
					Vec3 local_coors = cheatClient->GetPos(localplayer.GetVehicle());
					Vec2 marker_coors = cheatClient->GetMarkerCoors();
					*(float *)(localplayer.GetVehicle() + string2Offset(OBFUSCATE("0x3C"))) += 0.5f;

					Vec3 cam_coors = cheatClient->GetCameraCoors();
					
					float warpX = (marker_coors.x - local_coors.x);
					float warpY = (marker_coors.y - local_coors.y);
					float length_vector = sqrt(warpX * warpX + warpY * warpY);
					warpX = warpX / length_vector;
					warpY = warpY / length_vector;
					float warpZ = local_coors.z - cam_coors.z;
					cheatClient->Teleport.InCar.speed += 0.1f;
					if(cheatClient->Teleport.InCar.speed > cheatClient->Teleport.InCar.speed_max) {
						cheatClient->Teleport.InCar.speed = cheatClient->Teleport.InCar.speed_max;
					}
					*(float *)(localplayer.GetVehicle() + string2Offset(OBFUSCATE("0x7C"))) = warpX * (cheatClient->Teleport.InCar.speed * 0.01f);
					*(float *)(localplayer.GetVehicle() + string2Offset(OBFUSCATE("0x80"))) = warpY * (cheatClient->Teleport.InCar.speed * 0.01f);
					*(float *)(localplayer.GetVehicle() + string2Offset(OBFUSCATE("0x84"))) = 0.f;
					
					Vec3 marker_coorss = {marker_coors.x,marker_coors.y,20.f};
					
					float distToMarker = Get2dDistance(local_coors, marker_coorss);
					
					if(distToMarker < 40.f)
					{
						cheatClient->Teleport.InCar.speed_max = 100.f;
					}

					if(distToMarker < 5.f)
					{
						while(true)
						{
							marker_coorss.z = FindGroundZ(marker_coors.x, marker_coors.y);
							if(marker_coorss.z != 20.f)
							{
								break;
							}
						}
						marker_coorss.z += 1.5f;
						cheatClient->SetPos(localplayer.GetVehicle(), marker_coorss);
						//((void (*)(int))(libAddr + 0x20DCF4 + 1))(localped);
						cheatClient->Teleport.toMarker = false;
						cheatClient->Teleport.InCar.speed = 0;
						cheatClient->Teleport.InCar.speed_max = 100.f;
					}
				}
				else
				{
					// Stop teleport
					cheatClient->Teleport.toMarker = false;
					BrNotification(TYPE_TEXT_RED, OBFUSCATE("Вы не можете телепортироваться на пассажирском месте!"), 5);
				}
			}
			else
			{
				// OnFoot
				// Установка камеры на игрока
				*(char *)(libAddr + 0x8264BA) = 0;
				((void (*)(int))(libAddr + 0x2A6B66 + 1))(libAddr + 0x825AB8);
				((void (*)(int))(libAddr + 0x2AA6C8 + 1))(libAddr + 0x825AB8);
				((void (*)(int))(libAddr + 0x5D9E1C + 1))(libAddr + 0x825AB8);

				// Направляем игрока к маркеру
				Vec3 local_coors = cheatClient->GetPos(localped);
				Vec2 marker_coors = cheatClient->GetMarkerCoors();
				*(float *)(localped + string2Offset(OBFUSCATE("0x3C"))) += 0.5f;

				Vec3 cam_coors = cheatClient->GetCameraCoors();
				
				float warpX = (marker_coors.x - local_coors.x);
				float warpY = (marker_coors.y - local_coors.y);
				float length_vector = sqrt(warpX * warpX + warpY * warpY);
				warpX = warpX / length_vector;
				warpY = warpY / length_vector;
				float warpZ = local_coors.z - cam_coors.z;
				cheatClient->Teleport.OnFoot.speed += 0.1f;
				if(cheatClient->Teleport.OnFoot.speed > cheatClient->Teleport.OnFoot.speed_max) {
					cheatClient->Teleport.OnFoot.speed = cheatClient->Teleport.OnFoot.speed_max;
				}
				*(float *)(localped + 0x7C) = warpX * (cheatClient->Teleport.OnFoot.speed * 0.01f);
				*(float *)(localped + 0x80) = warpY * (cheatClient->Teleport.OnFoot.speed * 0.01f);
				*(float *)(localped + 0x84) = 0.f;
				
				Vec3 marker_coorss = {marker_coors.x,marker_coors.y,20.f};
				
				float distToMarker = Get2dDistance(local_coors, marker_coorss);
				
				if(distToMarker < 40.f)
				{
					cheatClient->Teleport.OnFoot.speed_max = 35.f;
				}

				if(distToMarker < 5.f)
				{
					while(true)
					{
						marker_coorss.z = FindGroundZ(marker_coors.x, marker_coors.y);
						if(marker_coorss.z != 20.f)
						{
							break;
						}
					}
					marker_coorss.z += 1.5f;
					cheatClient->SetPos(localped, marker_coorss);
					//((void (*)(int))(libAddr + 0x20DCF4 + 1))(localped);
					cheatClient->Teleport.toMarker = false;
					cheatClient->Teleport.OnFoot.speed = 0;
					cheatClient->Teleport.OnFoot.speed_max = 50.f;
				}
			}
		}

		if(cheatClient->Teleport.toCheckpoint)
		{
			// InCar
			if(localplayer.InVehicle())
			{
				CVehicle vehicle(localplayer.GetVehicle());
				if(vehicle.GetDriver() == localped)
				{
					// InCar
					// Установка камеры на игрока
					*(char *)(libAddr + 0x8264BA) = 0;
				((void (*)(int))(libAddr + 0x2A6B66 + 1))(libAddr + 0x825AB8);
				((void (*)(int))(libAddr + 0x2AA6C8 + 1))(libAddr + 0x825AB8);
				((void (*)(int))(libAddr + 0x5D9E1C + 1))(libAddr + 0x825AB8);

					// Направляем игрока к маркеру
					Vec3 local_coors = cheatClient->GetPos(localplayer.GetVehicle());
					Vec3 marker_coors = cheatClient->GetCheckpointCoors();
					*(float *)(localplayer.GetVehicle() + 0x3C) += 0.5f;

					Vec3 cam_coors = cheatClient->GetCameraCoors();
					
					float warpX = (marker_coors.x - local_coors.x);
					float warpY = (marker_coors.y - local_coors.y);
					float length_vector = sqrt(warpX * warpX + warpY * warpY);
					warpX = warpX / length_vector;
					warpY = warpY / length_vector;
					float warpZ = local_coors.z - cam_coors.z;
					cheatClient->Teleport.InCar.speed += 0.1f;
					if(cheatClient->Teleport.InCar.speed > cheatClient->Teleport.InCar.speed_max) {
						cheatClient->Teleport.InCar.speed = cheatClient->Teleport.InCar.speed_max;
					}
					*(float *)(localplayer.GetVehicle() + 0x7C) = warpX * (cheatClient->Teleport.InCar.speed * 0.01f);
					*(float *)(localplayer.GetVehicle() + 0x80) = warpY * (cheatClient->Teleport.InCar.speed * 0.01f);
					*(float *)(localplayer.GetVehicle() + 0x84) = 0.f;
					
					Vec3 marker_coorss = {marker_coors.x,marker_coors.y,20.f};
					
					float distToMarker = Get2dDistance(local_coors, marker_coorss);
					
					if(distToMarker < 40.f)
					{
						cheatClient->Teleport.InCar.speed_max = 100.f;
					}

					if(distToMarker < 5.f)
					{
						while(true)
						{
							marker_coorss.z = FindGroundZ(marker_coors.x, marker_coors.y);
							if(marker_coorss.z != 20.f)
							{
								break;
							}
						}
						marker_coorss.z += 1.5f;
						cheatClient->SetPos(localplayer.GetVehicle(), marker_coorss);
						//((void (*)(int))(libAddr + 0x20DCF4 + 1))(localped);
						cheatClient->Teleport.toCheckpoint = false;
						cheatClient->Teleport.InCar.speed = 0;
						cheatClient->Teleport.InCar.speed_max = 300.f;
					}
				}
				else
				{
					// Stop teleport
					cheatClient->Teleport.toCheckpoint = false;
					BrNotification(TYPE_TEXT_RED, OBFUSCATE("Вы не можете телепортироваться на пассажирском месте!"), 5);
				}
			}
			else
			{
				// OnFoot
				// Установка камеры на игрока
				*(char *)(libAddr + 0x8264BA) = 0;
				((void (*)(int))(libAddr + 0x2A6B66 + 1))(libAddr + 0x825AB8);
				((void (*)(int))(libAddr + 0x2AA6C8 + 1))(libAddr + 0x825AB8);
				((void (*)(int))(libAddr + 0x5D9E1C + 1))(libAddr + 0x825AB8);

				// Направляем игрока к маркеру
				Vec3 local_coors = cheatClient->GetPos(localped);
				Vec3 marker_coors = cheatClient->GetCheckpointCoors();
				*(float *)(localped + 0x3C) += 0.5f;

				Vec3 cam_coors = cheatClient->GetCameraCoors();
				
				float warpX = (marker_coors.x - local_coors.x);
				float warpY = (marker_coors.y - local_coors.y);
				float length_vector = sqrt(warpX * warpX + warpY * warpY);
				warpX = warpX / length_vector;
				warpY = warpY / length_vector;
				float warpZ = local_coors.z - cam_coors.z;
				cheatClient->Teleport.OnFoot.speed += 0.1f;
				if(cheatClient->Teleport.OnFoot.speed > cheatClient->Teleport.OnFoot.speed_max) {
					cheatClient->Teleport.OnFoot.speed = cheatClient->Teleport.OnFoot.speed_max;
				}
				*(float *)(localped + 0x7C) = warpX * (cheatClient->Teleport.OnFoot.speed * 0.01f);
				*(float *)(localped + 0x80) = warpY * (cheatClient->Teleport.OnFoot.speed * 0.01f);
				*(float *)(localped + 0x84) = 0.f;
				
				Vec3 marker_coorss = {marker_coors.x,marker_coors.y,20.f};
				
				float distToMarker = Get2dDistance(local_coors, marker_coorss);
				
				if(distToMarker < 40.f)
				{
					cheatClient->Teleport.OnFoot.speed_max = 35.f;
				}

				if(distToMarker < 5.f)
				{
					while(true)
					{
						marker_coorss.z = FindGroundZ(marker_coors.x, marker_coors.y);
						if(marker_coorss.z != 20.f)
						{
							break;
						}
					}
					marker_coorss.z += 1.5f;
					cheatClient->SetPos(localped, marker_coorss);
					//((void (*)(int))(libAddr + 0x20DCF4 + 1))(localped);
					cheatClient->Teleport.toCheckpoint = false;
					cheatClient->Teleport.OnFoot.speed = 0;
					cheatClient->Teleport.OnFoot.speed_max = 50.f;
				}
			}
		}
	}
}

void SetVehicleVelocityRPC(int a1)
{
	if(cheatClient != nullptr)
	{
		if(cheatClient->NOPs.vehicleVelocity)
		{
			return;
		}
		else
		{
			old_SetVehicleVelocityRPC(a1);
		}
	}
	else
	{
		old_SetVehicleVelocityRPC(a1);
	}
}

void CPlayerBubbleChatPool_New(int playerbubblepool, int playerId, char* szText, int color, float duration, int drawDistance)
{
	if(cheatClient->Wallhack.Player.nametags)
	{
		old_CPlayerBubbleChatPool_New(playerbubblepool, playerId, szText, color, 2000.f, drawDistance);
		return;
	}
	else
	{
		old_CPlayerBubbleChatPool_New(playerbubblepool, playerId, szText, color, duration, drawDistance);
		return;
	}
}

void ScrServerJoin(int a1)
{
	cheatClient->ServerPlayersCount++;
	old_ScrServerJoin(a1);
}

void ScrServerQuit(int a1)
{
	cheatClient->ServerPlayersCount--;
	old_ScrServerQuit(a1);
}

void ScrConnectionRejected()
{
	cheatClient->ServerPlayersCount = 1;
	for(int i = 0; i < 1500; i++)
	{
		cheatClient->ServerPlayers[i] = {0};
	}
	
	old_ScrConnectionRejected();
}

void ScrServerNewPlayer(int pool, int nickname, int playerID)
{
	ServerPlayer player;
	player.id = playerID;
	strncpy(player.nickname, (const char*)nickname, 25);
	cheatClient->ServerPlayers[playerID] = player;
	old_ScrServerNewPlayer(pool, nickname, playerID);
}
 
void ScrInitGame(RPCParameters* rpcParams)
{
	cheatClient->GameServer.sender = rpcParams->sender;
	cheatClient->GameServer.recipient = rpcParams->recipient;
	cheatClient->GameServer.replyToSender = rpcParams->replyToSender;
	
	for(int i = 0; i < 1500; i++)
	{
		cheatClient->ServerPlayers[i].id = -1;
	}
	
	old_ScrInitGame(rpcParams);
}

void ScrRequestSpawn()
{
	old_ScrRequestSpawn();
}

void ScrSetSpawnInfo(int a1)
{
	old_ScrSetSpawnInfo(a1);
}

void LocalSetSpawnInfo(int *localped, int *spawnInfo)
{
	old_LocalSetSpawnInfo(localped,spawnInfo);
}

void WorldVehicleDelete(RPCParameters* rpcParams)
{
	old_WorldVehicleDelete(rpcParams);
}

int GetIsLineOfSightClear(int origin, int target, int buildings, int vehicles, int peds, int objects, int dummies, int doSeeThroughCheck, int doCameraIgnoreCheck)
{
	if(cheatClient != nullptr)
	{
		if(cheatClient->Wallhack.Player.nametags)
		{
			return old_GetIsLineOfSightClear(origin, target, 0, 0, 0, 0, 0, 0, 0);
		}
		else
		{
			return old_GetIsLineOfSightClear(origin, target, buildings, vehicles, peds, objects, dummies, doSeeThroughCheck, doCameraIgnoreCheck);
		}
	}
	return old_GetIsLineOfSightClear(origin, target, buildings, vehicles, peds, objects, dummies, doSeeThroughCheck, doCameraIgnoreCheck);
}

void CSprintCalculator(int* result)
{
	if(cheatClient != nullptr)
	{
		if(cheatClient->PlayerCheat.Base.Invulnerability.endurance)
		{
			return;
		}
	}
	
	old_CSprintCalculator(result);
}

void CPedDamageCalculator(int a1, int a2, unsigned int a3, float a4, int a5, int a6)
{
	if(cheatClient != nullptr)
	{
		if(cheatClient->PlayerCheat.Base.Invulnerability.health)
		{
			old_CPedDamageCalculator(a1, a2, a3, 0.f, a5, a6);
			return;
		}
	}
	
	old_CPedDamageCalculator(a1, a2, a3, a4, a5, a6);
}

void CVehicleDamageCalculator(int a1, float a2, int a3)
{
	if(cheatClient != nullptr)
	{
		if(cheatClient->VehicleCheat.Invulnerability.health)
		{
			return;
		}
	}
	
	old_CVehicleDamageCalculator(a1, a2, a3);
}

void NoBikeCalculator(int a1, int a2, int a3, int a4, int a5)
{
	if(cheatClient != nullptr)
	{
		if(cheatClient->PlayerCheat.Base.Invulnerability.noBike)
		{
			return;
		}
	}
	
	old_NoBikeCalculator(a1,a2,a3,a4,a5);
}

int WorldToScreen(Vec3* pos, Vec3* output)
{
	char temp1[4];
	char temp2[4];
	
	if(((int (*)(uintptr_t, float*, float*, int, char))(cheatClient->GetLibAddr() + string2Offset(OBFUSCATE("0x3F3044")) + 1))((uintptr_t)pos, (float *)output, (float *)temp1, (int)temp2, 1) != 0)
	{
		return 1;
	}
	
	return 0;
}

void CPed_ProcessBuoyancy(int ped)
{
	if(cheatClient->PlayerCheat.Base.Invulnerability.walkUnderwater) { return; }
	old_CPed_ProcessBuoyancy(ped);
}

void CBike_ProcessBuoyancy(int bike)
{
	if(cheatClient->VehicleCheat.Invulnerability.driveUnderwater) { return; }
	old_CBike_ProcessBuoyancy(bike);
}

void CAutomobile_ProcessBuoyancy(int vehicle)
{
	if(cheatClient->VehicleCheat.Invulnerability.driveUnderwater) { return; }
	old_CAutomobile_ProcessBuoyancy(vehicle);
}

void log_1(int vehicle_pool, unsigned short* structure)
{
	unsigned short *v4 = structure + 3;
	int x = *(int *)(structure + 3);
	
	/**(float *)(structure + 3) = 0;
	*(float *)(structure + 4) = 0;
	*(float *)(structure + 5) = 10.f;
	
	/*int y = *((int *)v4 + 1);
	int z = *((int *)v4 + 2);*/
	//AddChatMessage("MODEL ID: %d, ID: %d, [11]: %u, HP: %f", *(int *)(structure + 1), *structure, structure[11], *((float *)structure + 6));
	//AddChatMessage("X: %f", x);
	old_log1(vehicle_pool, structure);
}

void log_2(int vehicle, int a2, int a3, int a4, int a5)
{
	//AddChatMessage("Vehicle: %d, a2: %d, a3: %d, a4: %d, a5: %d", vehicle, a2, a3, a4, a5);
	old_log2(vehicle, a2, a3, a4, a5);
}

void log_3(int peer, int *a2, RakNet::BitStream* bitStream, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14)
{
	gpeer = peer;
	ga7 = a7;
	ga8 = a8;
	
	if(*a2 == 115)
	{
		bool takeGive;
		unsigned short playerID;
		float damage;
		int weapon;
		int bone;

		bitStream->Read(takeGive);
    	bitStream->Read(playerID);
    	bitStream->Read(damage);
    	bitStream->Read(weapon);
    	bitStream->Read(bone);														
		
		//AddChatMessage("{ff0000}TakeGiveDamage |{ffffff} TakeGive: %d, PlayerID: %u, Damage: %f, Weapon: %d, Bone: %d",
		//takeGive, playerID, damage, weapon, bone);
	}
	old_log3(peer,a2,bitStream,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14);
}

void calculateNewCoordinates(float& x, float& y, float& z, float x_angle, float y_angle, float z_angle, float forward, float height) {
    // Время вперед (в секундах) и вверх (в секундах)
    float time_forward = forward;
    float time_up = height;
	float left = 2.f;
	
    // Углы наклона машины в радианах
    float x_angle_rad = x_angle;
    float y_angle_rad = y_angle;
    float z_angle_rad = z_angle;

    // Вычисляем новые координаты
    // Для упрощения расчетов, предположим, что машина движется с постоянной скоростью
    float new_x = x + time_forward * cos(z_angle_rad) * cos(y_angle_rad);
    float new_y = y + time_forward * sin(z_angle_rad) * cos(y_angle_rad);
    float new_z = z + time_forward * sin(y_angle_rad);

    // Обновляем координаты
    x = new_x - left * cos(x_angle);
    y = new_y - left * sin(x_angle);
    z = new_z + time_up;
}

void Draw3dBox(int object, ImVec4 color, float tickness)
{
	int colmodel = *(int *)(*(int *)(libAddr + 0x46E6700 + (4 * *(unsigned short *)(object + 0x5A))) + 28);
	if(colmodel)
	{
		int rw_object = *(int *)(object + 0x44);
		
		float object_angle_x = atan2f(*(float *)(rw_object + 36), *(float *)(rw_object + 40));
		float object_angle_y = atan2f(-*(float *)(rw_object + 32), sqrt((*(float *)(rw_object + 36) * *(float *)(rw_object + 36)) + (*(float *)(rw_object + 40) * *(float *)(rw_object + 40))));
		float object_angle_z = atan2f(*(float *)(rw_object + 4), *(float *)(rw_object + 20));
		
		float object_forward_width = (float)(*(float *)(colmodel + 28) - *(float *)(colmodel + 16));
		float object_side_width = (float)(*(float *)(colmodel + 32) - *(float *)(colmodel + 20));
		float object_height = (float)(*(float *)(colmodel + 36) - *(float *)(colmodel + 24));
		
		Vec3 screen_object_pos = Vec3(0,0,0);
		Vec3 object_pos = { *(float *)(object + 0x34) , *(float *)(object + 0x38) , *(float *)(object + 0x3C) };
		Vec3 object_pos_ = object_pos;
		Vec3 forward_up_left_point = Vec3(0,0,0);
		Vec3 forward_up_right_point = Vec3(0,0,0);
		Vec3 forward_down_left_point = Vec3(0,0,0);
		Vec3 forward_down_right_point = Vec3(0,0,0);
		Vec3 back_up_left_point = Vec3(0,0,0);
		Vec3 back_up_right_point = Vec3(0,0,0);
		Vec3 back_down_left_point = Vec3(0,0,0);
		Vec3 back_down_right_point = Vec3(0,0,0);
						
		Vec3 screen_forward_up_left_point = Vec3(0,0,0);
		Vec3 screen_forward_up_right_point = Vec3(0,0,0);
		Vec3 screen_forward_down_left_point = Vec3(0,0,0);
		Vec3 screen_forward_down_right_point = Vec3(0,0,0);
		Vec3 screen_back_up_left_point = Vec3(0,0,0);
		Vec3 screen_back_up_right_point = Vec3(0,0,0);
		Vec3 screen_back_down_left_point = Vec3(0,0,0);
		Vec3 screen_back_down_right_point = Vec3(0,0,0);
		
		float x2 = object_pos.x + (object_forward_width + 0.7f) * cos(object_angle_z);
    	float y2 = object_pos.y + (object_forward_width + 0.7f) * sin(object_angle_z);
		
		calculateNewCoordinates(object_pos.x, object_pos.y, object_pos.z, object_angle_y, object_angle_x, object_angle_z, object_forward_width, object_height);
		//float new_x = object_pos.x + object_forward_width * cos(object_angle_z);
    	//float new_y = object_pos.y + object_forward_width * sin(object_angle_z);
		Vec3 forward_pos = { object_pos.x, object_pos.y, object_pos.z };
		Vec3 screen_forward_pos = {0,0,0};
		
		if(WorldToScreen(&object_pos_, &screen_object_pos))
		{
			if(screen_object_pos.z >= 1.f)
			{
				if(WorldToScreen(&forward_pos, &screen_forward_pos))
				{
					if(screen_forward_pos.z >= 1.f)
					{
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_object_pos.x, screen_object_pos.y), ImVec2(screen_forward_pos.x, screen_forward_pos.y), IM_COL32(255,0,0,255), 5.f);
					}
				}
			}
		}
	}
}
