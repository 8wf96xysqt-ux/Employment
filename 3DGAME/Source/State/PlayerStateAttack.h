#pragma once
#include "PlayerStateBase.h"

class PlayerStateAttack : public PlayerStateBase
{
public:
	PlayerStateAttack() {};
	~PlayerStateAttack() {};

	virtual void Enter(Player* player)override;

	virtual void Update()override;

	virtual void Exit()override;
};