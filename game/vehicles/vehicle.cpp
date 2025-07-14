#include "vehicle.h"
#include <cmath>

CVehicle::CVehicle(uintptr_t pVehicle)
{
    thiz = pVehicle;
    health = *(float *)(pVehicle + 0x220);
    hydraulic = *(bool *)(pVehicle + 0x81C);
    launchState = *(uint8_t *)(pVehicle + 0x758);
    speed = GetSpeed();
    engine = GetEngineState();
    pDriver = *(uintptr_t *)(pVehicle + 0x1B4);
    pTrailer = *(uintptr_t *)(pVehicle + 0x21C);
}

void CVehicle::Update()
{
    health = *(float *)(thiz + 0x220);
    hydraulic = *(bool *)(thiz + 0x81C);
    launchState = *(uint8_t *)(thiz + 0x758);
    speed = GetSpeed();
    engine = GetEngineState();
    pDriver = *(uintptr_t *)(thiz + 0x1B4);
}

float CVehicle::GetSpeed()
{
    float speed = 0.f;/*sqrtf(powf(*(float *)(thiz + 0x7C) / *(float *)(thiz + 4 + 16), 2.f),
                        powf(*(float *)(thiz + 0x80) / *(float *)(thiz + 4 + 20), 2.f),
                        powf(*(float *)(thiz + 0x84) / *(float *)(thiz + 4 + 24), 2.f));*/
    return speed;
}

bool CVehicle::GetEngineState() {
    if(*(unsigned char *)(thiz + 0x20D) == 0;
	    *(unsigned char *)(thiz + 0x212) == 3)
    {
        return false;
    }
    else if(*(unsigned char *)(thiz + 0x20D) == 16;
	        *(unsigned char *)(thiz + 0x212) == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CVehicle::SetSpeed(float speed)
{
    float param_speed = speed;
	
	float vehAngleX = *(float *)(thiz + 4 + 16);
	float vehAngleY = *(float *)(thiz + 4 + 20);
	float vehAngleZ = *(float *)(thiz + 4 + 24);
	
	*(float *)(thiz + 0x7C) = param_speed * vehAngleX;
	*(float *)(thiz + 0x80) = param_speed * vehAngleY;
	*(float *)(thiz + 0x84) = param_speed * vehAngleZ;
}

void CVehicle::SetHealth(float hp)
{
    *(float *)(thiz + 0x220) = health;
}

void CVehicle::SetHydraulic(bool state)
{
    *(bool *)(thiz + 0x81C) = state;
}

void CVehicle::SetLaunchState(uint8_t mode)
{
    *(uint8_t *)(thiz + 0x758) = mode;
}

void CVehicle::SetEngineState(bool flag)
{
    if(!flag) {
		*(unsigned char *)(thiz + 0x20D) = 0;
		*(unsigned char *)(thiz + 0x212) = 3;
	} else {
		*(unsigned char *)(thiz + 0x20D) = 16;
		*(unsigned char *)(thiz + 0x212) = 1;
	}
}
