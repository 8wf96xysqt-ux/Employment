#include "CompositeNodeBase.h"

CompositeNodeBase::CompositeNodeBase(BlackBoard* blackBoard, const char* nodeName) : NodeBase(blackBoard, nodeName)
{
}

CompositeNodeBase::~CompositeNodeBase()
{
    for (INode* node : m_ChildNodes)
    {
        delete node;
    }

    m_ChildNodes.clear();
}

void CompositeNodeBase::Init()
{
    NodeBase::Init();

    m_RunningNodeIndex = 0;

    if (m_ChildNodes.empty())
    {
        m_NodeResult = NodeResult::Fail;
        return;
    }

    // 最初の子Nodeを初期化
    m_ChildNodes[m_RunningNodeIndex]->Init();
}

void CompositeNodeBase::Tick()
{
}

void CompositeNodeBase::Finalize()
{
    NodeBase::Finalize();
}

void CompositeNodeBase::AddNode(INode* node)
{
    // 子Nodeを追加
    m_ChildNodes.push_back(node);
}

bool CompositeNodeBase::NodeIncrement()
{
    // 現在のNodeを終了
    m_ChildNodes[m_RunningNodeIndex]->Finalize();

    // 次に実行するNodeのインデックスを取得
    m_RunningNodeIndex = GetNextIndex();

    if (m_RunningNodeIndex >= static_cast<int>(m_ChildNodes.size()))
    {
        return false;
    }

    // 次のNodeを初期化
    m_ChildNodes[m_RunningNodeIndex]->Init();

    return true;
}