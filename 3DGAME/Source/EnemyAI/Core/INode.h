#pragma once
#include "NodeResult.h"

class INode
{
public:
    virtual ~INode() = default;

    virtual void Init() = 0;
    virtual void Tick() = 0;
    virtual void Finalize() = 0;

    virtual NodeResult GetNodeResult() const = 0;
};