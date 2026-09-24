#pragma once

// FPS設定
constexpr int TARGET_FPS = 60;

// FPSを計測するフレーム数
constexpr int FPS_SAMPLE_NUM = TARGET_FPS;

class FPSSystem
{
public:
    // FPSシステムの初期化
    static void Init();
    // FPSとDeltaTimeの更新
    static void Update();
    // FPSを画面に表示
    static void Draw();
    // 目標FPSに合わせて処理を待機
    static void WaitFPS();
    // 1フレームに経過した時間を取得
    static float GetDeltaTime();

private:
    // FPS計測を開始した時刻
    static int m_StartTime;
    // FPS計測用のフレームカウンタ
    static int m_Count;
    // 現在のFPS
    static float m_Fps;
    // 前フレームからの経過時間（秒）
    static float m_DeltaTime;
    // DeltaTime計算用の前回時刻
    static int m_PreviousTime;
};