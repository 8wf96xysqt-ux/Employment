#include "DxLib.h"

#include "../Player/Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerStateAttack.h"
#include "PlayerStateJump.h"
#include "PlayerStateRoll.h"
#include "../Input/Input.h"


void PlayerStateIdle::Enter(Player* player)
{
    m_pPlayer = player;

    // 初期状態は自分を保持
    m_pNextState = this;


    // 待機アニメーション
    m_pPlayer->PlayAnimation(PLAYER_ANIMATION_IDLE,true);
}


void PlayerStateIdle::Update()
{
    
    //移動入力
    if (Input::GetStickLX() != 0 ||
        Input::GetStickLY() != 0 ||
        Input::IsInputKey(Input::KEY_W) ||
        Input::IsInputKey(Input::KEY_A) ||
        Input::IsInputKey(Input::KEY_S) ||
        Input::IsInputKey(Input::KEY_D))
    {
        ChangeState(new PlayerStateMove());

        return;
    }


    //攻撃
    if (Input::IsAttack())
    {
        ChangeState(new PlayerStateAttack());

        return;
    }


    
    //ジャンプ
    if (Input::IsTriggerJump())
    {
        ChangeState(new PlayerStateJump());

        return;
    }


    //ロール
    if (Input::IsTriggerKey(Input::KEY_PAD_B))
    {
        ChangeState(new PlayerStateRoll());

        return;
    }
}


void PlayerStateIdle::Exit()
{

}