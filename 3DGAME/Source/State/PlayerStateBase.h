#pragma once

class Player;

class PlayerStateBase
{
public:
	PlayerStateBase();
	virtual ~PlayerStateBase() {};

	virtual void Enter(Player* player)abstract;
	virtual void Update()abstract;
	virtual void Exit()abstract;

	PlayerStateBase* GetNextState()
	{
		return m_pNextState;
	}

	void ChangeState(PlayerStateBase* nextState)
	{
		m_pNextState = nextState;
	}

protected:
	Player* m_pPlayer = nullptr;
	PlayerStateBase* m_pNextState = nullptr;
};