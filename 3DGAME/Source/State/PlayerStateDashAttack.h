#pragma once

#include "PlayerStateBase.h"
#include "../Attack/AttackCombo.h"

struct AttackData;
struct PlayerData;

class PlayerStateDashAttack : public PlayerStateBase
{
public:
    PlayerStateDashAttack();
	~PlayerStateDashAttack() = default;

    void Enter(Player* player) override;
    void Update() override;
    void Exit() override;

    PlayerStateType GetStateType() const override
    {
        return PlayerStateType::DASH_ATTACK;
    }

private:
    bool m_IsRightPunch;
    AttackCombo m_AttackCombo;
    const AttackData* m_AttackData;
    const PlayerData* m_PlayerData;

    void PlayAttackAnimation();
};