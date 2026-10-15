#include "NodeBase.h"
#include "../Blackboard/BlackBoard.h"

NodeBase::NodeBase(BlackBoard* blackBoard, const char* nodeName)

    : m_BlackBoard(blackBoard)
    , m_NodeName(nodeName)
{
}

void NodeBase::Init()
{
    m_NodeResult = NodeResult::Running;
}

void NodeBase::Tick()
{
}

void NodeBase::Finalize()
{
}

NodeResult NodeBase::GetNodeResult() const
{
    return m_NodeResult;
}

void NodeBase::SetCurrentNode()
{
    m_BlackBoard->SetValue("CurrentNode", m_NodeName);
}
const char* NodeBase::GetNodeName() const
{
    return m_NodeName;
}