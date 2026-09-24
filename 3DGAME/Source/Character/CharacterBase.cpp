#include "CharacterBase.h"
#include "../FPS/FPS.h"

// コンストラクタ
CharacterBase::CharacterBase()
    : m_Pos(VGet(0.0f, 0.0f, 0.0f))
    , m_Rot(VGet(0.0f, 0.0f, 0.0f))
    , m_PrevPos(VGet(0.0f, 0.0f, 0.0f))

    , m_HP(1)
    , m_MaxHP(1)

    , m_IsDead(false)

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

void CharacterBase::SetPos(VECTOR pos)
{
    m_Pos = pos;
}

void CharacterBase::SetRot(VECTOR rot)
{
    m_Rot = rot;
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
