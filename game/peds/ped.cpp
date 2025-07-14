#include "ped.h"
#include "memory/memory.h"
#include "gameLibAddr.h"

CPed::CPed(uintptr_t ped)
{
    thiz = ped;
    health = *(float *)(thiz + 0x360);
    armour = *(float *)(thiz + 0x364);
    headingRate = *(float *)(thiz + 0x38C);
    jumpMult = *(float *)(thiz + 0xC4);
    pedState = *(ePedState *)(thiz + 0x250);
    prevPedState = *(ePedState *)(thiz + 0x254);
    pedMoveState = *(eMoveState *)(thiz + 0x258);
    animationID = *(uint16_t *)(ped + 0x64C);
    if(*(uintptr_t *)(thiz + 0x3B0))
    {
        vehicle = *(uintptr_t *)(thiz + 0x3B0);
    }
    else
    {
        vehicle = 0;
    }
	inVehicle = *(int8_t *)(thiz + 0x3B8);
}

void CPed::Update()
{
    health = *(float *)(thiz + 0x360);
    armour = *(float *)(thiz + 0x364);
    headingRate = *(float *)(thiz + 0x38C);
    jumpMult = *(float *)(thiz + 0xC4);
    pedState = *(ePedState *)(thiz + 0x250);
    prevPedState = *(ePedState *)(thiz + 0x254);
    pedMoveState = *(eMoveState *)(thiz + 0x258);
    animationID = *(uint16_t *)(thiz + 0x64C);
    if(*(uintptr_t *)(thiz + 0x3B0))
    {
        vehicle = *(uintptr_t *)(thiz + 0x3B0);
    }
    else
    {
        vehicle = 0;
    }
}

void CPed::SetHealth(float hp)
{
    *(float *)(thiz + 0x360) = hp;
}

void CPed::SetArmour(float armor)
{
    *(float *)(thiz + 0x364) = armor;
}

void CPed::SetHeadingRate(float rate)
{
    *(float *)(thiz + 0x38C) = rate;
}

void CPed::SetHighJumpMult(float mult)
{
    *(float *)(thiz + 0xC4) = mult;
}

void CPed::SetLongJumpMult(float mult)
{
    WriteMemoryFloat(gameLibAddr + 0x392FDC, mult);
}

void CPed::SetPedState(ePedState state)
{
    *(ePedState *)(thiz + 0x250) = state;
}

void CPed::SetPrevPedState(ePedState state)
{
    *(ePedState *)(thiz + 0x254) = state;
}

void CPed::SetMoveState(eMoveState state)
{
    *(eMoveState *)(thiz + 0x258) = state;
}

void CPed::SetMoveAnim()
{
    ((void(*)(uintptr_t))(gameLibAddr + 0x37da34 + 1))(thiz);
}
