#pragma once
#include "LeafNodeBase.h"
#include <DxLib.h>

class Roam : public LeafNodeBase
{
public:
    explicit Roam(BlackBoard* blackBoard);
    ~Roam() override = default;

    void Tick() override;

private:
    VECTOR m_TargetPos;
    bool m_HasTarget;
};