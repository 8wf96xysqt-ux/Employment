#pragma once

class HitStop
{
public:
    static void Start(float time);
    static void Update();
    static bool IsActive();

private:
    static float m_Time;
};