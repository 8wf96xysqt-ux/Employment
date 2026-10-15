#pragma once
#include "DxLib.h"

class CollisionAABB;
class CollisionOBB;
struct AttackData;

class AttackComponent
{
public:
    AttackComponent();
    ~AttackComponent();

    void Init(VECTOR* targetPos);
    void Fin();

    void StartAttack();
    void StartAttack(const AttackData* data);
    void EndAttack();

    void SetAttackData(const AttackData* data);

    void Update(float animationTime, float rotationY);

    bool CheckHit(CollisionAABB* target) const;

    bool HasAirAttacked() const
    {
        return m_HasAirAttacked;
    }

    void SetHasAirAttacked(bool value)
    {
        m_HasAirAttacked = value;
    }

private:
    CollisionOBB* m_AttackOBB;

    const AttackData* m_AttackData;

    bool m_IsAttack;
    bool m_HasAirAttacked;
};