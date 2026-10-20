#pragma once

#include "PlayerStateBase.h"
#include "../Attack/AttackCombo.h"

struct AttackData;

class PlayerStateGroundAttack : public PlayerStateBase
{
public:
    PlayerStateGroundAttack();
    ~PlayerStateGroundAttack() {};

    void Enter(Player* player) override;
    void Update() override;
    void Exit() override;

    PlayerStateType GetStateType() const override
    {
        return PlayerStateType::GROUND_ATTACK;
    }

private:
    bool m_IsRightPunch;
    AttackCombo m_AttackCombo;
    const AttackData* m_AttackData;

    void PlayAttackAnimation();
};