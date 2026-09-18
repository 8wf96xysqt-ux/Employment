#pragma once
#include "PlayerStateBase.h"

struct AttackData;

class PlayerStateAirAttack : public PlayerStateBase
{
public:
    PlayerStateAirAttack();
    ~PlayerStateAirAttack() override;

    void Enter(Player* player) override;
    void Update() override;
    void Exit() override;

    PlayerStateType GetStateType() const override
    {
        return PlayerStateType::AIR_ATTACK;
    }

private:
    // UŒ‚‚Ì‰EE¶
    bool m_IsRightPunch;

    // Ÿ‚ÌUŒ‚“ü—Í
    bool m_AttackBuffered;

    //UŒ‚ƒJƒEƒ“ƒg
    int m_AttackCount;

    const AttackData* m_AttackData;


private:
    void PlayAttackAnimation();
};