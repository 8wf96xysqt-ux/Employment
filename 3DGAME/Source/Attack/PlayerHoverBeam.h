#pragma once

#include "DxLib.h"

struct AttackData;
class Player;
class EffekseerEffect;
class CollisionOBB;
class CollisionAABB;

class PlayerHoverBeam
{
public:
    PlayerHoverBeam();
    ~PlayerHoverBeam();

    void Start(Player* player, const AttackData* attackData);
    void Update();
    void Fin();

    bool CheckAttackHit(CollisionAABB* target);

private:
    void UpdateBeamOBB();
    void UpdateBeamEffects();

    Player* m_pPlayer;
    const AttackData* m_AttackData;

    EffekseerEffect* m_LeftBeamEffect;
    EffekseerEffect* m_RightBeamEffect;
    EffekseerEffect* m_LeftBeamHitEffect;
    EffekseerEffect* m_RightBeamHitEffect;

    CollisionOBB* m_LeftBeamOBB;
    CollisionOBB* m_RightBeamOBB;
};