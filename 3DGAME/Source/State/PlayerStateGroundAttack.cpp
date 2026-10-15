#include "PlayerStateGroundAttack.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRoll.h"
#include "../Player/Player.h"
#include "../Attack/AttackComponent.h"
#include "../Input/Input.h"
#include "../ParameterData/AttackDataManager.h"

PlayerStateGroundAttack::PlayerStateGroundAttack()
    : m_IsRightPunch(true)
    , m_AttackBuffered(false)
    , m_AttackData(nullptr)
{
}

void PlayerStateGroundAttack::Enter(Player* player)
{
    m_pPlayer = player;

    // 最初は右パンチ
    m_IsRightPunch = true;
    m_AttackBuffered = false;

    // Ground1を取得
    m_AttackData = AttackDataManager::GetInstance()->GetAttackData(PlayerAttackType::GROUND_1);

    if (m_AttackData == nullptr)
    {
        ChangeState(std::make_unique<PlayerStateIdle>());
        return;
    }

    PlayAttackAnimation();
}

void PlayerStateGroundAttack::Update()
{
    Player* player = m_pPlayer;

    // ロールで攻撃キャンセル
    if (Input::IsRolling())
    {
        ChangeState(std::make_unique<PlayerStateRoll>());
        return;
    }

    // 次の攻撃入力を予約
    if (Input::IsAttack())
    {
        m_AttackBuffered = true;
    }

    float totalTime = player->GetAnimationTotalTime();

    if (totalTime <= 0.0f)
    {
        ChangeState(std::make_unique<PlayerStateIdle>());
        return;
    }

    float rate = player->GetAnimationNowTime() / totalTime;

    // 攻撃後半で次のパンチへ
    if (rate >= m_AttackData->chainRate && m_AttackBuffered)
    {
        m_AttackBuffered = false;

        // 右 → 左
        m_IsRightPunch = !m_IsRightPunch;

        // Ground1 / Ground2を取得
        if (m_IsRightPunch)
        {
            m_AttackData = AttackDataManager::GetInstance()->GetAttackData(PlayerAttackType::GROUND_1);
        }
        else
        {
            m_AttackData = AttackDataManager::GetInstance()->GetAttackData(PlayerAttackType::GROUND_2);
        }

        if (m_AttackData == nullptr)
        {
            ChangeState(std::make_unique<PlayerStateIdle>());
            return;
        }

        PlayAttackAnimation();
        return;
    }

    // 攻撃終了
    if (player->GetAnimationNowTime() >= totalTime)
    {
        ChangeState(std::make_unique<PlayerStateIdle>());
        return;
    }
}

void PlayerStateGroundAttack::Exit()
{
    m_pPlayer->GetAttackComponent()->EndAttack();
}

void PlayerStateGroundAttack::PlayAttackAnimation()
{
    m_pPlayer->PlayAnimation(
        m_AttackData->animation,
        false,
        m_AttackData->animationSpeed
    );

    AttackComponent* attack = m_pPlayer->GetAttackComponent();

    attack->StartAttack(m_AttackData);
}