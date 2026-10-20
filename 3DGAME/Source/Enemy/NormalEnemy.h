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
    void PlayAnimation(int anim, bool isLoop, float speed) override;

private:
    VECTOR m_BasePos;

    BehaviorTree* m_BehaviorTree;

    bool m_PlayDeathAnimation;
};