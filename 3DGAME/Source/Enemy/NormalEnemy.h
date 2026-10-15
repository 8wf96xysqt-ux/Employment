#pragma once
#include "EnemyBase.h"
#include "../Animation/NormalEnemyAnimParam..h"

class BehaviorTree;

class NormalEnemy : public EnemyBase
{
public:
    NormalEnemy();
    ~NormalEnemy() override;

    void Init() override;
    void Load() override;
    void Start() override;
    void Step() override;
    void Update() override;
    void Draw() override;
    void Fin() override;

    // 死亡処理
    void Die() override;
    bool IsDeathAnimationFinished() const override;

    // アニメーション
    float GetAnimationNowTime() const
    {
        return m_Animation.GetNowTime();
    }

    float GetAnimationTotalTime() const
    {
        return m_Animation.GetTotalTime();
    }

    void PlayAnimation(NormalEnemyAnimationType anim,bool isLoop,float speed)
    {
        m_Animation.Play(anim, isLoop, speed);
    }

private:
    VECTOR m_BasePos;

    BehaviorTree* m_BehaviorTree;

    bool m_PlayDeathAnimation;
};