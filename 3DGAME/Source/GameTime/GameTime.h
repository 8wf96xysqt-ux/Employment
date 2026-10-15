#pragma once

class GameTime
{
public:
    static void SetTimeScale(float scale);
    static float GetTimeScale();

    static void Stop();
    static void Resume();
    static bool IsStopped();

private:
    static float m_TimeScale;
    static bool m_IsStopped;
};