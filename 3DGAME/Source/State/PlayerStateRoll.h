#pragma once
#include "PlayerStateBase.h"

class PlayerStateRoll : public PlayerStateBase
{
public:
	PlayerStateRoll() {};
	~PlayerStateRoll() {};

	virtual void Enter(Player* player)override;

	virtual void Update()override;

	virtual void Exit()override;

};