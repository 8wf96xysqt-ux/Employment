#pragma once

#include "DxLib.h"

enum PlayerAnimationType
{
    PLAYER_ANIMATION_AIRCOMBO1,
    PLAYER_ANIMATION_AIRCOMBO2,
    PLAYER_ANIMATION_DASHCOMBO1,
    PLAYER_ANIMATION_DASHCOMBO2,
    PLAYER_ANIMATION_DASHJUMP,
    PLAYER_ANIMATION_DIE,
    PLAYER_ANIMATION_FALL,
    PLAYER_ANIMATION_GDCOMBO1,
    PLAYER_ANIMATION_GDCOMBO2,
    PLAYER_ANIMATION_HOVER,
    PLAYER_ANIMATION_IDLE,
    PLAYER_ANIMATION_JUMP,
    PLAYER_ANIMATION_ROLLING,
    PLAYER_ANIMATION_RUN,
    PLAYER_ANIMATION_WALK,
};

class AnimationController
{
public:
    AnimationController();
    ~AnimationController() {};

    void Init(int modelHandle);
    void Play(PlayerAnimationType anim, bool isLoop);
    void Play(PlayerAnimationType anim, bool isLoop, float speed);
    void Update();

    float GetNowTime() const;
    float GetTotalTime() const;

    PlayerAnimationType GetCurrentAnimation() const;

    bool IsFinished() const;

private:
    int m_ModelHandle;

    int m_AnimationAttachIndex;

    float m_AnimationTotalTime;
    float m_AnimationNowTime;

    bool m_IsLoopAnimation;

    float m_AnimationSpeed;

    PlayerAnimationType m_NowAnimation;
};