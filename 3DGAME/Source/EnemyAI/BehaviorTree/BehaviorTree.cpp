#include "BehaviorTree.h"
#include "../Core/INode.h"
#include "../Blackboard/BlackBoard.h"

BehaviorTree::BehaviorTree()
    : m_RootNode(nullptr)
    , m_BlackBoard(new BlackBoard())
{
}

BehaviorTree::~BehaviorTree()
{
    delete m_RootNode;
    delete m_BlackBoard;
}

void BehaviorTree::Init()
{
    if (m_RootNode == nullptr)
    {
        return;
    }

    m_RootNode->Init();
}

void BehaviorTree::Tick()
{
    if (m_RootNode == nullptr)
    {
        return;
    }

    // RootNodeからツリー全体を更新
    m_RootNode->Tick();
}

void BehaviorTree::Finalize()
{
    if (m_RootNode == nullptr)
    {
        return;
    }

    m_RootNode->Finalize();
}

void BehaviorTree::SetRootNode(INode* rootNode)
{
    m_RootNode = rootNode;
}

BlackBoard* BehaviorTree::GetBlackBoard()
{
    return m_BlackBoard;
}

const char* BehaviorTree::GetCurrentNodeName() const
{
    if (!m_BlackBoard->HasKey("CurrentNode"))
    {
        return "None";
    }

    // BlackBoardから現在実行中のNode名を取得する
    return m_BlackBoard->GetValue<const char*>("CurrentNode");
}