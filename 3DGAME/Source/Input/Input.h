#pragma once

#include <cstdint>

namespace Input
{
    enum InputKey : uint32_t
    {
        // キーボード
        KEY_UP = (1u << 0),
        KEY_DOWN = (1u << 1),
        KEY_LEFT = (1u << 2),
        KEY_RIGHT = (1u << 3),

        KEY_Z = (1u << 4),
        KEY_X = (1u << 5),
        KEY_Q = (1u << 6),
        KEY_E = (1u << 7),
        KEY_R = (1u << 8),

        KEY_W = (1u << 9),
        KEY_A = (1u << 10),
        KEY_S = (1u << 11),
        KEY_D = (1u << 12),

        KEY_1 = (1u << 13),
        KEY_SPACE = (1u << 14),
        KEY_L_SHIFT = (1u << 15),


        // ゲームパッド
        KEY_PAD_UP = (1u << 16),
        KEY_PAD_DOWN = (1u << 17),
        KEY_PAD_LEFT = (1u << 18),
        KEY_PAD_RIGHT = (1u << 19),

        KEY_PAD_A = (1u << 20),
        KEY_PAD_B = (1u << 21),
        KEY_PAD_X = (1u << 22),
        KEY_PAD_Y = (1u << 23),

        KEY_PAD_L1 = (1u << 24),
        KEY_PAD_R1 = (1u << 25),
        KEY_PAD_L2 = (1u << 26),
        KEY_PAD_R2 = (1u << 27),

        KEY_PAD_START = (1u << 28),
        KEY_PAD_BACK = (1u << 29),

        KEY_PAD_L_PUSH = (1u << 30),
        KEY_PAD_R_PUSH = (1u << 31)
    };

	enum DeviceType
	{
		DEVICE_NONE,
		DEVICE_KEYBOARD,
		DEVICE_GAMEPAD,
	};

	void Init();
	void Update();
	void Draw();
	void Fin();

	bool IsInputKey(InputKey key);
	bool IsTriggerKey(InputKey key);

	bool IsMoveUp();
	bool IsMoveDown();
	bool IsMoveLeft();
	bool IsMoveRight();
	bool IsJump();
    bool IsTriggerJump();
	bool IsAttack();
    bool IsShooting();
	bool IsMenu();
	bool IsCameraUp();
	bool IsCameraDown();
	bool IsCameraLeft();
	bool IsCameraRight();
	bool IsCameraReset();
    bool IsSkill1();
    bool IsSkill2();
    bool IsSkill3();
    bool IsSkill4();

	float GetStickLX();
	float GetStickLY();
	float GetStickRX();
	float GetStickRY();
}