#pragma once
#include "CompositeNodeBase.h"

class Sequence : public CompositeNodeBase
{
public:
    explicit Sequence(BlackBoard* blackBoard);
    ~Sequence() override = default;

    void Tick() override;

private:
    int GetNextIndex() const override;
};