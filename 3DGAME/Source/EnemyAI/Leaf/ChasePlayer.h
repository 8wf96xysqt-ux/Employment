#pragma once
#include "LeafNodeBase.h"

class ChasePlayer : public LeafNodeBase
{
public:
    explicit ChasePlayer(BlackBoard* blackBoard);
    ~ChasePlayer() override = default;

    void Tick() override;
};