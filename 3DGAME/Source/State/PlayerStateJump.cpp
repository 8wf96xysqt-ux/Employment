#include "DxLib.h"
#include "../Player/Player.h"
#include "PlayerStateJump.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAirAttack.h"
#include "PlayerStateHover.h"
#include "PlayerStateFall.h"
#include "../Input/Input.h"
#include "../MyMath/MyMath.h"
#include "../ParameterData/PlayerDataManager.h"

PlayerStateJump::PlayerStateJump()
    : m_PlayerData(nullptr)
{
}

void PlayerStateJump::Enter(Player* player)
{
    m_pPlayer = player;
    m_pNextState = this;

    m_PlayerData = PlayerDataManager::GetInstance()->GetParameter();
    m_pPlayer->SetParameter(m_PlayerData);

    VECTOR move = m_pPlayer->GetMove();

    move.y = m_PlayerData->jumpPower;

    m_pPlayer->SetMove(move);

    m_pPlayer->SetGround(false);

    m_pPlayer->PlayAnimation(PLAYER_ANIMATION_DASHJUMP,true,m_PlayerData->animSpeedDashJump);
}

void PlayerStateJump::Update()
{
    Player* player = m_pPlayer;
    VECTOR move = player->GetMove();

    // 空中攻撃
    if (Input::IsAttack() && !player->HasAirAttacked())
    {
        ChangeState(new PlayerStateAirAttack());
        return;
    }

   
    //ホバー遷移
    if (Input::IsTriggerJump() && !m_pPlayer->HasHovered())
    {
        m_pPlayer->SetHasHovered(true);
        ChangeState(new PlayerStateHover());
        return;
    }
    

    // 空中移動
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

    // 可変ジャンプ
    if (Input::IsJump()) 
    {
        move.y += m_PlayerData->jumpHoldPower * TARGET_FPS * FPSSystem::GetDeltaTime();
    }
    else 
    {
        if (move.y > 0) 
        {
            move.y *= m_PlayerData->jumpCutRate;
        }
    }
    player->SetMove(move);

   
    // 落下
    if (move.y <= 0) 
    { 
        ChangeState(new PlayerStateFall()); return;
    }
  
}

void PlayerStateJump::Exit()
{
}