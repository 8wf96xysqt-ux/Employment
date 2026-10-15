#include "PlayerStateFall.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAirAttack.h"
#include "PlayerStateHover.h"
#include "../Player/Player.h"
#include "../MyMath/MyMath.h"
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
    VECTOR move = player->GetMove();
    VECTOR airMove = player->GetCameraMoveInput();

    float length = MyMath::VecLong(airMove);

    if (length > m_PlayerData->moveInputThreshold)
    {
        airMove = MyMath::VecNormalize(airMove);

        move.x = airMove.x * m_PlayerData->airMoveSpeed;
        move.z = airMove.z * m_PlayerData->airMoveSpeed;

        player->SetTargetYaw(atan2f(airMove.x, airMove.z));
        player->DirectionRot();
    }
    else
    {
        move.x = 0.0f;
        move.z = 0.0f;
    }

    player->SetMove(move);

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