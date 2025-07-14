#pragma once

#include "unistd.h"

class CTimer
{
public:
    CTimer();
    void Update();

    unsigned int GetTimeInMilliseconds() { return m_snTimeInMilliseconds; }
    float GetTimeScale() { return m_fTimeScale; }
    
    void SetTimeInMilliseconds(int value) { m_snTimeInMilliseconds = value; }
    void SetTimeScale(float value) { m_fTimeScale = value; }
private:
    unsigned int m_snTimeInMilliseconds;
    float m_fTimeScale;
};