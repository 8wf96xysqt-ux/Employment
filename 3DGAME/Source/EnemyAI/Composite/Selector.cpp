#include "Selector.h"

Selector::Selector(BlackBoard* blackBoard) : CompositeNodeBase(blackBoard, "Selector")
{
}

void Selector::Tick()
{
    if (m_ChildNodes.empty())
    {
        m_NodeResult = NodeResult::Fail;
        return;
    }

    // 現在実行中のノードより前にあるノードを確認する
    for (int i = 0; i < m_RunningNodeIndex; ++i)
    {
        m_ChildNodes[i]->Init();
        m_ChildNodes[i]->Tick();

        NodeResult result = m_ChildNodes[i]->GetNodeResult();

        // 優先度の高いNodeが実行可能なら、そちらへ切り替える
        if (result == NodeResult::Success ||
            result == NodeResult::Running)
        {
            m_ChildNodes[m_RunningNodeIndex]->Finalize();

            m_RunningNodeIndex = i;

            m_NodeResult = result;
            return;
        }
    }

    // 現在のノードを実行
    m_ChildNodes[m_RunningNodeIndex]->Tick();

    NodeResult result = m_ChildNodes[m_RunningNodeIndex]->GetNodeResult();

    if (result == NodeResult::Success)
    {
        m_ChildNodes[m_RunningNodeIndex]->Finalize();

        m_NodeResult = NodeResult::Success;
        return;
    }

    if (result == NodeResult::Running)
    {
        m_NodeResult = NodeResult::Running;
        return;
    }

    // Failなら次のノードへ
    if (!NodeIncrement())
    {
        m_NodeResult = NodeResult::Fail;
        return;
    }

    m_NodeResult = NodeResult::Running;
}

int Selector::GetNextIndex() const
{
    return m_RunningNodeIndex + 1;
}