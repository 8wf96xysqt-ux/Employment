#include "PlayerStateHover.h"
#include "PlayerStateFall.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAirAttack.h"
#include "../Player/Player.h"
#include "../Movement/PlayerAirMovement.h"
#include "../Input/Input.h"
#include "../FPS/FPS.h"
#include "../Collision/CollisionAABB.h"
#include "../ParameterData/PlayerDataManager.h"
#include "../ParameterData/AttackDataManager.h"

PlayerStateHover::PlayerStateHover()
    : m_StartY(0.0f)
    , m_HoverTime(0.0f)
    , m_HoverTimeMax(0.0f)
    , m_IsRising(false)
    , m_PlayerData(nullptr)
{
}

void PlayerStateHover::Enter(Player* player)
{
    m_pPlayer = player;
    m_PlayerData = PlayerDataManager::GetInstance()->GetParameter();

    const AttackData* attackData = AttackDataManager::GetInstance()->GetAttackData(PlayerAttackType::HOVER);

    if (!m_PlayerData || !attackData)
    {
        ChangeState(std::make_unique<PlayerStateFall>());
        return;
    }

    m_StartY = m_pPlayer->GetPos().y;
    m_IsRising = true;
    m_HoverTime = 0.0f;
    m_HoverTimeMax = m_PlayerData->hoverTime;

    VECTOR move = m_pPlayer->GetMove();
    move.y = m_PlayerData->hoverRiseSpeed;
    m_pPlayer->SetMove(move);

    m_pPlayer->PlayAnimation(PLAYER_ANIMATION_HOVER,true,m_PlayerData->animSpeedHover);

    // ビームの当たり判定とエフェクトを開始
    m_HoverBeam.Start(m_pPlayer, attackData);
}

void PlayerStateHover::Update()
{
    if (!m_PlayerData)
    {
        return;
    }

    Player* player = m_pPlayer;

    // 空中攻撃へ移行
    if (Input::IsAttack() &&
        !player->GetAttackComponent()->HasAirAttacked())
    {
        ChangeState(std::make_unique<PlayerStateAirAttack>());
        return;
    }

    // ホバー移動を更新。時間切れでステート遷移した場合は終了
    if (UpdateHoverMovement())
    {
        return;
    }

    // ビームの位置とエフェクトを更新
    m_HoverBeam.Update();

    // 地面に着いたら待機状態へ移行
    if (player->IsGround())
    {
        player->SetHasHovered(false);
        ChangeState(std::make_unique<PlayerStateIdle>());
        return;
    }

    // ジャンプ入力を離したら落下状態へ移行
    if (!Input::IsJump())
    {
        ChangeState(std::make_unique<PlayerStateFall>());
        return;
    }
}

bool PlayerStateHover::UpdateHoverMovement()
{
    Player* player = m_pPlayer;

    VECTOR move = player->GetMove();
    float currentY = player->GetPos().y;

    if (m_IsRising)
    {
        move.y = m_PlayerData->hoverRiseSpeed;

        // 設定した高さまで上昇したらホバー状態へ
        if (currentY >= m_StartY + m_PlayerData->hoverRiseHeight)
        {
            m_IsRising = false;
            move.y = 0.0f;
        }
    }
    else
    {
        move.y = 0.0f;
        m_HoverTime += FPSSystem::GetDeltaTime();

        // ホバー時間が終了したら落下状態へ
        if (m_HoverTime >= m_HoverTimeMax)
        {
            ChangeState(std::make_unique<PlayerStateFall>());
            return true;
        }
    }

    player->SetMove(move);

    PlayerAirMovement::Update(player,m_PlayerData->hoverMoveSpeed,m_PlayerData->moveInputThreshold);

    return false;
}

void PlayerStateHover::Exit()
{
    // ビームのエフェクトと当たり判定を終了
    m_HoverBeam.Fin();
}

bool PlayerStateHover::CheckAttackHit(CollisionAABB* target)
{
    return m_HoverBeam.CheckAttackHit(target);
}