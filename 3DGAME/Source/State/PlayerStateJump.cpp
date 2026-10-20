#include "PlayerStateJump.h"
#include "../Player/Player.h"
#include "PlayerStateAirAttack.h"
#include "PlayerStateHover.h"
#include "PlayerStateFall.h"
#include "../Movement/PlayerAirMovement.h"
#include "../Input/Input.h"
#include "../FPS/FPS.h"
#include "../ParameterData/PlayerDataManager.h"

PlayerStateJump::PlayerStateJump()
    : m_PlayerData(nullptr)
{
}

void PlayerStateJump::Enter(Player* player)
{
    m_pPlayer = player;

    m_PlayerData = PlayerDataManager::GetInstance()->GetParameter();

    VECTOR move = m_pPlayer->GetMove();

    move.y = m_PlayerData->jumpPower;

    m_pPlayer->SetMove(move);

    m_pPlayer->SetGround(false);

    m_pPlayer->PlayAnimation(
        PLAYER_ANIMATION_DASHJUMP,
        true,
        m_PlayerData->animSpeedDashJump);
}

void PlayerStateJump::Update()
{
    Player* player = m_pPlayer;

    // 空中攻撃
    if (Input::IsAttack() && !player->GetAttackComponent()->HasAirAttacked())
    {
        ChangeState(std::make_unique<PlayerStateAirAttack>());
        return;
    }

    // ホバー遷移
    if (Input::IsTriggerJump() && !player->HasHovered())
    {
        player->SetHasHovered(true);
        ChangeState(std::make_unique<PlayerStateHover>());
        return;
    }

    // 空中移動
    PlayerAirMovement::Update(
        player,
        m_PlayerData->airMoveSpeed,
        m_PlayerData->moveInputThreshold);

    VECTOR move = player->GetMove();

    // 可変ジャンプ
    if (Input::IsJump())
    {
        move.y += m_PlayerData->jumpHoldPower
            * TARGET_FPS
            * FPSSystem::GetDeltaTime();
    }
    else
    {
        if (move.y > 0.0f)
        {
            move.y *= m_PlayerData->jumpCutRate;
        }
    }

    player->SetMove(move);

    // 落下
    if (move.y <= 0.0f)
    {
        ChangeState(std::make_unique<PlayerStateFall>());
    }
}

void PlayerStateJump::Exit()
{
}