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
    DASH_2,
    HOVER,
};

struct AttackData
{
    // 攻撃の種類
    PlayerAttackType type;
    // CSV上で使用する攻撃名
    std::string name;
    // 再生するアニメーション
    PlayerAnimationType animation;
    // アニメーション再生速度
    float animationSpeed;
    // 攻撃判定の発生時間
    float start;
    // 攻撃判定の持続時間
    float duration;
    // 与えるダメージ
    int damage;
    // 攻撃間隔
    int interval;
    // 次の攻撃へ移行できるタイミング
    float chainRate;
    // 攻撃中の移動速度
    float moveSpeed;
    // 攻撃判定のサイズ
    float hitboxSizeX;
    float hitboxSizeY;
    float hitboxSizeZ;
    // 攻撃判定の位置
    float hitboxOffsetX;
    float hitboxOffsetY;
    float hitboxOffsetZ;
    // ホバービームのRayの長さ
    float rayLength;

    AttackData()
        : type(PlayerAttackType::GROUND_1)
        , animation(PLAYER_ANIMATION_IDLE)
        , animationSpeed(0.0f)
        , start(0.0f)
        , duration(0.0f)
        , damage(0)
        , interval(0)
        , chainRate(0.0f)
        , moveSpeed(0.0f)
        , hitboxSizeX(0.0f)
        , hitboxSizeY(0.0f)
        , hitboxSizeZ(0.0f)
        , hitboxOffsetX(0.0f)
        , hitboxOffsetY(0.0f)
        , hitboxOffsetZ(0.0f)
        , rayLength(0.0f)
    {
    }
};