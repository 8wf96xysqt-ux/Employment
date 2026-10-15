#include "AttackComponent.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionOBB.h"
#include "../Collision/CollisionAABB.h"
#include "../ParameterData/AttackParameter.h"
#include <math.h>

AttackComponent::AttackComponent()
    : m_AttackOBB(nullptr)
    , m_AttackData(nullptr)
    , m_IsAttack(false)
    , m_HasAirAttacked(false)
{
}

AttackComponent::~AttackComponent()
{
}

void AttackComponent::Init(VECTOR* targetPos)
{
    m_AttackOBB = CollisionManager::GetInstance()->CreateOBB();

    if (!m_AttackOBB)
    {
        return;
    }

    m_AttackOBB->SetTargetPos(targetPos);
    m_AttackOBB->SetLocalPos(VGet(0.0f, 1.0f, 0.0f));
    m_AttackOBB->SetSize(VGet(0.8f, 0.8f, 0.8f));
    m_AttackOBB->SetRotation(0.0f);
    m_AttackOBB->SetActive(false);
}

void AttackComponent::Fin()
{
    if (m_AttackOBB)
    {
        CollisionManager::GetInstance()->DeleteOBB(m_AttackOBB);
        m_AttackOBB = nullptr;
    }

    m_AttackData = nullptr;
    m_IsAttack = false;
    m_HasAirAttacked = false;
}

void AttackComponent::StartAttack()
{
    m_IsAttack = true;
}

// 攻撃データを設定して攻撃開始
void AttackComponent::StartAttack(const AttackData* data)
{
    SetAttackData(data);
    StartAttack();
}

void AttackComponent::EndAttack()
{
    m_IsAttack = false;
}


void AttackComponent::SetAttackData(const AttackData* data)
{
    m_AttackData = data;
}

void AttackComponent::Update(float animationTime, float rotationY)
{
    if (!m_AttackOBB)
    {
        return;
    }

    if (!m_IsAttack || !m_AttackData)
    {
        m_AttackOBB->SetActive(false);
        return;
    }

    if (animationTime < m_AttackData->start ||
        animationTime > m_AttackData->start + m_AttackData->duration)
    {
        m_AttackOBB->SetActive(false);
        return;
    }

    m_AttackOBB->SetActive(true);

    m_AttackOBB->SetRotation(rotationY);

    m_AttackOBB->SetSize(VGet(
        m_AttackData->hitboxSizeX,
        m_AttackData->hitboxSizeY,
        m_AttackData->hitboxSizeZ));

    VECTOR pos;

    pos.x = sinf(rotationY) * m_AttackData->hitboxOffsetZ
        + cosf(rotationY) * m_AttackData->hitboxOffsetX;

    pos.y = m_AttackData->hitboxOffsetY;

    pos.z = cosf(rotationY) * m_AttackData->hitboxOffsetZ
        - sinf(rotationY) * m_AttackData->hitboxOffsetX;

    m_AttackOBB->SetLocalPos(pos);
}

bool AttackComponent::CheckHit(CollisionAABB* target) const
{
    if (!target)
    {
        return false;
    }

    if (!m_AttackOBB || !m_AttackOBB->IsActive())
    {
        return false;
    }

    return target->CheckOBB(m_AttackOBB);
}