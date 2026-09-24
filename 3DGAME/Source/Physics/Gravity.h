#pragma once

class Gravity
{
public:
    Gravity();

    void Update(float& velocityY);
    void Stop();
    void Reset();

private:
    float m_Gravity;
    float m_MaxFallSpeed;
    bool m_IsStopped;
};