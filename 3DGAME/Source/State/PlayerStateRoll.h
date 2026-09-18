#pragma once
#include "DxLib.h"
#include "PlayerStateBase.h"

struct PlayerData;

class PlayerStateRoll : public PlayerStateBase
{
public:
    PlayerStateRoll();
    ~PlayerStateRoll();

    void Enter(Player* player) override;
    void Update() override;
    void Exit() override;

    bool IsGroundState() const override { return true; }

    PlayerStateType GetStateType() const override {return PlayerStateType::ROLL;}

private:
    float m_RollTime;
    float m_RollTimeMax;
    VECTOR m_RollDir;
    const PlayerData* m_PlayerData;
};