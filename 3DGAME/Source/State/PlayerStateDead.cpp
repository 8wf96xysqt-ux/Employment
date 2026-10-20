#include "PlayerStateDead.h"
#include "PlayerStateIdle.h"
#include "../Player/Player.h"
#include "../GameTime/GameTime.h"
#include "../Camera/CameraManager.h"

PlayerStateDead::PlayerStateDead()
    : m_IsRespawned(false)
{
}

void PlayerStateDead::Enter(Player* player)
{
    if (player == nullptr)
    {
        return;
    }

    m_pPlayer = player;
    m_IsRespawned = false;

    // 移動を停止
    m_pPlayer->SetMove(VGet(0.0f, 0.0f, 0.0f));

    // 重力を停止
    m_pPlayer->StopGravity();

    // 死亡アニメーションを再生
    m_pPlayer->PlayAnimation(PLAYER_ANIMATION_DEAD, false, 0.5f);
}

void PlayerStateDead::Update()
{
    if (m_pPlayer == nullptr || m_IsRespawned)
    {
        return;
    }

    // ゲーム全体をスローにする
    GameTime::SetTimeScale(0.7f);

    // 死亡アニメーションが終了したらリスポーン
    if (m_pPlayer->GetAnimationNowTime() >= m_pPlayer->GetAnimationTotalTime())
    {
        m_IsRespawned = true;
        m_pPlayer->Respawn();
    }
}

void PlayerStateDead::Exit()
{
    // 通常速度に戻す
    GameTime::SetTimeScale(1.0f);
}

