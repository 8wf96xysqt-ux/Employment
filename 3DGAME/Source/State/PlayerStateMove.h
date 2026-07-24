#pragma once
#include "PlayerStateBase.h"

class PlayerStateMove : public PlayerStateBase
{
public:
	PlayerStateMove() {};
	~PlayerStateMove() {};

	virtual void Enter(Player* player)override;

	virtual void Update()override;

	virtual void Exit()override;

};