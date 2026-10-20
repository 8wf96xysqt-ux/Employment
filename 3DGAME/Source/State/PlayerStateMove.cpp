#include "PlayerStateMove.h"
#include "PlayerStateIdle.h"
#include "PlayerStateJump.h"
#include "PlayerStateDashAttack.h"
#include "PlayerStateRoll.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../Movement/PlayerGroundMovement.h"
#include "../ParameterData/PlayerDataManager.h"

#include <cmath>

PlayerStateMove::PlayerStateMove()
    : m_PlayerData(nullptr)
{
}

void PlayerStateMove::Enter(Player* player)
{
    m_pPlayer = player;

    m_PlayerData = PlayerDataManager::GetInstance()->GetParameter();
}

void PlayerStateMove::Update()
{
    Player* player = m_pPlayer;

    // ジャンプ
    if (player->IsJumpBuffered() && (player->IsGround() || player->GetCoyoteTime() > 0.0f))
    {
        player->ResetJumpBuffer();

        ChangeState(std::make_unique<PlayerStateJump>());
        return;
    }

    // 攻撃
    if (Input::IsAttack())
    {
        ChangeState(std::make_unique<PlayerStateDashAttack>());
        return;
    }

    // ロール
    if (Input::IsRolling())
    {
        ChangeState(std::make_unique<PlayerStateRoll>());
        return;
    }

    // 移動
    if (!PlayerGroundMovement::Update(
        player,
        m_PlayerData->moveSpeed,
        m_PlayerData->moveInputThreshold))
    {
        ChangeState(std::make_unique<PlayerStateIdle>());
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