#pragma once
#include "EnemyBase.h"

class NormalEnemy : public EnemyBase
{
public:
    NormalEnemy();
    ~NormalEnemy() override;

    void Init() override;
    void Load() override;
    void Start() override;
    void Step() override;
    void Update() override;
    void Draw() override;
    void Fin() override;
};