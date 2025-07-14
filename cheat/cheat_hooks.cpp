#include "cheat_hooks.h"

void CGame_Process();
int GetIsLineOfSightClear(int origin, int target, int buildings, int vehicles, int peds, int objects, int dummies, int doSeeThroughCheck, int doCameraIgnoreCheck);
void CSprintCalculator(int*);
void CPedDamageCalculator(int,int,unsigned int,float,int,int);
void CVehicleDamageCalculator(int,float,int);
void NoBikeCalculator(int,int,int,int,int);
void CPed_ProcessBuoyancy(int ped);
void CBike_ProcessBuoyancy(int);
void CAutomobile_ProcessBuoyancy(int);
void CDraw_SetFOV(float);

// = Samp
void ChatWindowInputHandler(char*);
void SetVehicleVelocityRPC(int);
void CPlayerBubbleChatPool_New(int playerbubblepool, int playerId, char* szText, int color, float drawDistance, int duration);
void ScrServerJoin(int);
void ScrServerQuit(int);
void ScrConnectionRejected();
void ScrServerNewPlayer(int pool, int nickname, int playerID);
void ScrServerDeletePlayer(int pool, int playerID);
void ScrRequestSpawn();
void ScrSetSpawnInfo(int);
void LocalSetSpawnInfo(int*,int*);
void AddChatMessage_hook(char*);
void ScrInitGame(RPCParameters*);
void WorldVehicleDelete(RPCParameters* rpcParams);

// ================================================================ //

// Before (old) Functions
void (*old_Java_com_blackhub_bronline_game_core_JNILib_multiTouchEvent)(JNIEnv* env, jclass cls, int actionMasked, int actionIndex, int x1, int y1, int x2, int y2, int x3, int y3);

// = Game
void (*old_CGame_Process)();
float (*FindGroundZ)(float,float);
int (*CPlayerPool_GetAt)(int pool, unsigned short id);
int (*CVehiclePool_GetAt)(int pool, unsigned short id);
int (*GetBonePosition)(int, int);
int WorldToScreen(Vec3*, Vec3*);
void (*SetSeek)(int player, float x, float y, float z, float dist);
void (*SetMoveAnim)(int player);
int (*old_GetIsLineOfSightClear)(int origin, int target, int buildings, int vehicles, int peds, int objects, int dummies, int doSeeThroughCheck, int doCameraIgnoreCheck);
void (*old_CSprintCalculator)(int*);
void (*old_CPedDamageCalculator)(int,int,unsigned int,float,int,int);
void (*old_CVehicleDamageCalculator)(int,float,int);
void (*old_NoBikeCalculator)(int,int,int,int,int);
void (*old_CPed_ProcessBuoyancy)(int ped);
void (*old_CBike_ProcessBuoyancy)(int);
void (*old_CAutomobile_ProcessBuoyancy)(int);
void (*old_CDraw_SetFOV)(float);

// = SA-MP
void (*old_ChatWindowInputHandler)(char*);
void (*old_SetVehicleVelocityRPC)(int);
void (*AddChatMessage)(char* msg, ...);
void (*SendChatMessage)(char* msg);
void (*SendChatCommand)(char* cmd);
void (*CVehiclePool_New)(int pool, unsigned short*);
void (*old_CPlayerBubbleChatPool_New)(int playerbubblepool, int playerId, char* szText, int color, float drawDistance, int duration);
void (*old_ScrServerJoin)(int);
void (*old_ScrServerQuit)(int);
void (*old_ScrConnectionRejected)();
void (*old_ScrServerNewPlayer)(int pool, int nickname, int playerID);
void (*old_ScrServerDeletePlayer)(int pool, int playerID);
void (*old_WorldVehicleDelete)(RPCParameters* rpcParams);
void (*old_ScrInitGame)(RPCParameters*);
void (*old_ScrRequestSpawn)();
void (*old_ScrSetSpawnInfo)(int);
void (*old_LocalSetSpawnInfo)(int*,int*);
void (*old_AddChatMessage)(char*);

// For Testing/Log Hook ================================================= //
void (*old_log1)(int,unsigned short*);
void (*old_log2)(int,int,int,int,int);
void (*old_log3)(int peer, int *a2, RakNet::BitStream* a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14);
void (*old_log4)(int a1, int, int, float,float,float);

void log_1(int vehicle_pool, unsigned short* structure);
void log_2(int,int,int,int,int);
void log_3(int peer, int *a2, RakNet::BitStream* a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14);
void log_4(int a1, int, int, float,float,float);

// Definition
void ChangeCarModel_NumPad();


