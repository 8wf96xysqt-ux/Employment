#include "PlayerStateFall.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAirAttack.h"
#include "PlayerStateHover.h"
#include "../Player/Player.h"
#include "../Movement/PlayerAirMovement.h"
#include "../FPS/FPS.h"
#include "../Input/Input.h"
#include "../ParameterData/PlayerDataManager.h"

PlayerStateFall::PlayerStateFall()
    : m_FallAnimDelay(0.0f)
    , m_PlayerData(nullptr)
{
}

void PlayerStateFall::Enter(Player* player)
{
    m_pPlayer = player;
    m_PlayerData = PlayerDataManager::GetInstance()->GetParameter();

    m_FallAnimDelay = m_PlayerData->fallAnimDelay;
}

void PlayerStateFall::Update()
{
    if (m_pPlayer == nullptr)
    {
        return;
    }

    Player* player = m_pPlayer;

    // 空中攻撃
    if (Input::IsAttack() && !player->GetAttackComponent()->HasAirAttacked())
    {
        ChangeState(std::make_unique<PlayerStateAirAttack>());
        return;
    }

    // 着地
    if (player->IsGround())
    {
        player->SetHasHovered(false);
        ChangeState(std::make_unique<PlayerStateIdle>());
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
    PlayerAirMovement::Update(player,m_PlayerData->airMoveSpeed,m_PlayerData->moveInputThreshold);

    // Fallアニメーション開始まで待つ
    if (m_FallAnimDelay > 0.0f)
    {
        m_FallAnimDelay -= FPSSystem::GetDeltaTime();

        if (m_FallAnimDelay > 0.0f)
        {
            return;
        }

        m_FallAnimDelay = 0.0f;
    }

    player->PlayAnimation(PLAYER_ANIMATION_FALL,true,m_PlayerData->animSpeedFall);
}

void PlayerStateFall::Exit()
{
}