#pragma once
#include "DxLib.h"
#include "../Physics/Gravity.h"
#include "../Animation/AnimationController.h"
#include <vector>

class StageObject;
class CollisionAABB;

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

    // 目標方向へ回転
    void RotateToward(float targetYaw, float rotationSpeed);

    // HP
    int GetHP() const;
    int GetMaxHP() const;

    // 状態
    bool IsDead() const;
    bool IsInvincible() const;
    bool IsGround() const;
    void SetGround(bool value);
    CollisionAABB* GetAABB() const{return m_AABB;}

    // 移動量
    VECTOR GetMove() const { return m_Move; }
    void SetMove(VECTOR move) { m_Move = move; }

    // ステージオブジェクトとの当たり判定解決（共通実装）
    // deltaTime は FPSSystem::GetDeltaTime() を渡すこと
    // 戻り値: Y方向で着地したかどうか
    bool ResolveStageCollisions(const std::vector<StageObject*>& objects, float deltaTime);

    // 着地 / 離地のフック（必要なら派生クラスでオーバーライド）
    virtual void OnLand() {}
    virtual void OnLeaveGround() {}

protected:
    // 無敵時間更新
    void UpdateInvincible();

    VECTOR m_Pos;
    VECTOR m_Rot;
    VECTOR m_PrevPos;
    
    // モデル
    int m_Handle;
    VECTOR m_Scale;

    // 移動
    VECTOR m_Move;

    // 当たり判定
    CollisionAABB* m_AABB;

    // アニメーション
    AnimationController m_Animation;

    // 接地
    bool m_IsGround;

    // HP
    int m_HP;
    int m_MaxHP;

    bool m_IsDead;

    // 無敵
    bool m_IsInvincible;
    float m_InvincibleTime;

    // 重力
    Gravity m_Gravity;
};