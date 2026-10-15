#include "PlayerStateDashAttack.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRoll.h"
#include "../Player/Player.h"
#include "../Attack/AttackComponent.h"
#include "../Input/Input.h"
#include "../ParameterData/AttackDataManager.h"

PlayerStateDashAttack::PlayerStateDashAttack()
    : m_IsRightPunch(true)
    , m_AttackBuffered(false)
    , m_AttackData(nullptr)
{
}

void PlayerStateDashAttack::Enter(Player* player)
{
    m_pPlayer = player;

    m_IsRightPunch = true;
    m_AttackBuffered = false;

    // Dash1‚ğæ“¾
    m_AttackData = AttackDataManager::GetInstance()->GetAttackData(PlayerAttackType::DASH_1);

    if (m_AttackData == nullptr)
    {
        ChangeState(std::make_unique<PlayerStateIdle>());
        return;
    }

    PlayAttackAnimation();
}

void PlayerStateDashAttack::Update()
{
    Player* player = m_pPlayer;

    // UŒ‚’†‚à“ü—Í•ûŒü‚ÖˆÚ“®E•ûŒü“]Š·
    player->UpdateMove(m_AttackData->moveSpeed);

    // ƒ[ƒ‹‚ÅUŒ‚ƒLƒƒƒ“ƒZƒ‹
    if (Input::IsRolling())
    {
        ChangeState(std::make_unique<PlayerStateRoll>());
        return;
    }

    // Ÿ‚ÌUŒ‚“ü—Í‚ğ—\–ñ
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

    // UŒ‚Œã”¼‚ÅŸ‚Ìƒpƒ“ƒ`
    if (rate >= m_AttackData->chainRate && m_AttackBuffered)
    {
        m_AttackBuffered = false;

        // ‰E ¨ ¶
        m_IsRightPunch = !m_IsRightPunch;

        if (m_IsRightPunch)
        {
            m_AttackData = AttackDataManager::GetInstance()->GetAttackData(PlayerAttackType::DASH_1);
        }
        else
        {
            m_AttackData = AttackDataManager::GetInstance()->GetAttackData(PlayerAttackType::DASH_2);
        }

        if (m_AttackData == nullptr)
        {
            ChangeState(std::make_unique<PlayerStateIdle>());
            return;
        }

        PlayAttackAnimation();
        return;
    }

    // UŒ‚I—¹
    if (player->GetAnimationNowTime() >= totalTime)
    {
        ChangeState(std::make_unique<PlayerStateIdle>());
        return;
    }
}

void PlayerStateDashAttack::Exit()
{
    m_pPlayer->GetAttackComponent()->EndAttack();

    // UŒ‚I—¹‚É…•½•ûŒü‚ÌˆÚ“®‚ğ’â~
    VECTOR move = m_pPlayer->GetMove();
    move.x = 0.0f;
    move.z = 0.0f;

    m_pPlayer->SetMove(move);
}

void PlayerStateDashAttack::PlayAttackAnimation()
{
    m_pPlayer->PlayAnimation(
        m_AttackData->animation,
        false,
        m_AttackData->animationSpeed
    );

    AttackComponent* attack = m_pPlayer->GetAttackComponent();

    attack->StartAttack(m_AttackData);
}