#pragma once

// FPS設定
constexpr int TARGET_FPS = 60;

// FPSを計測するフレーム数
constexpr int FPS_SAMPLE_NUM = TARGET_FPS;

class FPSSystem
{
public:
    static void Init();
    static void Update();
    static void Draw();
    static void WaitFPS();

    // ゲーム内時間の経過時間
    static float GetDeltaTime();

    // 実時間の経過時間
    static float GetRealDeltaTime();

private:
    static int m_StartTime;
    static int m_Count;
    static float m_Fps;
    static float m_DeltaTime;
    static float m_GameDeltaTime;
    static int m_PreviousTime;
};