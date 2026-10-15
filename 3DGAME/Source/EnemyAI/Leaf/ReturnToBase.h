#pragma once
#include "LeafNodeBase.h"

class ReturnToBase : public LeafNodeBase
{
public:
    explicit ReturnToBase(BlackBoard* blackBoard);
    ~ReturnToBase() override = default;

    void Tick() override;
};