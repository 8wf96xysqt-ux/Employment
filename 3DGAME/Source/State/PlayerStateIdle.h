#pragma once
#include "PlayerStateBase.h"

class PlayerStateIdle : public PlayerStateBase
{
public:
	PlayerStateIdle() {};
	~PlayerStateIdle() {};

	virtual void Enter(Player* player)override;

	virtual void Update()override;

	virtual void Exit()override;
};