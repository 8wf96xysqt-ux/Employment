#include "ReturnToBase.h"
#include "../Blackboard/BlackBoard.h"
#include "../../Enemy/EnemyBase.h"
#include "../../MyMath/MyMath.h"
#include "../../Animation/NormalEnemyAnimParam..h"

constexpr float MOVE_SPEED = 0.05f;
constexpr float STOP_DISTANCE = 0.1f;
constexpr float ROTATE_SPEED = 0.08f;

ReturnToBase::ReturnToBase(BlackBoard* blackBoard)
    : LeafNodeBase(blackBoard, "ReturnToBase")
{
}

void ReturnToBase::Tick()
{
    SetCurrentNode();

    // ベースへ戻る必要があるか確認
    bool returnToBase =
        m_BlackBoard->GetValue<bool>("ReturnToBase");

    if (!returnToBase)
    {
        m_NodeResult = NodeResult::Fail;
        return;
    }

    // 敵を取得
    EnemyBase* enemy =
        m_BlackBoard->GetValue<EnemyBase*>("Enemy");

    if (enemy == nullptr)
    {
        m_NodeResult = NodeResult::Fail;
        return;
    }

    // ベース位置を取得
    VECTOR basePos =
        m_BlackBoard->GetValue<VECTOR>("BasePos");

    // ベースまでの方向を計算
    VECTOR direction = MyMath::VecSub(basePos, enemy->GetPos());

    // 高さ方向は無視する
    direction.y = 0.0f;

    // ベースまでの距離を取得
    float distance = MyMath::VecLong(direction);

    // ベースに到着した場合
    if (distance <= STOP_DISTANCE)
    {
        enemy->SetBasePos(basePos);

        m_BlackBoard->SetValue("ReturnToBase", false);
        m_BlackBoard->SetValue("IsChasing", false);

        enemy->SetMove(VGet(0.0f, 0.0f, 0.0f));

        // 待機アニメーション
        enemy->PlayAnimation(NORMAL_ANIMATION_IDLE, true, 1.0f);

        m_NodeResult = NodeResult::Success;
        return;
    }

    // 移動方向を正規化
    direction = MyMath::VecNormalize(direction);

    // 移動方向へ敵を回転
    float targetYaw = atan2f(direction.x, direction.z);
    enemy->RotateToward(targetYaw, ROTATE_SPEED);

    // ベース方向への移動量を設定
    VECTOR move = VGet(direction.x * MOVE_SPEED, 0.0f, direction.z * MOVE_SPEED);

    enemy->SetMove(move);

    // 歩行アニメーション
    enemy->PlayAnimation(NORMAL_ANIMATION_WALK, true, 1.0f);

    // まだベースへ移動中
    m_NodeResult = NodeResult::Running;
}