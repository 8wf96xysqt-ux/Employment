    #pragma once
    #include "DxLib.h"
    #include "../Physics/Gravity.h"

class CharacterBase
{
public:
    CharacterBase();
    virtual ~CharacterBase();

    // ダメージ処理
    virtual void TakeDamage(int damage);

    // 死亡処理
    virtual void Die();

    // 無敵処理
    void StartInvincible(float time);
    void EndInvincible();

    // 重力
    void StopGravity();
    void ResetGravity();

    // 座標・回転
    VECTOR GetPos() const;
    VECTOR* GetPosPtr();
    VECTOR GetRot() const;
    void SetPos(VECTOR pos);
    void SetRot(VECTOR rot);

    // HP
    int GetHP() const;
    int GetMaxHP() const;

    // 状態
    bool IsDead() const;
    bool IsInvincible() const;

protected:
    void UpdateInvincible();

    VECTOR m_Pos;
    VECTOR m_Rot;
    VECTOR m_PrevPos;

    int m_HP;
    int m_MaxHP;

    bool m_IsDead;

    bool m_IsInvincible;
    float m_InvincibleTime;

    Gravity m_Gravity;
};