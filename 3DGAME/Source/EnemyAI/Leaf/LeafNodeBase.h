#pragma once

#include "../Core/NodeBase.h"

class BlackBoard;

class LeafNodeBase : public NodeBase
{
protected:
    explicit LeafNodeBase(BlackBoard* blackBoard,const char* nodeName);

    virtual ~LeafNodeBase() = default;
};