#include "PlayerStateGroundAttack.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRoll.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../ParameterData/AttackDataManager.h"

PlayerStateGroundAttack::PlayerStateGroundAttack()
    : m_IsRightPunch(true)
    , m_AttackBuffered(false)
    , m_AttackData(nullptr)
{
}

PlayerStateGroundAttack::~PlayerStateGroundAttack()
{
}

void PlayerStateGroundAttack::Enter(Player* player)
{
    m_pPlayer = player;
    m_pNextState = this;

    // Å‰‚Í‰Eƒpƒ“ƒ`
    m_IsRightPunch = true;
    m_AttackBuffered = false;

    // Ground1‚ðŽæ“¾
    m_AttackData = AttackDataManager::GetInstance()->GetAttackData(PlayerAttackType::GROUND_1);

    if (m_AttackData == nullptr)
    {
        ChangeState(new PlayerStateIdle());
        return;
    }

    PlayAttackAnimation();
}

void PlayerStateGroundAttack::Update()
{

    // ƒ[ƒ‹‚ÅUŒ‚ƒLƒƒƒ“ƒZƒ‹
    if (Input::IsRolling())
    {
        ChangeState(new PlayerStateRoll());
        return;
    }

    // ŽŸ‚ÌUŒ‚“ü—Í‚ð—\–ñ
    if (Input::IsAttack())
    {
        m_AttackBuffered = true;
    }

    float totalTime = m_pPlayer->GetAnimationTotalTime();

    if (totalTime <= 0.0f)
    {
        ChangeState(new PlayerStateIdle());
        return;
    }

    float rate = m_pPlayer->GetAnimationNowTime() / totalTime;

    // UŒ‚Œã”¼‚ÅŽŸ‚Ìƒpƒ“ƒ`‚Ö
    if (rate >= m_AttackData->chainRate && m_AttackBuffered)
    {
        m_AttackBuffered = false;

        // ‰E ¨ ¶
        m_IsRightPunch = !m_IsRightPunch;

        // Ground1 / Ground2‚ðŽæ“¾
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
            ChangeState(new PlayerStateIdle());
            return;
        }

        PlayAttackAnimation();
        return;
    }

    // UŒ‚I—¹
    if (m_pPlayer->GetAnimationNowTime() >= totalTime)
    {
        ChangeState(new PlayerStateIdle());
        return;
    }
}

void PlayerStateGroundAttack::Exit()
{
    m_pPlayer->EndAttack();
}

void PlayerStateGroundAttack::PlayAttackAnimation()
{
    m_pPlayer->PlayAnimation( m_AttackData->animation, false, m_AttackData->animationSpeed );

    m_pPlayer->SetAttackData(m_AttackData);

    m_pPlayer->StartAttack();
}