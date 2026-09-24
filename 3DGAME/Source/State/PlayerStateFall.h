#pragma once
#include "PlayerStateBase.h"

struct PlayerData;

class PlayerStateFall : public PlayerStateBase
{
public:
    PlayerStateFall();
    ~PlayerStateFall() {};

    void Enter(Player* player) override;
    void Update() override;
    void Exit() override;
    bool IsGroundState() const override{return false; }
    PlayerStateType GetStateType() const override{return PlayerStateType::FALL;}
private:
    float m_FallAnimDelay;
    const PlayerData* m_PlayerData;
};