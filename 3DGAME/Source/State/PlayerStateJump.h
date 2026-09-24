#pragma once
#include "PlayerStateBase.h"

struct PlayerData;

class PlayerStateJump : public PlayerStateBase
{
public:
	PlayerStateJump();
	~PlayerStateJump() {};

	virtual void Enter(Player* player)override;

	virtual void Update()override;

	virtual void Exit()override;

	//空中ステートなので false
	bool IsGroundState() const override { return false; }

	//JUMPタイプを返す
	PlayerStateType GetStateType() const override { return PlayerStateType::JUMP; }
private:
	const PlayerData* m_PlayerData;
};