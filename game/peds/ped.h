#pragma once

#include "stdint.h"
#include "game/vehicles/vehicle.h"


enum ePedState {
    PEDSTATE_NONE = 0,
    PEDSTATE_IDLE = 1,
    PEDSTATE_LOOK_ENTITY = 2,
    PEDSTATE_LOOK_HEADING = 3,
    PEDSTATE_WANDER_RANGE = 4,
    PEDSTATE_WANDER_PATH = 5,
    PEDSTATE_SEEK_POSITION = 6,
    PEDSTATE_SEEK_ENTITY = 7,
    PEDSTATE_FLEE_POSITION = 8,
    PEDSTATE_FLEE_ENTITY = 9,
    PEDSTATE_PURSUE = 10,
    PEDSTATE_FOLLOW_PATH = 11,
    PEDSTATE_SNIPER_MODE = 12,
    PEDSTATE_ROCKET_MODE = 13,
    PEDSTATE_DUMMY = 14,
    PEDSTATE_PAUSE = 15,
    PEDSTATE_ATTACK = 16,
    PEDSTATE_FIGHT = 17,
    PEDSTATE_FACE_PHONE = 18,
    PEDSTATE_MAKE_PHONECALL = 19,
    PEDSTATE_CHAT = 20,
    PEDSTATE_MUG = 21,
    PEDSTATE_AIMGUN = 22,
    PEDSTATE_AI_CONTROL = 23,
    PEDSTATE_SEEK_CAR = 24,
    PEDSTATE_SEEK_BOAT = 25,
    PEDSTATE_FOLLOW_ROUTE = 26,
    PEDSTATE_CPR = 27,
    PEDSTATE_SOLICIT = 28,
    PEDSTATE_BUY_ICECREAM = 29,
    PEDSTATE_INVESTIGATE = 30,
    PEDSTATE_STEP_AWAY = 31,
    PEDSTATE_ON_FIRE = 32,
    PEDSTATE_HANG_OUT = 33,
    PEDSTATE_JUMP = 41,
    PEDSTATE_FALL = 42,
    PEDSTATE_GETUP = 43,
    PEDSTATE_JUMP_FROM_VEHICLE = 44,
    PEDSTATE_DRIVING = 50,
    PEDSTATE_DEAD = 54,
    PEDSTATE_WASTED = 55,
    PEDSTATE_CAR_JACK = 56,
    PEDSTATE_ENTER_CAR = 58,
    PEDSTATE_EXIT_CAR = 60
};

enum eMoveState {
	PEDMOVE_NONE,
	PEDMOVE_STILL,
	PEDMOVE_WALK,
	PEDMOVE_RUN,
	PEDMOVE_SPRINT,
};

class CPed
{
public:
    CPed(uintptr_t ped);
    void Update();

    // Getters
    uintptr_t GetMe() { return thiz; };
    float GetHealth() { return health; }
    float GetArmour() { return armour; }
    float GetHeadingRate() { return headingRate; }
    float GetJumpMult() { return jumpMult; }
    ePedState GetPedState() { return pedState; }
    ePedState GetPrevPedState() { return prevPedState; }
    eMoveState GetMoveState() { return pedMoveState; }
    uintptr_t GetVehicle() { return vehicle; }
    uint16_t GetAnimationID() { return animationID; }
    bool InVehicle() { return inVehicle; }
    // Setters
    void SetHealth(float hp);
    void SetArmour(float armor);
    void SetHeadingRate(float rate);
    void SetHighJumpMult(float mult);
    void SetLongJumpMult(float mult);
    void SetPedState(ePedState state);
    void SetPrevPedState(ePedState state);
    void SetMoveState(eMoveState state);
    void SetMoveAnim();
    
private:

    uintptr_t thiz;
    float health;
    float armour;
    float headingRate;
    float jumpMult;
    ePedState pedState;
    ePedState prevPedState;
    eMoveState pedMoveState;
    uintptr_t vehicle;
    uint16_t animationID;
	int8_t inVehicle;
	
};
