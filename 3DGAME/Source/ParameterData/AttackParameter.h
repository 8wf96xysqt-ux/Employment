#pragma once
#include "../Animation/AnimationController.h"
#include <string>

enum class PlayerAttackType
{
    GROUND_1,
    GROUND_2,
    AIR_1,
    AIR_2,
    DASH_1,
    DASH_2
};

struct AttackData
{
    // 攻撃の種類
    PlayerAttackType type = PlayerAttackType::GROUND_1;
    // CSV上で使用する攻撃名
    std::string name = "";
    // 再生するアニメーション
    PlayerAnimationType animation = PLAYER_ANIMATION_IDLE;
    // アニメーション再生速度
    float animationSpeed = 1.0f;
    // 攻撃判定の発生時間
    float start = 0.0f;
    // 攻撃判定の持続時間
    float duration = 0.0f;
    // 与えるダメージ
    int damage = 0;
    // 攻撃間隔
    int interval = 0;
    // 次の攻撃へ移行できるタイミング
    float chainRate = 0.8f;
    // 攻撃中の移動速度
    float moveSpeed = 0.0f;
    // 攻撃判定のサイズ
    float hitboxSizeX = 0.8f;
    float hitboxSizeY = 0.8f;
    float hitboxSizeZ = 0.8f;
    // 攻撃判定の位置
    float hitboxOffsetX = 0.0f;
    float hitboxOffsetY = 1.0f;
    float hitboxOffsetZ = 0.7f;
};