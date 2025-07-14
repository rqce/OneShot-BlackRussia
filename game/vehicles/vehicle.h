#pragma once

#include "stdint.h"

enum eVehicleType
{
    BIKE = 5,
    TRAILER = 7
};

class CVehicle
{
public:
    CVehicle(uintptr_t pVehicle);
    void Update();
    float GetSpeed();
    float GetHealth() { return health; }
    bool GetHydraulic() { return hydraulic; }
    uint8_t GetLaunchState() { return launchState; }
    bool GetEngineState();
    uintptr_t GetDriver() { return pDriver; }
    uintptr_t GetTrailer() { return pTrailer; }

    void SetSpeed(float speed);
    void SetHealth(float hp);
    void SetHydraulic(bool state);
    void SetLaunchState(uint8_t mode);
    void SetEngineState(bool state);

private:
    uintptr_t thiz;
    float speed;
    float health;
    bool hydraulic;
    uint8_t launchState;
    bool engine;
    uintptr_t pDriver;
    uintptr_t pTrailer;
};