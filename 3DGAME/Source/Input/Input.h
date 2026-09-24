#pragma once

#include <cstdint>

namespace Input
{
    // 物理入力
    enum InputKey : uint64_t
    {
        // キーボード
        KEY_UP = (1ull << 0),
        KEY_DOWN = (1ull << 1),
        KEY_LEFT = (1ull << 2),
        KEY_RIGHT = (1ull << 3),

        KEY_Z = (1ull << 4),
        KEY_X = (1ull << 5),
        KEY_Q = (1ull << 6),
        KEY_E = (1ull << 7),
        KEY_R = (1ull << 8),

        KEY_W = (1ull << 9),
        KEY_A = (1ull << 10),
        KEY_S = (1ull << 11),
        KEY_D = (1ull << 12),

        KEY_1 = (1ull << 13),
        KEY_SPACE = (1ull << 14),
        KEY_L_SHIFT = (1ull << 15),
        KEY_L_CTRL = (1ull << 16),

        // ゲームパッド
        KEY_PAD_UP = (1ull << 17),
        KEY_PAD_DOWN = (1ull << 18),
        KEY_PAD_LEFT = (1ull << 19),
        KEY_PAD_RIGHT = (1ull << 20),

        KEY_PAD_A = (1ull << 21),
        KEY_PAD_B = (1ull << 22),
        KEY_PAD_X = (1ull << 23),
        KEY_PAD_Y = (1ull << 24),

        KEY_PAD_L1 = (1ull << 25),
        KEY_PAD_R1 = (1ull << 26),
        KEY_PAD_L2 = (1ull << 27),
        KEY_PAD_R2 = (1ull << 28),

        KEY_PAD_START = (1ull << 29),
        KEY_PAD_BACK = (1ull << 30),

        KEY_PAD_L_PUSH = (1ull << 31),
        KEY_PAD_R_PUSH = (1ull << 32)
    };

    // デバイス
    enum DeviceType
    {
        DEVICE_NONE,
        DEVICE_KEYBOARD,
        DEVICE_GAMEPAD
    };

    // 入力システム
    void Init();
    void Update();
    void Draw();
    void Fin();

    // 基本入力
    bool IsInputKey(InputKey key);
    bool IsTriggerKey(InputKey key);
    bool IsReleaseKey(InputKey key);

    // 移動
    bool IsMove();
    VECTOR GetMove();
    bool IsMoveUp();
    bool IsMoveDown();
    bool IsMoveLeft();
    bool IsMoveRight();

    // ジャンプ
    bool IsJump();
    bool IsTriggerJump();
    bool IsReleaseJump();

    // アクション
    bool IsAttack();
    bool IsRolling();
    bool IsShooting();

    // メニュー
    bool IsMenu();

    // カメラ

    bool IsCameraUp();
    bool IsCameraDown();
    bool IsCameraLeft();
    bool IsCameraRight();
    bool IsCameraReset();

    // スキル
    bool IsSkill1();
    bool IsSkill2();
    bool IsSkill3();
    bool IsSkill4();

    // スティック
    float GetStickLX();
    float GetStickLY();
    float GetStickRX();
    float GetStickRY();
}
