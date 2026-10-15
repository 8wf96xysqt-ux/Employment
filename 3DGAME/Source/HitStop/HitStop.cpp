#include "HitStop.h"
#include "../FPS/FPS.h"

float HitStop::m_Time = 0.0f;

void HitStop::Start(float time)
{
    if (time > m_Time)
    {
        m_Time = time;
    }
}

void HitStop::Update()
{
    if (m_Time <= 0.0f) return;

    m_Time -= FPSSystem::GetDeltaTime();

    if (m_Time < 0.0f)
    {
        m_Time = 0.0f;
    }
}

bool HitStop::IsActive()
{
    return m_Time > 0.0f;
}