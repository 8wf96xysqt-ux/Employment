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
    m_pPlayer->SetParameter(m_PlayerData);
    m_FallAnimDelay =  m_PlayerData -> fallAnimDelay;
}

void PlayerStateFall::Update()
{
    if (m_pPlayer == nullptr)
    {
        return;
    }

    // 空中攻撃
    if (Input::IsAttack() && !m_pPlayer->HasAirAttacked())
    {
        ChangeState(new PlayerStateAirAttack());
        return;
    }

    if (m_pPlayer->IsGround())
    {
        m_pPlayer->SetHasHovered(false);
        ChangeState(new PlayerStateIdle());
        return;
    }

    //ホバー遷移
    if (Input::IsTriggerJump() && !m_pPlayer->HasHovered())
    {
        m_pPlayer->SetHasHovered(true);
        ChangeState(new PlayerStateHover());
        return;
    }

    VECTOR move = m_pPlayer->GetMove();
    VECTOR airMove = m_pPlayer->GetCameraMoveInput();

    float length = MyMath::VecLong(airMove);

    if (length > m_PlayerData->moveInputThreshold)
    {
        airMove = MyMath::VecNormalize(airMove);

        move.x = airMove.x * m_PlayerData->airMoveSpeed;
        move.z = airMove.z * m_PlayerData->airMoveSpeed;

        m_pPlayer->SetTargetYaw(atan2f(airMove.x, airMove.z));

        m_pPlayer->DirectionRot();
    }
    else
    {
        move.x = 0.0f;
        move.z = 0.0f;
    }

    m_pPlayer->SetMove(move);

    // Fallアニメーション開始まで待つ
    if (m_FallAnimDelay > 0.0f)
    {
        m_FallAnimDelay -= FPSSystem::GetDeltaTime();

        if (m_FallAnimDelay <= 0.0f)
        {
            m_FallAnimDelay = 0.0f;

            m_pPlayer->PlayAnimation( PLAYER_ANIMATION_FALL, true, m_PlayerData->animSpeedFall);
        }
    }
    else
    {
        m_pPlayer->PlayAnimation(  PLAYER_ANIMATION_FALL,  true, m_PlayerData->animSpeedFall);
    }
}

void PlayerStateFall::Exit()
{
}