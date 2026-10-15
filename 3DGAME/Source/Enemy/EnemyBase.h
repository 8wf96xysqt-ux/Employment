#pragma once
#include "../Character/CharacterBase.h"
#include <vector>

class StageObject;
class CollisionAABB;

class EnemyBase : public CharacterBase
{
public:
    EnemyBase();
    virtual ~EnemyBase();

    // 純粋仮想関数（インターフェースの定義）
    virtual void Init() = 0;
    virtual void Load() = 0;
    virtual void Start() = 0;
    virtual void Step() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Fin() = 0;

    // 共通の公開関数
    void TakeDamage(int damage) override;
    void CheckHitStageObjects(const std::vector<StageObject*>& objects);
    bool IsKnockback() const;
    virtual bool IsDeathAnimationFinished() const;

    // Unityからロードした移動データをセットする関数（宣言のみ）
    void SetMovementParameter(int moveType, float radius, bool clockwise, const std::vector<VECTOR>& points);

    // 外部のAI（Roamノード等）から移動制御を行うための関数（宣言のみ）
    void UpdateMovementPattern();
    void SetBasePos(VECTOR pos);
protected:
    // 内部制御関数
    void StartKnockback(const VECTOR& direction, float power);
    void UpdateKnockback();

    // ノックバック用変数
    VECTOR m_KnockbackVelocity;
    float m_KnockbackTime;
    bool m_IsKnockback;

    // Unityから引き継ぐ移動設定パラメータ
    int m_MovementType = 0;
    float m_CircleRadius = 0.0f;
    bool m_Clockwise = true;
    std::vector<VECTOR> m_PatrolPoints;

    // 移動計算用の内部制御変数
    VECTOR m_BasePos;
    float m_Angle = 0.0f;
    int m_TargetPointIdx = 0;
    float m_MovementSpeed = 0.1f;


    bool m_IsDeathAnimationStarted = false;

};
