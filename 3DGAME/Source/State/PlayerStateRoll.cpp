#include "PlayerStateRoll.h"
#include "PlayerStateIdle.h"
#include "../Player/Player.h"
#include "../Attack/AttackComponent.h"
#include "../Input/Input.h"
#include "../FPS/FPS.h"
#include "../ParameterData/PlayerDataManager.h"
#include <cmath>

PlayerStateRoll::PlayerStateRoll()
    : m_RollTime(0.0f)
    , m_RollTimeMax(0.0f)
    , m_RollDir(VGet(0.0f, 0.0f, 0.0f))
    , m_PlayerData(nullptr)
{
}

void PlayerStateRoll::Enter(Player* player)
{
    m_pPlayer = player;

    m_PlayerData = PlayerDataManager::GetInstance()->GetParameter();

    // ローリング時間
    m_RollTime = m_PlayerData->rollTime;
    m_RollTimeMax = m_PlayerData->rollTime;

    // 無敵開始
    m_pPlayer->StartInvincible(m_RollTimeMax);

    // 攻撃キャンセル
    m_pPlayer->GetAttackComponent()->EndAttack();

    // 向いている方向へローリング
    VECTOR rot = m_pPlayer->GetRot();

    m_RollDir.x = sinf(rot.y);
    m_RollDir.y = 0.0f;
    m_RollDir.z = cosf(rot.y);

    // ローリングアニメーション
    m_pPlayer->PlayAnimation(
        PLAYER_ANIMATION_ROLLING,
        false,
        m_PlayerData->animSpeedRolling
    );
}

void PlayerStateRoll::Update()
{
    // ローリング時間を減らす
    m_RollTime -= FPSSystem::GetDeltaTime();

    // ローリング移動
    VECTOR move = m_pPlayer->GetMove();

    move.x = m_RollDir.x * m_PlayerData->rollSpeed;
    move.z = m_RollDir.z * m_PlayerData->rollSpeed;

    m_pPlayer->SetMove(move);

    // ローリング終了
    if (m_RollTime <= 0.0f)
    {
        ChangeState(std::make_unique<PlayerStateIdle>());
        return;
    }
}

void PlayerStateRoll::Exit()
{
    // 移動停止
    VECTOR move = m_pPlayer->GetMove();

    move.x = 0.0f;
    move.z = 0.0f;

    m_pPlayer->SetMove(move);

    // 無敵終了
    m_pPlayer->EndInvincible();
}