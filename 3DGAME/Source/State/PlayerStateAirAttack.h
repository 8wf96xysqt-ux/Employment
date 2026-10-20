#pragma once

#include "PlayerStateBase.h"
#include "../Attack/AttackCombo.h"

struct AttackData;

class PlayerStateAirAttack : public PlayerStateBase
{
public:
    PlayerStateAirAttack();
    ~PlayerStateAirAttack() = default;

    void Enter(Player* player) override;
    void Update() override;
    void Exit() override;

    PlayerStateType GetStateType() const override
    {
        return PlayerStateType::AIR_ATTACK;
    }

private:
    // 攻撃の右・左
    bool m_IsRightPunch;

    // コンボ管理
    AttackCombo m_AttackCombo;

    const AttackData* m_AttackData;

    void PlayAttackAnimation();
};