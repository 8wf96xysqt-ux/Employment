#include "DxLib.h"
#include "FPS.h"
#include "../GameSetting/Color.h"

// 静的メンバ変数の初期化
int FPSSystem::m_StartTime = 0;
int FPSSystem::m_Count = 0;
float FPSSystem::m_Fps = 0.0f;
float FPSSystem::m_DeltaTime = 0.0f;
int FPSSystem::m_PreviousTime = 0;

void FPSSystem::Init()
{
    // 現在時刻を取得
    m_StartTime = GetNowCount();
    m_PreviousTime = m_StartTime;

    // FPS計測値を初期化
    m_Count = 0;
    m_Fps = 0.0f;

    // 初期DeltaTimeは60FPSを基準にする
    m_DeltaTime = 1.0f / static_cast<float>(TARGET_FPS);
}

void FPSSystem::Update()
{
    // 現在の時刻を取得
    int currentTime = GetNowCount();

    // 前フレームからの経過時間を秒に変換
    m_DeltaTime =
        (currentTime - m_PreviousTime) / 1000.0f;

    // 次のフレームのために現在時刻を保存
    m_PreviousTime = currentTime;


    // FPS計測開始
    if (m_Count == 0)
    {
        m_StartTime = currentTime;
    }

    // 一定フレーム数ごとにFPSを計算
    if (m_Count == FPS_SAMPLE_NUM)
    {
        int time = currentTime;

        // サンプルしたフレーム数と経過時間からFPSを計算
        m_Fps = 1000.0f / ((time - m_StartTime) / static_cast<float>(FPS_SAMPLE_NUM));
        // 次の計測のためにリセット
        m_Count = 0;
        m_StartTime = time;
    }

    // FPS計測用のフレーム数を加算
    m_Count++;
}

void FPSSystem::Draw()
{
#ifdef _DEBUG
    // デバッグ時のみ現在のFPSを表示
    DrawFormatString( 0,880, Color::Red(), "FPS【%.1f】", m_Fps );
#endif
}

void FPSSystem::WaitFPS()
{
    // 現在の計測開始から経過した時間
    int takeTime = GetNowCount() - m_StartTime;

    // 目標FPSに達するまでの待機時間を計算
    int waitTime = m_Count * 1000 / TARGET_FPS - takeTime;

    // 処理が早く終わっていた場合は待機
    if (waitTime > 0)
    {
        Sleep(waitTime);
    }
}

float FPSSystem::GetDeltaTime()
{
    // 1フレームの経過時間を返す
    return m_DeltaTime;
}