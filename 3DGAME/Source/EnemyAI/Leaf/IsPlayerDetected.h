#pragma once
#include "LeafNodeBase.h"

class IsPlayerDetected : public LeafNodeBase
{
public:
    explicit IsPlayerDetected(BlackBoard* blackBoard);
    ~IsPlayerDetected() override = default;

    void Tick() override;
};