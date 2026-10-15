#include "Sequence.h"

Sequence::Sequence(BlackBoard* blackBoard) : CompositeNodeBase(blackBoard, "Sequence")
{
}

void Sequence::Tick()
{
    // 先頭の条件ノードを毎フレーム確認
    m_ChildNodes[0]->Init();
    m_ChildNodes[0]->Tick();

    NodeResult conditionResult = m_ChildNodes[0]->GetNodeResult();

    // 条件を満たしていない
    if (conditionResult == NodeResult::Fail)
    {
        m_NodeResult = NodeResult::Fail;
        Finalize();
        return;
    }

    // 条件を満たしている場合
    if (m_RunningNodeIndex == 0)
    {
        m_RunningNodeIndex = 1;
        m_ChildNodes[m_RunningNodeIndex]->Init();
    }

    m_ChildNodes[m_RunningNodeIndex]->Tick();

    NodeResult result = m_ChildNodes[m_RunningNodeIndex]->GetNodeResult();

    if (result == NodeResult::Success)
    {
        m_NodeResult = NodeResult::Success;
        Finalize();
        return;
    }

    if (result == NodeResult::Fail)
    {
        m_NodeResult = NodeResult::Fail;
        Finalize();
        return;
    }

    // Nodeの処理が継続中
    m_NodeResult = NodeResult::Running;
}

int Sequence::GetNextIndex() const
{
    return m_RunningNodeIndex + 1;
}