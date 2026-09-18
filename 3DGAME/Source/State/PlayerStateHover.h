#pragma once

#include "PlayerStateBase.h"

class EffekseerEffect;
class CollisionOBB;
struct PlayerData;

class PlayerStateHover : public PlayerStateBase
{
public:
    PlayerStateHover();
    ~PlayerStateHover() {};

    void Enter(Player* player) override;
    void Update() override;
    void Exit() override;

    bool IsGroundState() const override{return false;}

    PlayerStateType GetStateType() const override {return PlayerStateType::HOVER;}

private:
    float m_StartY;
    float m_HoverTime;
    float m_HoverTimeMax;
    bool m_IsRising;
    EffekseerEffect* m_LeftBeamEffect;
    EffekseerEffect* m_RightBeamEffect;
    EffekseerEffect* m_LeftBeamHitEffect;
    EffekseerEffect* m_RightBeamHitEffect;
    CollisionOBB* m_LeftBeamOBB;
    CollisionOBB* m_RightBeamOBB;
    const PlayerData* m_PlayerData;
};
