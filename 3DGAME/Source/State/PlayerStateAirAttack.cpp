#include "PlayerStateAirAttack.h"
#include "PlayerStateIdle.h"
#include "PlayerStateFall.h"
#include "PlayerStateRoll.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../GameSetting/GameSetting.h"
#include "../ParameterData/AttackDataManager.h"

constexpr int MAX_AIR_ATTACK_COUNT = 3;

PlayerStateAirAttack::PlayerStateAirAttack()
    : m_IsRightPunch(true)
    , m_AttackBuffered(false)
    , m_AttackCount(0)
    , m_AttackData(nullptr)
{
}

PlayerStateAirAttack::~PlayerStateAirAttack()
{
}

void PlayerStateAirAttack::Enter(Player* player)
{
    m_pPlayer = player;
    m_pNextState = this;

     // ‹ó’†UŒ‚ŠJŽn
    m_pPlayer->StartAttack();

    // Å‰‚ÌUŒ‚
    m_IsRightPunch = true;
    m_AttackBuffered = false;
    m_AttackCount = 1;

    // Air1‚ðŽæ“¾
    m_AttackData = AttackDataManager::GetInstance()->GetAttackData(PlayerAttackType::AIR_1);

    if (m_AttackData == nullptr)
    {
        ChangeState(new PlayerStateFall());
        return;
    }

    PlayAttackAnimation();

    // ‹ó’†UŒ‚ŠJŽnŽž‚Ì’âŽ~
    m_pPlayer->SetMove(VGet(0.0f, 0.0f, 0.0f));
    m_pPlayer->StopGravity();

    // ‹ó’†UŒ‚Ï‚Ý
    m_pPlayer->SetHasAirAttacked(true);
}

void PlayerStateAirAttack::Update()
{

    // ƒ[ƒ‹‚ÅUŒ‚ƒLƒƒƒ“ƒZƒ‹
    if (Input::IsRolling())
    {
        ChangeState(new PlayerStateRoll());
        return;
    }

    // UŒ‚“ü—Í‚ð—\–ñ
    if (Input::IsAttack())
    {
        m_AttackBuffered = true;
    }

    // ‹ó’†UŒ‚ŠJŽnŽž‚Ì’âŽ~ŽžŠÔ
    m_pPlayer->StopGravity();
    m_pPlayer->SetMove(VGet(0.0f, 0.0f, 0.0f));
    

    float totalTime = m_pPlayer->GetAnimationTotalTime();

    if (totalTime <= 0.0f)
    {
        ChangeState(new PlayerStateFall());
        return;
    }

    float rate = m_pPlayer->GetAnimationNowTime() / totalTime;

    // ŽŸ‚ÌUŒ‚‚Ö
    if (rate >= m_AttackData->chainRate && m_AttackBuffered && m_AttackCount < MAX_AIR_ATTACK_COUNT)
    {
        m_AttackBuffered = false;

        // UŒ‚‰ñ”‚ð‘‚â‚·
        m_AttackCount++;

        // ‰E ¨ ¶ ¨ ‰E
        m_IsRightPunch = !m_IsRightPunch;

        // Air1 / Air2‚ðŽæ“¾
        if (m_IsRightPunch)
        {
            m_AttackData = AttackDataManager::GetInstance()->GetAttackData(PlayerAttackType::AIR_1);
        }
        else
        {
            m_AttackData = AttackDataManager::GetInstance()->GetAttackData(PlayerAttackType::AIR_2);
        }

        if (m_AttackData == nullptr)
        {
            ChangeState(new PlayerStateFall());
            return;
        }

        PlayAttackAnimation();
        return;
    }

    // ’…’n
    if (m_pPlayer->IsGround())
    {
        ChangeState(new PlayerStateIdle());
        return;
    }

    // UŒ‚I—¹
    if (m_pPlayer->GetAnimationNowTime() >= totalTime)
    {
        ChangeState(new PlayerStateFall());
        return;
    }
}

void PlayerStateAirAttack::Exit()
{
    m_pPlayer->EndAttack();
    m_pPlayer->ResetGravity();
}
void PlayerStateAirAttack::PlayAttackAnimation()
{
    m_pPlayer->PlayAnimation(m_AttackData->animation,false, m_AttackData->animationSpeed );

    m_pPlayer->SetAttackData(m_AttackData);

    m_pPlayer->StartAttack();
}