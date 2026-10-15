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

	//JUMPƒ^ƒCƒv‚ð•Ô‚·
	PlayerStateType GetStateType() const override 
	{
		return PlayerStateType::JUMP; 
	}
private:
	const PlayerData* m_PlayerData;
};