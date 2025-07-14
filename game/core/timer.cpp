#include "timer.h"
#include "gameLibAddr.h"

CTimer::CTimer()
{
    m_snTimeInMilliseconds = 1;
    m_fTimeScale = 1.f;
}

void CTimer::Update()
{
    float* timeScale = (float *)(gameLibAddr + 0x464508C);
    if(timeScale)
    {
        SetTimeScale(*timeScale);
    }

    int* timeInMilliseconds = (int *)(gameLibAddr + 0x464507C);
    if(timeInMilliseconds)
    {
        SetTimeInMilliseconds(*timeInMilliseconds);
    }
}
