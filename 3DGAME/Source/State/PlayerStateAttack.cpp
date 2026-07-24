#include "DxLib.h"
#include "../Player/Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerStateAttack.h"
#include "PlayerStateJump.h"
#include "PlayerStateRoll.h"


void PlayerStateAttack::Enter(Player* player)
{
	//プレイヤーを設定
	m_pPlayer = player;
	//次のstateを自身のstate設定
	m_pNextState = this;
}

void PlayerStateAttack::Update()
{
}

void PlayerStateAttack::Exit()
{
}
