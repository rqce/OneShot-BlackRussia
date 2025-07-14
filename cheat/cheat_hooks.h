#ifndef CHEAT_HOOKS_H
#define CHEAT_HOOKS_H

#include <jni.h>
#include "network/raknet/BitStream.h"
#include "network/raknet/NetworkTypes.h"
#include "vector/vector3.h"

// New Hook Functions

// = Game

extern void CGame_Process();
extern int GetIsLineOfSightClear(int origin, int target, int buildings, int vehicles, int peds, int objects, int dummies, int doSeeThroughCheck, int doCameraIgnoreCheck);
extern void CSprintCalculator(int*);
extern void CPedDamageCalculator(int,int,unsigned int,float,int,int);
extern void CVehicleDamageCalculator(int,float,int);
extern void NoBikeCalculator(int,int,int,int,int);
extern void CPed_ProcessBuoyancy(int ped);
extern void CBike_ProcessBuoyancy(int);
extern void CAutomobile_ProcessBuoyancy(int);
extern void CDraw_SetFOV(float);

// = Samp
extern void ChatWindowInputHandler(char*);
extern void SetVehicleVelocityRPC(int);
extern void CPlayerBubbleChatPool_New(int playerbubblepool, int playerId, char* szText, int color, float drawDistance, int duration);
extern void ScrServerJoin(int);
extern void ScrServerQuit(int);
extern void ScrConnectionRejected();
extern void ScrServerNewPlayer(int pool, int nickname, int playerID);
extern void ScrServerDeletePlayer(int pool, int playerID);
extern void ScrRequestSpawn();
extern void ScrSetSpawnInfo(int);
extern void LocalSetSpawnInfo(int*,int*);
extern void AddChatMessage_hook(char*);
extern void ScrInitGame(RPCParameters*);
extern void WorldVehicleDelete(RPCParameters* rpcParams);

// ================================================================ //

// Before (old) Functions
extern void (*old_Java_com_blackhub_bronline_game_core_JNILib_multiTouchEvent)(JNIEnv* env, jclass cls, int actionMasked, int actionIndex, int x1, int y1, int x2, int y2, int x3, int y3);

// = Game
extern void (*old_CGame_Process)();
extern float (*FindGroundZ)(float,float);
extern int (*CPlayerPool_GetAt)(int pool, unsigned short id);
extern int (*CVehiclePool_GetAt)(int pool, unsigned short id);
extern int (*GetBonePosition)(int, int);
extern int WorldToScreen(Vec3*, Vec3*);
extern void (*SetSeek)(int player, float x, float y, float z, float dist);
extern void (*SetMoveAnim)(int player);
extern int (*old_GetIsLineOfSightClear)(int origin, int target, int buildings, int vehicles, int peds, int objects, int dummies, int doSeeThroughCheck, int doCameraIgnoreCheck);
extern void (*old_CSprintCalculator)(int*);
extern void (*old_CPedDamageCalculator)(int,int,unsigned int,float,int,int);
extern void (*old_CVehicleDamageCalculator)(int,float,int);
extern void (*old_NoBikeCalculator)(int,int,int,int,int);
extern void (*old_CPed_ProcessBuoyancy)(int ped);
extern void (*old_CBike_ProcessBuoyancy)(int);
extern void (*old_CAutomobile_ProcessBuoyancy)(int);
extern void (*old_CDraw_SetFOV)(float);

// = SA-MP
extern void (*old_ChatWindowInputHandler)(char*);
extern void (*old_SetVehicleVelocityRPC)(int);
extern void (*AddChatMessage)(char* msg, ...);
extern void (*SendChatMessage)(char* msg);
extern void (*SendChatCommand)(char* cmd);
extern void (*CVehiclePool_New)(int pool, unsigned short*);
extern void (*old_CPlayerBubbleChatPool_New)(int playerbubblepool, int playerId, char* szText, int color, float drawDistance, int duration);
extern void (*old_ScrServerJoin)(int);
extern void (*old_ScrServerQuit)(int);
extern void (*old_ScrConnectionRejected)();
extern void (*old_ScrServerNewPlayer)(int pool, int nickname, int playerID);
extern void (*old_ScrServerDeletePlayer)(int pool, int playerID);
extern void (*old_WorldVehicleDelete)(RPCParameters* rpcParams);
extern void (*old_ScrInitGame)(RPCParameters*);
extern void (*old_ScrRequestSpawn)();
extern void (*old_ScrSetSpawnInfo)(int);
extern void (*old_LocalSetSpawnInfo)(int*,int*);
extern void (*old_AddChatMessage)(char*);

// For Testing/Log Hook ================================================= //
extern void (*old_log1)(int,unsigned short*);
extern void (*old_log2)(int,int,int,int,int);
extern void (*old_log3)(int peer, int *a2, RakNet::BitStream* a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14);
extern void (*old_log4)(int a1, int, int, float,float,float);

extern void log_1(int vehicle_pool, unsigned short* structure);
extern void log_2(int,int,int,int,int);
extern void log_3(int peer, int *a2, RakNet::BitStream* a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14);
extern void log_4(int a1, int, int, float,float,float);

// Definition
void ChangeCarModel_NumPad();

#endif
