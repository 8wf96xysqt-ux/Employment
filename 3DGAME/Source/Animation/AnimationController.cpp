#include "AnimationController.h"
#include "../GameTime/GameTime.h"

AnimationController::AnimationController()
    : m_ModelHandle(-1)
    , m_AnimationAttachIndex(-1)
    , m_AnimationTotalTime(0.0f)
    , m_AnimationNowTime(0.0f)
    , m_IsLoopAnimation(false)
    , m_AnimationSpeed(1.0f)
    , m_NowAnimation(-1)
{
}

void AnimationController::Init(int modelHandle)
{
    m_ModelHandle = modelHandle;

    m_AnimationAttachIndex = -1;
    m_AnimationTotalTime = 0.0f;
    m_AnimationNowTime = 0.0f;
    m_IsLoopAnimation = false;
    m_AnimationSpeed = 1.0f;

    m_NowAnimation = -1;
}

void AnimationController::Play(int anim, bool isLoop)
{
    if (anim == m_NowAnimation)
    {
        return;
    }

    if (m_AnimationAttachIndex >= 0)
    {
        MV1DetachAnim(m_ModelHandle, m_AnimationAttachIndex);
    }

    m_AnimationAttachIndex = MV1AttachAnim(m_ModelHandle, anim);

    m_AnimationTotalTime = MV1GetAttachAnimTotalTime(m_ModelHandle, m_AnimationAttachIndex);

    m_AnimationNowTime = 0.0f;
    m_IsLoopAnimation = isLoop;
    m_NowAnimation = anim;
    m_AnimationSpeed = 1.0f;
}

void AnimationController::Play(int anim, bool isLoop, float speed)
{
    if (anim == m_NowAnimation)
    {
        m_AnimationSpeed = speed;
        return;
    }

    if (m_AnimationAttachIndex >= 0)
    {
        MV1DetachAnim(m_ModelHandle, m_AnimationAttachIndex);
    }

    m_AnimationAttachIndex = MV1AttachAnim(m_ModelHandle, anim);

    m_AnimationTotalTime = MV1GetAttachAnimTotalTime(m_ModelHandle, m_AnimationAttachIndex);

    m_AnimationNowTime = 0.0f;
    m_IsLoopAnimation = isLoop;
    m_NowAnimation = anim;
    m_AnimationSpeed = speed;
}

void AnimationController::Update()
{
    if (m_ModelHandle < 0)
    {
        return;
    }

    if (m_AnimationAttachIndex < 0)
    {
        return;
    }

    MV1SetAttachAnimTime(m_ModelHandle,m_AnimationAttachIndex,m_AnimationNowTime);

    // ƒQ[ƒ€‘S‘Ì‚ÌŽžŠÔ”{—¦‚ð”½‰f
    m_AnimationNowTime +=
        m_AnimationSpeed * GameTime::GetTimeScale();

    if (m_AnimationNowTime > m_AnimationTotalTime)
    {
        if (m_IsLoopAnimation)
        {
            m_AnimationNowTime = 0.0f;
        }
        else
        {
            m_AnimationNowTime = m_AnimationTotalTime;
        }
    }
}

float AnimationController::GetNowTime() const
{
    return m_AnimationNowTime;
}

float AnimationController::GetTotalTime() const
{
    return m_AnimationTotalTime;
}

int AnimationController::GetCurrentAnimation() const
{
    return m_NowAnimation;
}

bool AnimationController::IsFinished() const
{
    if (m_IsLoopAnimation)
    {
        return false;
    }

    return m_AnimationNowTime >= m_AnimationTotalTime;
}