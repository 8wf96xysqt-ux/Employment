#include "CharacterBase.h"
#include "../Collision/CollisionAABB.h"
#include "../StageObject/StageObject.h"
#include "../FPS/FPS.h"
#include <corecrt_math.h>

// コンストラクタ
CharacterBase::CharacterBase()
    : m_Pos(VGet(0.0f, 0.0f, 0.0f))
    , m_Rot(VGet(0.0f, 0.0f, 0.0f))
    , m_PrevPos(VGet(0.0f, 0.0f, 0.0f))

    // モデル
    , m_Handle(-1)
    , m_Scale(VGet(1.0f, 1.0f, 1.0f))

    // 移動
    , m_Move(VGet(0.0f, 0.0f, 0.0f))

    // 当たり判定
    , m_AABB(nullptr)

    // 接地
    , m_IsGround(true)

    // HP
    , m_HP(1)
    , m_MaxHP(1)

    , m_IsDead(false)

    // 無敵
    , m_IsInvincible(false)
    , m_InvincibleTime(0.0f)
{
}

// デストラクタ
CharacterBase::~CharacterBase()
{
}

// ダメージ処理
void CharacterBase::TakeDamage(int damage)
{
    // 死亡中・無敵中はダメージを受けない
    if (m_IsDead || m_IsInvincible)
    {
        return;
    }

    m_HP -= damage;

    // HPが0以下になったら死亡
    if (m_HP <= 0)
    {
        m_HP = 0;
        Die();
    }
}

// 死亡処理
void CharacterBase::Die()
{
    m_IsDead = true;
}

// 無敵開始
void CharacterBase::StartInvincible(float time)
{
    m_IsInvincible = true;
    m_InvincibleTime = time;
}

// 無敵終了
void CharacterBase::EndInvincible()
{
    m_IsInvincible = false;
    m_InvincibleTime = 0.0f;
}

// 重力停止
void CharacterBase::StopGravity()
{
    m_Gravity.Stop();
}

// 重力再開
void CharacterBase::ResetGravity()
{
    m_Gravity.Reset();
}

// 座標設定
void CharacterBase::SetPos(VECTOR pos)
{
    m_Pos = pos;
}

// 回転設定
void CharacterBase::SetRot(VECTOR rot)
{
    m_Rot = rot;
}

// 目標方向へ回転
void CharacterBase::RotateToward(float targetYaw, float rotationSpeed)
{
    while (targetYaw < 0.0f)
    {
        targetYaw += DX_TWO_PI_F;
    }

    while (targetYaw >= DX_TWO_PI_F)
    {
        targetYaw -= DX_TWO_PI_F;
    }

    float difference = targetYaw - m_Rot.y;

    // 最短方向に回転する
    if (difference > DX_PI_F)
    {
        difference -= DX_TWO_PI_F;
    }
    else if (difference < -DX_PI_F)
    {
        difference += DX_TWO_PI_F;
    }

    if (fabsf(difference) <= rotationSpeed)
    {
        m_Rot.y = targetYaw;
    }
    else if (difference > 0.0f)
    {
        m_Rot.y += rotationSpeed;
    }
    else
    {
        m_Rot.y -= rotationSpeed;
    }

    while (m_Rot.y < 0.0f)
    {
        m_Rot.y += DX_TWO_PI_F;
    }

    while (m_Rot.y >= DX_TWO_PI_F)
    {
        m_Rot.y -= DX_TWO_PI_F;
    }
}

// 無敵時間更新
void CharacterBase::UpdateInvincible()
{
    if (!m_IsInvincible)
    {
        return;
    }

    m_InvincibleTime -= FPSSystem::GetDeltaTime();

    if (m_InvincibleTime <= 0.0f)
    {
        m_InvincibleTime = 0.0f;
        m_IsInvincible = false;
    }
}

// 座標取得
VECTOR CharacterBase::GetPos() const
{
    return m_Pos;
}

// 座標ポインタ取得
VECTOR* CharacterBase::GetPosPtr()
{
    return &m_Pos;
}

// 回転取得
VECTOR CharacterBase::GetRot() const
{
    return m_Rot;
}

// HP取得
int CharacterBase::GetHP() const
{
    return m_HP;
}

// 最大HP取得
int CharacterBase::GetMaxHP() const
{
    return m_MaxHP;
}

// 死亡状態取得
bool CharacterBase::IsDead() const
{
    return m_IsDead;
}

// 無敵状態取得
bool CharacterBase::IsInvincible() const
{
    return m_IsInvincible;
}

// 接地状態取得
bool CharacterBase::IsGround() const
{
    return m_IsGround;
}

// 接地状態設定
void CharacterBase::SetGround(bool value)
{
    m_IsGround = value;
}

