#include "AttackPlayer.h"
#include "../Blackboard/BlackBoard.h"
#include "../Core/NodeResult.h"
#include "../../Enemy/EnemyBase.h"
#include "../../Enemy/NormalEnemy.h"
#include "../../Player/Player.h"
#include "../../Collision/CollisionManager.h"
#include "../../Collision/CollisionOBB.h"
#include "../../Collision/CollisionAABB.h"
#include "../../FPS/FPS.h"
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

    // 攻撃中はノックバックの影響を受けていないか確認
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

    // 攻撃中でなければ攻撃範囲を判定
    if (!m_IsAttacking && distance > ATTACK_RANGE)
    {
        m_AttackOBB->SetActive(false);
        m_NodeResult = NodeResult::Fail;
        return;
    }

    enemy->SetMove(VGet(0.0f, 0.0f, 0.0f));
    enemy->RotateToward(atan2f(dx, dz), 0.08f);

    m_Cooldown -= FPSSystem::GetDeltaTime();

    // 攻撃中でなければ攻撃を開始
    if (!m_IsAttacking && m_Cooldown <= 0.0f)
    {
        NormalEnemy* normalEnemy = dynamic_cast<NormalEnemy*>(enemy);

        if (normalEnemy)
        {
            normalEnemy->PlayAnimation(
                NORMAL_ANIMATION_ATTACK01,
                false,
                1.0f);
        }

        m_IsAttacking = true;
        m_HasHit = false;
    }

    // 攻撃アニメーションの更新
    if (m_IsAttacking)
    {
        NormalEnemy* normalEnemy = dynamic_cast<NormalEnemy*>(enemy);

        if (!normalEnemy)
        {
            Finalize();
            m_NodeResult = NodeResult::Fail;
            return;
        }

        float totalTime = normalEnemy->GetAnimationTotalTime();
        float nowTime = normalEnemy->GetAnimationNowTime();

        if (totalTime <= 0.0f)
        {
            m_AttackOBB->SetActive(false);
            m_NodeResult = NodeResult::Running;
            return;
        }

        float startTime = totalTime * ATTACK_DAMAGE_START;
        float endTime = totalTime * ATTACK_DAMAGE_END;

        // 敵の向きに合わせて攻撃判定の位置と回転を更新
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

        // 攻撃判定が有効な時間だけ命中を調べる
        if (m_AttackOBB->IsActive() &&
            !m_HasHit &&
            player->GetAABB()->CheckOBB(m_AttackOBB))
        {
            player->TakeDamage(ATTACK_DAMAGE);
            m_HasHit = true;
        }

        // 攻撃終了
        if (nowTime >= totalTime)
        {
            m_AttackOBB->SetActive(false);
            m_IsAttacking = false;
            m_Cooldown = ATTACK_COOLDOWN;

            normalEnemy->PlayAnimation(
                NORMAL_ANIMATION_IDLE,
                true,
                0.8f);
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
