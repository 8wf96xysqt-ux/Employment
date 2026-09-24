#pragma once

#include "PlayerStateBase.h"

struct AttackData;

class PlayerStateGroundAttack : public PlayerStateBase
{
public:
    PlayerStateGroundAttack();
    ~PlayerStateGroundAttack() override;

    void Enter(Player* player) override;
    void Update() override;
    void Exit() override;

    bool IsGroundState() const override { return true; }

    PlayerStateType GetStateType() const override { return PlayerStateType::GROUND_ATTACK; }

private:
    bool m_IsRightPunch;
    bool m_AttackBuffered;

    const AttackData* m_AttackData;

private:
    void PlayAttackAnimation();
};