#pragma once
#include "CompositeNodeBase.h"

class Selector : public CompositeNodeBase
{
public:
    explicit Selector(BlackBoard* blackBoard);
    ~Selector() override = default;

    void Tick() override;

private:
    int GetNextIndex() const override;
};