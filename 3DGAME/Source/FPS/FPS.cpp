#include "DxLib.h"
#include "FPS.h"
#include "../GameSetting/Color.h"
#include "../GameTime/GameTime.h"

// 静的メンバ変数の初期化
int FPSSystem::m_StartTime = 0;
int FPSSystem::m_Count = 0;
float FPSSystem::m_Fps = 0.0f;
float FPSSystem::m_DeltaTime = 0.0f;
float FPSSystem::m_GameDeltaTime = 0.0f;
int FPSSystem::m_PreviousTime = 0;

void FPSSystem::Init()
{
    m_StartTime = GetNowCount();
    m_PreviousTime = m_StartTime;

    m_Count = 0;
    m_Fps = 0.0f;

    m_DeltaTime = 1.0f / static_cast<float>(TARGET_FPS);
    m_GameDeltaTime = m_DeltaTime;
}

void FPSSystem::Update()
{
    int currentTime = GetNowCount();

    // 実時間の経過時間
    m_DeltaTime = (currentTime - m_PreviousTime) / 1000.0f;

    if (m_DeltaTime > 0.05f)
    {
        m_DeltaTime = 0.05f;
    }

    m_PreviousTime = currentTime;

    // ゲーム内時間の経過時間
    m_GameDeltaTime = m_DeltaTime * GameTime::GetTimeScale();

    // FPS計測
    if (m_Count == 0)
    {
        m_StartTime = currentTime;
    }

    if (m_Count == FPS_SAMPLE_NUM)
    {
        int time = currentTime;

        m_Fps = 1000.0f /
            ((time - m_StartTime) / static_cast<float>(FPS_SAMPLE_NUM));

        m_Count = 0;
        m_StartTime = time;
    }

    m_Count++;
}

void FPSSystem::Draw()
{
#ifdef _DEBUG
    DrawFormatString(0, 880, Color::Red(), "FPS【%.1f】", m_Fps);
#endif
}

void FPSSystem::WaitFPS()
{
    int takeTime = GetNowCount() - m_StartTime;
    int waitTime = m_Count * 1000 / TARGET_FPS - takeTime;

    if (waitTime > 0)
    {
        Sleep(waitTime);
    }
}

float FPSSystem::GetDeltaTime()
{
    return m_GameDeltaTime;
}

float FPSSystem::GetRealDeltaTime()
{
    return m_DeltaTime;
}