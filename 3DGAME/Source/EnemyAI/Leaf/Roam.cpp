#include "Roam.h"
#include "../Blackboard/BlackBoard.h"
#include "../../Enemy/EnemyBase.h"
#include "../../Animation/NormalEnemyAnimParam..h"
#include "../../MyMath/MyMath.h"
#include <cmath>

namespace
{
    constexpr float ROTATE_SPEED = 0.08f;
}

Roam::Roam(BlackBoard* blackBoard)
    : LeafNodeBase(blackBoard, "Roam")
    , m_TargetPos(VGet(0.0f, 0.0f, 0.0f))
    , m_HasTarget(false)
{
}

void Roam::Tick()
{
    SetCurrentNode();

    EnemyBase* enemy =
        m_BlackBoard->GetValue<EnemyBase*>("Enemy");

    if (enemy == nullptr)
    {
        m_NodeResult = NodeResult::Fail;
        return;
    }

    // 移動先を更新
    enemy->UpdateMovementPattern();

    VECTOR move = enemy->GetMove();

    if (move.x != 0.0f || move.z != 0.0f)
    {
        // 移動方向へ回転
        float targetYaw = atan2f(move.x, move.z);
        enemy->RotateToward(targetYaw, ROTATE_SPEED);

        // 移動中は歩行アニメーション
        enemy->PlayAnimation(NORMAL_ANIMATION_WALK,true,1.0f);
    }
    else
    {
        // 停止中は待機アニメーション
        enemy->PlayAnimation(NORMAL_ANIMATION_IDLE,true,1.0f);
    }

    m_NodeResult = NodeResult::Running;
}