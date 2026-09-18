#include "PlayerStateDashAttack.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRoll.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../ParameterData/AttackDataManager.h"


PlayerStateDashAttack::PlayerStateDashAttack()
    : m_IsRightPunch(true)
    , m_AttackBuffered(false)
    , m_AttackData(nullptr)
{
}

PlayerStateDashAttack::~PlayerStateDashAttack()
{
}

void PlayerStateDashAttack::Enter(Player* player)
{
    m_pPlayer = player;
    m_pNextState = this;

    m_IsRightPunch = true;
    m_AttackBuffered = false;

    // Dash1‚ðŽæ“¾
    m_AttackData = AttackDataManager::GetInstance()->GetAttackData(PlayerAttackType::DASH_1);

    if (m_AttackData == nullptr)
    {
        ChangeState(new PlayerStateIdle());
        return;
    }

    PlayAttackAnimation();
}

void PlayerStateDashAttack::Update()
{
    // UŒ‚’†‚àŽ©—R‚É•ûŒü“]Š·
    if (!m_pPlayer->UpdateMove(m_AttackData->moveSpeed))
    {
        ChangeState(new PlayerStateIdle());
        return;
    }

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

    // UŒ‚Œã”¼‚ÅŽŸ‚Ìƒpƒ“ƒ`
    if (rate >= m_AttackData->chainRate && m_AttackBuffered)
    {
        m_AttackBuffered = false;

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

void PlayerStateDashAttack::Exit()
{
    m_pPlayer->EndAttack();

    VECTOR move = m_pPlayer->GetMove();
    move.x = 0.0f;
    move.z = 0.0f;

    m_pPlayer->SetMove(move);
}

void PlayerStateDashAttack::PlayAttackAnimation()
{
    m_pPlayer->PlayAnimation( m_AttackData->animation,  false, m_AttackData->animationSpeed);

    m_pPlayer->SetAttackData(m_AttackData);

    m_pPlayer->StartAttack();
}