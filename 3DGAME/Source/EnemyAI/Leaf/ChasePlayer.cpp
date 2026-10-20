#include "ChasePlayer.h"
#include "../Blackboard/BlackBoard.h"
#include "../../Enemy/EnemyBase.h"
#include "../../Player/Player.h"
#include "../../MyMath/MyMath.h"
#include "../../StageObject/StageObjectManager.h"
#include "../../StageObject/StageObject.h"
#include "../../Collision/CollisionAABB.h"
#include "../../Animation/NormalEnemyAnimParam..h"

constexpr float MOVE_SPEED = 0.055f;
constexpr float ROTATE_SPEED = 0.08f;
constexpr float RAY_DISTANCE = 3.0f;
constexpr float RAY_HEIGHT = 0.5f;
constexpr float ENEMY_RADIUS = 0.5f;

// ステージの障害物にレイが当たるか
bool IsBlockedByStage(VECTOR start, VECTOR end)
{
    auto objects = StageObjectManager::GetInstance()->GetStageObjects();

    for (auto obj : objects)
    {
        if (obj == nullptr || !obj->IsPathfindingTarget())
        {
            continue;
        }

        CollisionAABB* aabb = obj->GetAABB();

        if (aabb == nullptr)
        {
            continue;
        }

        if (aabb->CheckRayHit(start, end))
        {
            return true;
        }
    }

    return false;
}

ChasePlayer::ChasePlayer(BlackBoard* blackBoard)
    : LeafNodeBase(blackBoard, "ChasePlayer")
{
}

void ChasePlayer::Tick()
{
    SetCurrentNode();

    EnemyBase* enemy = m_BlackBoard->GetValue<EnemyBase*>("Enemy");

    Player* player = m_BlackBoard->GetValue<Player*>("Player");

    if (enemy == nullptr || player == nullptr)
    {
        m_NodeResult = NodeResult::Fail;
        return;
    }

    VECTOR enemyPos = enemy->GetPos();
    VECTOR playerPos = player->GetPos();

    VECTOR direction = MyMath::VecSub(playerPos, enemyPos);
    direction.y = 0.0f;

    float distance = MyMath::VecLong(direction);

    // プレイヤーとほぼ同じ位置なら移動しない
    if (distance < 0.001f)
    {
        enemy->SetMove(VGet(0.0f, 0.0f, 0.0f));
        enemy->PlayAnimation(NORMAL_ANIMATION_IDLE, true, 1.0f);

        m_NodeResult = NodeResult::Running;
        return;
    }

    direction = MyMath::VecNormalize(direction);

    VECTOR right = VGet(direction.z, 0.0f, -direction.x);
    VECTOR left = VScale(right, -1.0f);

    VECTOR rayStart = VAdd(enemyPos,VGet(0.0f, RAY_HEIGHT, 0.0f));

    // 敵の幅を考慮した3本の前方レイ
    VECTOR centerEnd = VAdd(rayStart, VScale(direction, RAY_DISTANCE));

    VECTOR leftStart = VAdd(rayStart, VScale(left, ENEMY_RADIUS));

    VECTOR rightStart = VAdd(rayStart, VScale(right, ENEMY_RADIUS));

    VECTOR leftEnd = VAdd(leftStart, VScale(direction, RAY_DISTANCE));

    VECTOR rightEnd = VAdd(rightStart, VScale(direction, RAY_DISTANCE));

    bool frontHit =
        IsBlockedByStage(rayStart, centerEnd) ||
        IsBlockedByStage(leftStart, leftEnd) ||
        IsBlockedByStage(rightStart, rightEnd);

    // 基本はプレイヤーへ向かう方向
    VECTOR moveDirection = direction;

    if (frontHit)
    {
        // プレイヤー方向を少し残した斜め方向へ回避
        VECTOR leftDiagonal = MyMath::VecNormalize(VAdd(direction, VScale(left, 1.0f)));

        VECTOR rightDiagonal = MyMath::VecNormalize(VAdd(direction, VScale(right, 1.0f)));

        VECTOR leftDiagonalEnd = VAdd(rayStart,VScale(leftDiagonal, RAY_DISTANCE));

        VECTOR rightDiagonalEnd =VAdd(rayStart,VScale(rightDiagonal, RAY_DISTANCE));

        bool leftBlocked = IsBlockedByStage(rayStart, leftDiagonalEnd);

        bool rightBlocked = IsBlockedByStage(rayStart, rightDiagonalEnd);

        if (!leftBlocked)
        {
            moveDirection = leftDiagonal;
        }
        else if (!rightBlocked)
        {
            moveDirection = rightDiagonal;
        }
        else
        {
            // 左右どちらにも進めない
            enemy->SetMove(VGet(0.0f, 0.0f, 0.0f));
            m_NodeResult = NodeResult::Running;
            return;
        }
    }

    float targetYaw =
        atan2f(moveDirection.x, moveDirection.z);

    enemy->RotateToward(targetYaw, ROTATE_SPEED);

    VECTOR move = VGet(moveDirection.x * MOVE_SPEED,0.0f,moveDirection.z * MOVE_SPEED);

    enemy->SetMove(move);

    enemy->PlayAnimation(NORMAL_ANIMATION_DISCOVERY,true,0.8f);

    m_NodeResult = NodeResult::Running;
}