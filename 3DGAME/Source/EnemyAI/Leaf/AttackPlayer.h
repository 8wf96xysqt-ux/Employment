#pragma once
#include "LeafNodeBase.h"
#include "DxLib.h"

class CollisionOBB;

class AttackPlayer : public LeafNodeBase
{
public:
    explicit AttackPlayer(BlackBoard* blackBoard);
    ~AttackPlayer() override;

    void Init() override;
    void Tick() override;
    void Finalize() override;

private:
    CollisionOBB* m_AttackOBB;
    bool m_IsAttacking;
    bool m_HasHit;
    float m_Cooldown;
};