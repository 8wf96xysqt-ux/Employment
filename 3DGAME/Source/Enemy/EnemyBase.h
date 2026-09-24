#pragma once

#include "../Character/CharacterBase.h"

class CollisionAABB;

class EnemyBase : public CharacterBase
{
public:
    EnemyBase();
    virtual ~EnemyBase();

    // 初期化
    virtual void Init() = 0;
    //ロード
    virtual void Load() = 0;
    //スタート
    virtual void Start() = 0;
    //ステップ
    virtual void Step() = 0;
    // 更新
    virtual void Update() = 0;
    // 描画
    virtual void Draw() = 0;
    //終了
    virtual void Fin() = 0;
    // ダメージ処理
    void TakeDamage(int damage) override;

protected:
    // ノックバック開始
    void StartKnockback(const VECTOR& direction, float power);

    // ノックバック更新
    void UpdateKnockback();

    // ノックバック中か
    bool IsKnockback() const;

    // 当たり判定
    CollisionAABB* m_AABB;

    // モデルハンドル
    int m_Handle;

    // 移動速度
    VECTOR m_Move;

    // ノックバック
    VECTOR m_KnockbackVelocity;
    float m_KnockbackTime;

    // ノックバック中か
    bool m_IsKnockback;
};