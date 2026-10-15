#include "GameTime.h"

float GameTime::m_TimeScale = 1.0f;
bool GameTime::m_IsStopped = false;

void GameTime::SetTimeScale(float scale)
{
    if (scale < 0.0f) scale = 0.0f;

    m_TimeScale = scale;
}

float GameTime::GetTimeScale()
{
    return m_TimeScale;
}

void GameTime::Stop()
{
    m_IsStopped = true;
}

void GameTime::Resume()
{
    m_IsStopped = false;
}

bool GameTime::IsStopped()
{
    return m_IsStopped;
}