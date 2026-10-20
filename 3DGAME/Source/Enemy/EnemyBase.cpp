#include "EnemyBase.h"
#include "../FPS/FPS.h"
#include "../GameSetting/GameSetting.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../State/PlayerStateBase.h"
#include "../StageObject/StageObject.h"
#include "../Collision/CollisionAABB.h"
#include "../Camera/CameraManager.h"
#include "../Random/Random.h"
#include <cmath>



namespace
{
    constexpr float CIRCLE_MOVE_SPEED = 0.02f;
    constexpr float PATROL_MOVE_SPEED = 0.025f;
    constexpr float KNOCKBACK_TIME = 0.35f;
    constexpr float KNOCKBACK_GRAVITY = 0.02f;

    constexpr float MIN_MOVE_WAIT = 0.1f;
    constexpr float MAX_MOVE_WAIT = 1.0f;
}

EnemyBase::EnemyBase()
    : m_KnockbackVelocity(VGet(0.0f, 0.0f, 0.0f))
    , m_KnockbackTime(0.0f)
    , m_IsKnockback(false)
    , m_MoveWaitTime(0.0f)
    , m_MoveTimer(0.0f)
    , m_IsMoveWaiting(true)
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

    // 移動開始タイミングをランダムに設定
    m_MoveWaitTime = Random::GetFloat(MIN_MOVE_WAIT, MAX_MOVE_WAIT);
    m_MoveTimer = 0.0f;
    m_IsMoveWaiting = true;
}

void EnemyBase::UpdateMovementPattern()
{
    if (m_IsKnockback) return;

    float deltaTime = FPSSystem::GetDeltaTime();
    if (deltaTime <= 0.0f) return;

    // ランダムな待機時間が終わるまで移動しない
    if (m_IsMoveWaiting)
    {
        m_MoveTimer += deltaTime;

        m_Move.x = 0.0f;
        m_Move.z = 0.0f;

        if (m_MoveTimer < m_MoveWaitTime)
        {
            return;
        }

        m_IsMoveWaiting = false;
    }

    // ここから下は既存の円移動・巡回移動の処理
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

    // m_PrevPos は呼び出し側で更新されている前提（既存コードのまま）
    ResolveStageCollisions(objects);
}

bool EnemyBase::IsKnockback() const
{
    return m_IsKnockback;
}

bool EnemyBase::IsDeathAnimationFinished() const
{
    return IsDead();
}
