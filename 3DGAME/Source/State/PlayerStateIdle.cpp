#include "../Player/Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateGroundAttack.h"
#include "PlayerStateMove.h"
#include "PlayerStateJump.h"
#include "PlayerStateRoll.h"
#include "../Input/Input.h"
#include "../ParameterData/PlayerDataManager.h"

PlayerStateIdle::PlayerStateIdle()
    :m_PlayerData(nullptr)
{
}

void PlayerStateIdle::Enter(Player* player)
{
    m_pPlayer = player;
    m_PlayerData = PlayerDataManager::GetInstance()->GetParameter();

    // 水平方向の移動を停止
    VECTOR move = m_pPlayer->GetMove();
    move.x = 0.0f;
    move.z = 0.0f;
    m_pPlayer->SetMove(move);
    //待機アニメーション
    m_pPlayer->PlayAnimation(PLAYER_ANIMATION_IDLE,true,m_PlayerData->animSpeedIdle);
}

void PlayerStateIdle::Update()
{
    Player* player = m_pPlayer;

    // ジャンプ
    if (player->IsJumpBuffered())
    {
        if (player->IsGround() || player->GetCoyoteTime() > 0.0f)
        {
            player->ResetJumpBuffer();
            ChangeState(std::make_unique<PlayerStateJump>());
            return;
        }
    }

    // 攻撃
    if (Input::IsAttack())
    {
        ChangeState(std::make_unique<PlayerStateGroundAttack>());
        return;
    }

    // ロール
    if (Input::IsRolling())
    {
        ChangeState(std::make_unique<PlayerStateRoll>());
        return;
    }

    // 移動
    if (Input::IsMove())
    {
        ChangeState(std::make_unique<PlayerStateMove>());
        return;
    }
}

void PlayerStateIdle::Exit()
{
}