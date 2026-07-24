#include "PlayerStateRoll.h"

void PlayerStateRoll::Enter(Player* player)
{
	//プレイヤーを設定
	m_pPlayer = player;
	//次のstateを自身のstate設定
	m_pNextState = this;
}

void PlayerStateRoll::Update()
{
}

void PlayerStateRoll::Exit()
{
}
