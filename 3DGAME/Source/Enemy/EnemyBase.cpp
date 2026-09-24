#include "EnemyBase.h"
#include "../FPS/FPS.h"
#include "../GameSetting/GameSetting.h"

// コンストラクタ
EnemyBase::EnemyBase()
    : m_AABB(nullptr)
    , m_Handle(-1)
    , m_Move(VGet(0.0f, 0.0f, 0.0f))
    , m_KnockbackVelocity(VGet(0.0f, 0.0f, 0.0f))
    , m_KnockbackTime(0.0f)
    , m_IsKnockback(false)
{
}

// デストラクタ
EnemyBase::~EnemyBase()
{
}

// ダメージ処理
void EnemyBase::TakeDamage(int damage)
{
    // CharacterBaseのダメージ処理
    CharacterBase::TakeDamage(damage);

    // 死亡した場合はノックバックしない
    if (IsDead())
    {
        return;
    }

    // TODO:
    // 実際の攻撃方向・ノックバック量は
    // Playerの攻撃判定から受け取る
}

// ノックバック開始
void EnemyBase::StartKnockback(
    const VECTOR& direction,
    float power)
{
    m_KnockbackVelocity = VNorm(direction);
    m_KnockbackVelocity.y = 0.0f;

    m_KnockbackVelocity =
        VScale(m_KnockbackVelocity, power);

    m_KnockbackTime = 0.2f;
    m_IsKnockback = true;
}

// ノックバック更新
void EnemyBase::UpdateKnockback()
{
    if (!m_IsKnockback)
    {
        return;
    }

    m_Pos.x += m_KnockbackVelocity.x * TARGET_FPS * FPSSystem::GetDeltaTime();

    m_Pos.z +=m_KnockbackVelocity.z *TARGET_FPS * FPSSystem::GetDeltaTime();

    m_KnockbackTime -= FPSSystem::GetDeltaTime();

    if (m_KnockbackTime <= 0.0f)
    {
        m_KnockbackTime = 0.0f;
        m_KnockbackVelocity = VGet(0.0f, 0.0f, 0.0f);
        m_IsKnockback = false;
    }
}

// ノックバック中か
bool EnemyBase::IsKnockback() const
{
    return m_IsKnockback;
}

