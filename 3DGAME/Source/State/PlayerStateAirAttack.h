#pragma once
#include "PlayerStateBase.h"

struct AttackData;

class PlayerStateAirAttack : public PlayerStateBase
{
public:
    PlayerStateAirAttack();
    ~PlayerStateAirAttack(){};

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

    // 次の攻撃入力
    bool m_AttackBuffered;

    //攻撃カウント
    int m_AttackCount;

    const AttackData* m_AttackData;


private:
    void PlayAttackAnimation();
};