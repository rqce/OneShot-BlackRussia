#pragma once
#define CLIENT_VER	    "1.4.0"
#define SAMP_VERSION	"0.3.7"
#define MAX_PLAYERS		1004
#define MAX_VEHICLES	2000
#define MAX_PLAYER_NAME	24

#define RAKSAMP_CLIENT
#define NETCODE_CONNCOOKIELULZ 0x6969
#include <unistd.h>
#include <cstring>
#include <stdio.h>

#include "network/raknet/SAMP/samp_netencr.h"
#include "network/raknet/SAMP/SAMPRPC.h"

/*uintptr_t localped = cheatClient->FindLocalPlayer();
	if(localped)
	{
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
					
					Vec3 screen_pos;
					Vec3 pos = { *(float *)(player + 0x34), *(float *)(player + 0x38), *(float *)(player + 0x3C) };
	
					if(WorldToScreen(&pos, &screen_pos))
					{
						if(screen_pos.z >= 1.f)
						{
							float dist = distance(pos.x, pos.y, pos.z, *(float *)(localped + 0x34), *(float*)(localped + 0x38), *(float *)(localped + 0x3C));
							std::string distance_str = to_string(dist);
							//ImGui::GetBackgroundDrawList()->AddText(ImVec2(screen_pos.x, screen_pos.y), IM_COL32_WHITE, distance_str.c_str());
							
							int colmodel = *(int *)(*(int *)(libAddr + 0x46E6700 + (4 * *(unsigned short *)(player + 0x5A))) + 28);
							if(colmodel)
							{
								Vec3 col_model_screen;
								Vec3 col_min_x_pos = { pos.x ,pos.y,pos.z + 0.2f };
								
								if(WorldToScreen(&col_min_x_pos, &col_model_screen))
								{
									if(col_model_screen.z >= 1.f)
									{
										char buff[256];
										sprintf(buff, "Triangles: %d, Coors: %f %f, BBoxXY: %f %f", *(unsigned short *)(colmodel + 46), pos.x, pos.y, (float)(*(float *)(colmodel + 28) - *(float *)(colmodel + 16)), (float)(*(float *)(colmodel + 32) - *(float *)(colmodel + 20)));
										ImGui::GetBackgroundDrawList()->AddText(ImVec2(col_model_screen.x, col_model_screen.y), IM_COL32_WHITE, buff);
									}
								}
							}
						}
					}
				}
			}
		}*/
		/*
		for(int i = 0; i < 1500; i++)
		{
			int playerpool = cheatClient->GetVehiclePool();
			if(!playerpool) { continue; }
			uintptr_t player = CVehiclePool_GetAt(playerpool, i);
			if(player)
			{
				//player = player + 4;
				if(player)
				{
					
					Vec3 screen_pos = Vec3(0,0,0);
					Vec3 pos = { *(float *)(player + 0x34), *(float *)(player + 0x38), *(float *)(player + 0x3C) };
	
					if(WorldToScreen(&pos, &screen_pos))
					{
						if(screen_pos.z >= 1.f)
						{
							float dist = distance(pos.x, pos.y, pos.z, *(float *)(localped + 0x34), *(float*)(localped + 0x38), *(float *)(localped + 0x3C));
							std::string distance_str = to_string(dist);
							//ImGui::GetBackgroundDrawList()->AddText(ImVec2(screen_pos.x, screen_pos.y), IM_COL32_WHITE, distance_str.c_str());
							
							int colmodel = *(int *)(*(int *)(libAddr + 0x46E6700 + (4 * *(unsigned short *)(player + 0x5A))) + 28);
							if(colmodel)
							{
								Vec3 col_model_screen = Vec3(0,0,0);
								Vec3 col_min_x_pos = { pos.x ,pos.y,pos.z + 0.2f };
								
								if(WorldToScreen(&col_min_x_pos, &col_model_screen))
								{
									if(col_model_screen.z >= 1.f)
									{
										char buff[256];
										sprintf(buff, "Triangles: %d, Coors: %f %f, BBoxXYZ: %f %f %f", *(unsigned short *)(colmodel + 46), pos.x, pos.y, (float)(*(float *)(colmodel + 28) - *(float *)(colmodel + 16)), (float)(*(float *)(colmodel + 32) - *(float *)(colmodel + 20)), (float)(*(float *)(colmodel + 36) - *(float *)(colmodel + 24)));
										//ImGui::GetBackgroundDrawList()->AddText(ImVec2(col_model_screen.x, col_model_screen.y), IM_COL32_WHITE, buff);
									}
								}
							}
						}
					}
				}
			}
		}
	}*/