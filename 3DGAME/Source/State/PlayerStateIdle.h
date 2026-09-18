#pragma once
#include "PlayerStateBase.h"

struct PlayerData;

class PlayerStateIdle : public PlayerStateBase
{
public:
	PlayerStateIdle();
	~PlayerStateIdle() {};

	virtual void Enter(Player* player)override;

	virtual void Update()override;

	virtual void Exit()override;

	bool IsGroundState() const override { return true; }

	PlayerStateType GetStateType() const override { return PlayerStateType::IDLE; }
private:
	const PlayerData* m_PlayerData;
};