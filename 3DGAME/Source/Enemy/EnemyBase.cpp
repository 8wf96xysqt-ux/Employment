#include "EnemyBase.h"
#include "../FPS/FPS.h"
#include "../GameSetting/GameSetting.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../State/PlayerStateBase.h"
#include "../StageObject/StageObject.h"
#include "../Collision/CollisionAABB.h"
#include "../Camera/CameraManager.h"
#include <cmath>


namespace
{
    constexpr float CIRCLE_MOVE_SPEED = 0.02f;
    constexpr float PATROL_MOVE_SPEED = 0.025f;
    constexpr float KNOCKBACK_TIME = 0.35f;
    constexpr float KNOCKBACK_GRAVITY = 0.02f;
}

EnemyBase::EnemyBase()
    : m_KnockbackVelocity(VGet(0.0f, 0.0f, 0.0f))
    , m_KnockbackTime(0.0f)
    , m_IsKnockback(false)
{
    m_BasePos = VGet(0.0f, 0.0f, 0.0f);
    m_MovementSpeed = PATROL_MOVE_SPEED;
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::SetMovementParameter(
    int moveType,
    float radius,
    bool clockwise,
    const std::vector<VECTOR>& points)
{
    m_MovementType = moveType;
    m_CircleRadius = radius;
    m_Clockwise = clockwise;
    m_PatrolPoints = points;

    SetBasePos(m_Pos);
}

void EnemyBase::UpdateMovementPattern()
{
    if (m_IsKnockback) return;

    float deltaTime = FPSSystem::GetDeltaTime();
    if (deltaTime <= 0.0f) return;

    if (m_MovementType == 0)
    {
        float angleSpeed = CIRCLE_MOVE_SPEED * TARGET_FPS * deltaTime;

        if (m_Clockwise)
        {
            m_Angle += angleSpeed;
        }
        else
        {
            m_Angle -= angleSpeed;
        }

        VECTOR nextPos = m_Pos;
        nextPos.x = m_BasePos.x + cosf(m_Angle) * m_CircleRadius;
        nextPos.z = m_BasePos.z + sinf(m_Angle) * m_CircleRadius;

        m_Move.x = (nextPos.x - m_Pos.x) / (TARGET_FPS * deltaTime);
        m_Move.z = (nextPos.z - m_Pos.z) / (TARGET_FPS * deltaTime);
    }
    else if (m_MovementType == 1)
    {
        if (m_PatrolPoints.empty()) return;

        VECTOR target = m_PatrolPoints[m_TargetPointIdx];
        VECTOR direction = VSub(target, m_Pos);
        direction.y = 0.0f;

        float distance = VSize(direction);

        if (distance < 0.2f)
        {
            m_TargetPointIdx = (m_TargetPointIdx + 1) % m_PatrolPoints.size();
            m_Move.x = 0.0f;
            m_Move.z = 0.0f;
            return;
        }

        direction = VNorm(direction);
        m_Move.x = direction.x * PATROL_MOVE_SPEED * TARGET_FPS * deltaTime;
        m_Move.z = direction.z * PATROL_MOVE_SPEED * TARGET_FPS * deltaTime;
    }
}

void EnemyBase::SetBasePos(VECTOR pos)
{
    m_BasePos = pos;

    if (m_MovementType == 0)
    {
        m_BasePos.x -= m_CircleRadius;
        m_Angle = 0.0f;
    }
}


void EnemyBase::TakeDamage(int damage)
{
    if (IsDead() || IsInvincible()) return;

    Player* player = PlayerManager::GetInstance()->GetPlayer();
    if (!player) return;

    bool isHover = player->GetStateType() == PlayerStateType::HOVER;

    if (!isHover)
    {
        VECTOR direction = VSub(m_Pos, player->GetPos());
        direction.y = 0.0f;

        if (VSize(direction) > 0.0f)
        {
            StartKnockback(direction, 0.3f);
        }
    }

    CharacterBase::TakeDamage(damage);
}


void EnemyBase::StartKnockback(const VECTOR& direction, float power)
{
    m_KnockbackVelocity = VScale(VNorm(direction), power);
    m_KnockbackVelocity.y = 0.4f;
    m_KnockbackTime = KNOCKBACK_TIME;
    m_IsKnockback = true;
}

void EnemyBase::UpdateKnockback()
{
    if (!m_IsKnockback) return;

    float deltaTime = FPSSystem::GetDeltaTime();

    m_Pos.x += m_KnockbackVelocity.x * TARGET_FPS * deltaTime;
    m_Pos.y += m_KnockbackVelocity.y * TARGET_FPS * deltaTime;
    m_Pos.z += m_KnockbackVelocity.z * TARGET_FPS * deltaTime;

    m_KnockbackVelocity.y -= KNOCKBACK_GRAVITY;
    m_KnockbackTime -= deltaTime;

    if (m_KnockbackTime <= 0.0f)
    {
        m_KnockbackTime = 0.0f;
        m_KnockbackVelocity = VGet(0.0f, 0.0f, 0.0f);
        m_IsKnockback = false;
    }
}

void EnemyBase::CheckHitStageObjects(const std::vector<StageObject*>& objects)
{
    if (!m_AABB) return;

    float deltaTime = FPSSystem::GetDeltaTime();

    // XŽ²
    m_Pos.x += m_Move.x * TARGET_FPS * deltaTime;

    for (auto obj : objects)
    {
        if (!obj) continue;

        const CollisionAABB* objAABB = obj->GetAABB();
        if (!objAABB) continue;

        if (m_AABB->CheckAABB(objAABB))
        {
            m_Pos.x = m_PrevPos.x;
            break;
        }
    }

    // YŽ²
    m_Pos.y += m_Move.y * TARGET_FPS * deltaTime;
    m_IsGround = false;

    for (auto obj : objects)
    {
        if (!obj) continue;

        const CollisionAABB* objAABB = obj->GetAABB();
        if (!objAABB) continue;

        if (m_AABB->CheckAABB(objAABB))
        {
            m_Pos.y = m_PrevPos.y;
            m_Move.y = 0.0f;
            m_IsGround = true;
            break;
        }
    }

    // ZŽ²
    m_Pos.z += m_Move.z * TARGET_FPS * deltaTime;

    for (auto obj : objects)
    {
        if (!obj) continue;

        const CollisionAABB* objAABB = obj->GetAABB();
        if (!objAABB) continue;

        if (m_AABB->CheckAABB(objAABB))
        {
            m_Pos.z = m_PrevPos.z;
            break;
        }
    }
}

bool EnemyBase::IsKnockback() const
{
    return m_IsKnockback;
}

bool EnemyBase::IsDeathAnimationFinished() const
{
    return IsDead();
}
