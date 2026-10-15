#pragma once

#include "DxLib.h"

class AnimationController
{
public:
    AnimationController();
    ~AnimationController() {};

    void Init(int modelHandle);

    void Play(int anim, bool isLoop);
    void Play(int anim, bool isLoop, float speed);

    void Update();

    float GetNowTime() const;
    float GetTotalTime() const;

    int GetCurrentAnimation() const;

    bool IsFinished() const;

private:
    int m_ModelHandle;

    int m_AnimationAttachIndex;

    float m_AnimationTotalTime;
    float m_AnimationNowTime;

    bool m_IsLoopAnimation;

    float m_AnimationSpeed;

    int m_NowAnimation;
};