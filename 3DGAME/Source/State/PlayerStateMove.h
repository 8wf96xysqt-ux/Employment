#pragma once
#include "PlayerStateBase.h"

struct PlayerData;

class PlayerStateMove : public PlayerStateBase
{
public:
	PlayerStateMove();
	virtual~PlayerStateMove() {};

	virtual void Enter(Player* player)override;

	virtual void Update()override;

	virtual void Exit()override;

	bool IsGroundState() const override { return true; }

	PlayerStateType GetStateType() const override { return PlayerStateType::MOVE; }
private:
	const PlayerData* m_PlayerData;

};