#pragma once

#include "INode.h"
#include <string>

class BlackBoard;

class NodeBase : public INode
{
protected:
    explicit NodeBase(BlackBoard* blackBoard,const char* nodeName);

    virtual ~NodeBase() = default;

    void Init() override;
    void Tick() override;
    void Finalize() override;

    NodeResult GetNodeResult() const override;

    // åªç›é¿çsíÜÇÃÉmÅ[ÉhÇ∆ÇµÇƒìoò^
    void SetCurrentNode();

public:
    const char* GetNodeName() const;

protected:
    NodeResult m_NodeResult = NodeResult::Idle;
    BlackBoard* m_BlackBoard = nullptr;

    const char* m_NodeName = nullptr;
};