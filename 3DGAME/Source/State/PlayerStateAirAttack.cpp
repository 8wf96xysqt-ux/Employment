#include "PlayerStateAirAttack.h"
#include "PlayerStateIdle.h"
#include "PlayerStateFall.h"
#include "PlayerStateRoll.h"
#include "../Player/Player.h"
#include "../Attack/AttackComponent.h"
#include "../Input/Input.h"
#include "../ParameterData/AttackDataManager.h"

constexpr int MAX_AIR_ATTACK_COUNT = 3;

PlayerStateAirAttack::PlayerStateAirAttack()
    : m_IsRightPunch(true)
    , m_AttackBuffered(false)
    , m_AttackCount(0)
    , m_AttackData(nullptr)
{
}

void PlayerStateAirAttack::Enter(Player* player)
{
    m_pPlayer = player;

    // Å‰‚ÌUŒ‚
    m_IsRightPunch = true;
    m_AttackBuffered = false;
    m_AttackCount = 1;

    // Air1‚ğæ“¾
    m_AttackData = AttackDataManager::GetInstance()->GetAttackData(PlayerAttackType::AIR_1);

    if (m_AttackData == nullptr)
    {
        ChangeState(std::make_unique<PlayerStateFall>());
        return;
    }

    PlayAttackAnimation();

    // ‹ó’†UŒ‚ŠJn‚Ì’â~
    player->SetMove(VGet(0.0f, 0.0f, 0.0f));
    player->StopGravity();

    // ‹ó’†UŒ‚Ï‚İ
    player->GetAttackComponent()->SetHasAirAttacked(true);
}

void PlayerStateAirAttack::Update()
{
    Player* player = m_pPlayer;

    // ƒ[ƒ‹‚ÅUŒ‚ƒLƒƒƒ“ƒZƒ‹
    if (Input::IsRolling())
    {
        ChangeState(std::make_unique<PlayerStateRoll>());
        return;
    }

    // UŒ‚“ü—Í‚ğ—\–ñ
    if (Input::IsAttack())
    {
        m_AttackBuffered = true;
    }

    // ‹ó’†UŒ‚’†‚Í’â~
    player->StopGravity();
    player->SetMove(VGet(0.0f, 0.0f, 0.0f));

    float totalTime = player->GetAnimationTotalTime();

    if (totalTime <= 0.0f)
    {
        ChangeState(std::make_unique<PlayerStateFall>());
        return;
    }

    float rate = player->GetAnimationNowTime() / totalTime;

    // Ÿ‚ÌUŒ‚‚Ö
    if (rate >= m_AttackData->chainRate && m_AttackBuffered && m_AttackCount < MAX_AIR_ATTACK_COUNT)
    {
        m_AttackBuffered = false;

        // UŒ‚‰ñ”‚ğ‘‚â‚·
        m_AttackCount++;

        // ‰E ¨ ¶ ¨ ‰E
        m_IsRightPunch = !m_IsRightPunch;

        // Air1 / Air2‚ğæ“¾
        PlayerAttackType attackType = m_IsRightPunch ? PlayerAttackType::AIR_1 : PlayerAttackType::AIR_2;

        m_AttackData = AttackDataManager::GetInstance()->GetAttackData(attackType);

        if (m_AttackData == nullptr)
        {
            ChangeState(std::make_unique<PlayerStateFall>());
            return;
        }

        PlayAttackAnimation();
        return;
    }

    // ’…’n
    if (player->IsGround())
    {
        ChangeState(std::make_unique<PlayerStateIdle>());
        return;
    }

    // UŒ‚I—¹
    if (player->GetAnimationNowTime() >= totalTime)
    {
        ChangeState(std::make_unique<PlayerStateFall>());
        return;
    }
}

void PlayerStateAirAttack::Exit()
{
    m_pPlayer->GetAttackComponent()->EndAttack();
    m_pPlayer->ResetGravity();
}

void PlayerStateAirAttack::PlayAttackAnimation()
{
    m_pPlayer->PlayAnimation(
        m_AttackData->animation,
        false,
        m_AttackData->animationSpeed
    );

    AttackComponent* attack = m_pPlayer->GetAttackComponent();

    attack->StartAttack(m_AttackData);
}