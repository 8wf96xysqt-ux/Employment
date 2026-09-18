#include "DxLib.h"
#include "../Player/Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateGroundAttack.h"
#include "PlayerStateMove.h"
#include "PlayerStateJump.h"
#include "PlayerStateRoll.h"
#include "PlayerStateFall.h"
#include "../Input/Input.h"
#include "../ParameterData/PlayerDataManager.h"

PlayerStateIdle::PlayerStateIdle()
    :m_PlayerData(nullptr)
{
}

void PlayerStateIdle::Enter(Player* player)
{
    m_pPlayer = player;
    m_pNextState = this;
    m_PlayerData = PlayerDataManager::GetInstance()->GetParameter();
    //待機アニメーション
    m_pPlayer->PlayAnimation(PLAYER_ANIMATION_IDLE,true,m_PlayerData->animSpeedIdle);
}

void PlayerStateIdle::Update()
{
    //バッファーとコヨーテタイム
    if (m_pPlayer->IsJumpBuffered())
    {
        if (m_pPlayer->IsGround() || m_pPlayer->GetCoyoteTime() > 0.0f)
        {
            m_pPlayer->ResetJumpBuffer();

            ChangeState(new PlayerStateJump());

            return;
        }
    }

    VECTOR move = m_pPlayer->GetMove();

    move.x = 0.0f;
    move.z = 0.0f;

    m_pPlayer->SetMove(move);

    // 攻撃
    if (Input::IsAttack())
    {
        ChangeState(new PlayerStateGroundAttack());
        return;
    }

    // ロール
    if (Input::IsRolling())
    {
        ChangeState(new PlayerStateRoll());
        return;
    }

    // 移動
    if (Input::IsMove())
    {
        ChangeState(new PlayerStateMove());
        return;
    }
}

void PlayerStateIdle::Exit()
{
}