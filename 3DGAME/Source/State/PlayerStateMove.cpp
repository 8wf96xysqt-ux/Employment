#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerStateJump.h"
#include "PlayerStateDashAttack.h"
#include "PlayerStateRoll.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../MyMath/MyMath.h"
#include "../ParameterData/PlayerDataManager.h"


PlayerStateMove::PlayerStateMove()
    : m_PlayerData(nullptr)
{
}
void PlayerStateMove::Enter(Player* player)
{
    m_pPlayer = player;
    m_pNextState = this;

    m_PlayerData = PlayerDataManager::GetInstance()->GetParameter();

}

void PlayerStateMove::Update()
{
    Player* player = m_pPlayer;

    // ジャンプ
    if (player->IsJumpBuffered() && (player->IsGround() || player->GetCoyoteTime() > 0.0f))
    {
        player->ResetJumpBuffer();

        ChangeState(new PlayerStateJump());
        return;
    }

    // 攻撃
    if (Input::IsAttack())
    {
        ChangeState(new PlayerStateDashAttack());
        return;
    }

    // ロール
    if (Input::IsRolling())
    {
        ChangeState(new PlayerStateRoll());
        return;
    }

    // 移動
    if (!player->UpdateMove(m_PlayerData->moveSpeed))
    {
        ChangeState(new PlayerStateIdle());
        return;
    }

    // アニメーション
    float lx = Input::GetStickLX();
    float ly = Input::GetStickLY();

    float power = sqrtf(lx * lx + ly * ly);

    if (power < m_PlayerData->runInputThreshold)
    {
        player->PlayAnimation(PLAYER_ANIMATION_WALK,true,m_PlayerData->animSpeedWalk);
    }
    else
    {
        player->PlayAnimation(PLAYER_ANIMATION_RUN,true,m_PlayerData->animSpeedRun);
    }
}
void PlayerStateMove::Exit()
{
}