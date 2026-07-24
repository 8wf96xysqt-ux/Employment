#pragma once
#include "PlayerStateBase.h"

class PlayerStateJump : public PlayerStateBase
{
public:
	PlayerStateJump() {};
	~PlayerStateJump() {};

	virtual void Enter(Player* player)override;

	virtual void Update()override;

	virtual void Exit()override;
};