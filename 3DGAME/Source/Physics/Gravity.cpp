#include "Gravity.h"
#include "../GameSetting/GameSetting.h"
#include "../FPS/FPS.h"

constexpr float GRAVITY = 0.01f;
constexpr float MAX_FALL_SPEED = 0.25f;

Gravity::Gravity()
    : m_Gravity(GRAVITY)
    , m_MaxFallSpeed(MAX_FALL_SPEED)
    , m_IsStopped(false)
{
}

void Gravity::Update(float& velocityY)
{
    if (m_IsStopped)
    {
        return;
    }

    velocityY -= m_Gravity * TARGET_FPS * FPSSystem::GetDeltaTime();

    // óéâ∫ë¨ìxêßå¿
    if (velocityY < -m_MaxFallSpeed)
    {
        velocityY = -m_MaxFallSpeed;
    }
}

void Gravity::Stop()
{
    m_IsStopped = true;
}

void Gravity::Reset()
{
    m_IsStopped = false;
}