#include "IsPlayerDetected.h"
#include "../Blackboard/BlackBoard.h"
#include "../../Enemy/EnemyBase.h"
#include "../../Player/Player.h"
#include "../../MyMath/MyMath.h"

constexpr float FOLLOW_START_RANGE = 4.0f;
constexpr float FOLLOW_END_RANGE = 5.0f;

IsPlayerDetected::IsPlayerDetected(BlackBoard* blackBoard)
    : LeafNodeBase(blackBoard, "IsPlayerDetected")
{
}

void IsPlayerDetected::Tick()
{
    SetCurrentNode();

    EnemyBase* enemy =
        m_BlackBoard->GetValue<EnemyBase*>("Enemy");

    Player* player =
        m_BlackBoard->GetValue<Player*>("Player");

    if (enemy == nullptr || player == nullptr)
    {
        m_NodeResult = NodeResult::Fail;
        return;
    }

    VECTOR direction =
        MyMath::VecSub(player->GetPos(), enemy->GetPos());

    direction.y = 0.0f;

    float distance = MyMath::VecLong(direction);

    bool isChasing =
        m_BlackBoard->GetValue<bool>("IsChasing");

    if (!isChasing)
    {
        if (distance <= FOLLOW_START_RANGE)
        {
            m_BlackBoard->SetValue("IsChasing", true);

            m_NodeResult = NodeResult::Success;
            return;
        }

        m_NodeResult = NodeResult::Fail;
        return;
    }

    if (distance >= FOLLOW_END_RANGE)
    {
        m_BlackBoard->SetValue("IsChasing", false);
        m_BlackBoard->SetValue("ReturnToBase", true);

        m_NodeResult = NodeResult::Fail;
        return;
    }

    m_NodeResult = NodeResult::Success;
}