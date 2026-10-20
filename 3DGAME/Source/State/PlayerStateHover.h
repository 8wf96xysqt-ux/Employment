#pragma once

#include "PlayerStateBase.h"
#include "../Attack/PlayerHoverBeam.h"

struct PlayerData;

class PlayerStateHover : public PlayerStateBase
{
public:
    PlayerStateHover();
    ~PlayerStateHover() override = default;

    void Enter(Player* player) override;
    void Update() override;
    void Exit() override;
    bool CheckAttackHit(CollisionAABB* target) override;

    PlayerStateType GetStateType() const override
    {
        return PlayerStateType::HOVER;
    }

private:
    bool UpdateHoverMovement();

    float m_StartY;
    float m_HoverTime;
    float m_HoverTimeMax;
    bool m_IsRising;
    const PlayerData* m_PlayerData;
    PlayerHoverBeam m_HoverBeam;
};