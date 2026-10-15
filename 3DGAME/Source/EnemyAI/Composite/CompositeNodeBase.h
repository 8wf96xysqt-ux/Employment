#pragma once
#include "../Core/NodeBase.h"
#include <vector>

class CompositeNodeBase : public NodeBase
{
public:
    void AddNode(INode* node);
protected:
    explicit CompositeNodeBase(BlackBoard* blackBoard,const char* nodeName);
    virtual ~CompositeNodeBase();

    void Init() override;
    void Tick() override;
    void Finalize() override;

    bool NodeIncrement();

    virtual int GetNextIndex() const = 0;

protected:
    std::vector<INode*> m_ChildNodes;
    int m_RunningNodeIndex = 0;
};