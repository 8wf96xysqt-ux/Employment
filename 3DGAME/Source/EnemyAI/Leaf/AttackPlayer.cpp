#include "AttackPlayer.h"
#include "../Blackboard/BlackBoard.h"
#include "../Core/NodeResult.h"
#include "../../Enemy/EnemyBase.h"
#include "../../Player/Player.h"
#include "../../Collision/CollisionManager.h"
#include "../../Collision/CollisionOBB.h"
#include "../../Collision/CollisionAABB.h"
#include "../../FPS/FPS.h"
#include "../../Animation/NormalEnemyAnimParam..h"
#include <cmath>

namespace
{
    constexpr float ATTACK_RANGE = 1.5f;
    constexpr float ATTACK_COOLDOWN = 0.8f;
    constexpr float ATTACK_DAMAGE_START = 0.3f;
    constexpr float ATTACK_DAMAGE_END = 0.6f;
    constexpr int ATTACK_DAMAGE = 1;

    constexpr float ATTACK_OFFSET_X = 0.0f;
    constexpr float ATTACK_OFFSET_Y = 0.8f;
    constexpr float ATTACK_OFFSET_Z = 0.8f;

    const VECTOR ATTACK_SIZE = VGet(1.0f, 1.0f, 1.0f);
}

AttackPlayer::AttackPlayer(BlackBoard* blackBoard)
    : LeafNodeBase(blackBoard, "AttackPlayer")
    , m_AttackOBB(nullptr)
    , m_IsAttacking(false)
    , m_HasHit(false)
    , m_Cooldown(0.0f)
{
    m_AttackOBB = CollisionManager::GetInstance()->CreateOBB();

    if (m_AttackOBB)
    {
        m_AttackOBB->SetSize(ATTACK_SIZE);
        m_AttackOBB->SetLocalPos(VGet(
            ATTACK_OFFSET_X,
            ATTACK_OFFSET_Y,
            ATTACK_OFFSET_Z));
        m_AttackOBB->SetActive(false);
    }
}

AttackPlayer::~AttackPlayer()
{
    Finalize();

    if (m_AttackOBB)
    {
        CollisionManager::GetInstance()->DeleteOBB(m_AttackOBB);
        m_AttackOBB = nullptr;
    }
}

void AttackPlayer::Init()
{
    NodeBase::Init();
}

void AttackPlayer::Tick()
{
    SetCurrentNode();

    EnemyBase* enemy = m_BlackBoard->GetValue<EnemyBase*>("Enemy");
    Player* player = m_BlackBoard->GetValue<Player*>("Player");

    if (!enemy || !player || !m_AttackOBB)
    {
        if (m_AttackOBB)
        {
            m_AttackOBB->SetActive(false);
        }

        m_NodeResult = NodeResult::Fail;
        return;
    }

    // çUåÇíÜÇÕÉmÉbÉNÉoÉbÉNÇÃâeãøÇéÛÇØÇƒÇ¢Ç»Ç¢Ç©ämîF
    if (enemy->IsKnockback())
    {
        Finalize();
        m_NodeResult = NodeResult::Fail;
        return;
    }

    VECTOR enemyPos = enemy->GetPos();
    VECTOR playerPos = player->GetPos();

    float dx = playerPos.x - enemyPos.x;
    float dz = playerPos.z - enemyPos.z;
    float distance = sqrtf(dx * dx + dz * dz);

    // çUåÇíÜÇ≈Ç»ÇØÇÍÇŒçUåÇîÕàÕÇîªíË
    if (!m_IsAttacking && distance > ATTACK_RANGE)
    {
        m_AttackOBB->SetActive(false);
        m_NodeResult = NodeResult::Fail;
        return;
    }

    enemy->SetMove(VGet(0.0f, 0.0f, 0.0f));
    enemy->RotateToward(atan2f(dx, dz), 0.08f);

    m_Cooldown -= FPSSystem::GetDeltaTime();

    // çUåÇíÜÇ≈Ç»ÇØÇÍÇŒçUåÇÇäJén
    if (!m_IsAttacking && m_Cooldown <= 0.0f)
    {
        enemy->PlayAnimation(NORMAL_ANIMATION_ATTACK01, false, 0.8f);

        m_IsAttacking = true;
        m_HasHit = false;
    }

    // çUåÇÉAÉjÉÅÅ[ÉVÉáÉìÇÃçXêV
    if (m_IsAttacking)
    {
        float totalTime = enemy->GetAnimationTotalTime();
        float nowTime = enemy->GetAnimationNowTime();

        if (totalTime <= 0.0f)
        {
            m_AttackOBB->SetActive(false);
            m_NodeResult = NodeResult::Running;
            return;
        }

        float startTime = totalTime * ATTACK_DAMAGE_START;
        float endTime = totalTime * ATTACK_DAMAGE_END;

        // ìGÇÃå¸Ç´Ç…çáÇÌÇπÇƒçUåÇîªíËÇÃà íuÇ∆âÒì]ÇçXêV
        float rotationY = enemy->GetRot().y;

        VECTOR localPos = VGet(
            sinf(rotationY) * ATTACK_OFFSET_Z
            + cosf(rotationY) * ATTACK_OFFSET_X,
            ATTACK_OFFSET_Y,
            cosf(rotationY) * ATTACK_OFFSET_Z
            - sinf(rotationY) * ATTACK_OFFSET_X
        );

        m_AttackOBB->SetTargetPos(enemy->GetPosPtr());
        m_AttackOBB->SetLocalPos(localPos);
        m_AttackOBB->SetRotation(rotationY);
        m_AttackOBB->SetActive(
            nowTime >= startTime && nowTime <= endTime);

        // çUåÇîªíËÇ™óLå¯Ç»éûä‘ÇæÇØñΩíÜÇí≤Ç◊ÇÈ
        if (m_AttackOBB->IsActive()
            && !m_HasHit
            && player->GetAABB()->CheckOBB(m_AttackOBB))
        {
            player->TakeDamage(ATTACK_DAMAGE);
            m_HasHit = true;
        }

        // çUåÇèIóπ
        if (nowTime >= totalTime)
        {
            m_AttackOBB->SetActive(false);
            m_IsAttacking = false;
            m_Cooldown = ATTACK_COOLDOWN;

            enemy->PlayAnimation(NORMAL_ANIMATION_IDLE, true, 0.8f);
        }
    }

    m_NodeResult = NodeResult::Running;
}

void AttackPlayer::Finalize()
{
    if (m_AttackOBB)
    {
        m_AttackOBB->SetActive(false);
    }

    m_IsAttacking = false;
    NodeBase::Finalize();
}