#include "DxLib.h"
#include "Input.h"
#include <cmath>

uint64_t g_InputState = 0;
uint64_t g_PrevInputState = 0;

float g_StickLX = 0.0f;
float g_StickLY = 0.0f;
float g_StickRX = 0.0f;
float g_StickRY = 0.0f;

namespace Input
{
    void Init()
    {
        g_InputState = 0;
        g_PrevInputState = 0;

        g_StickLX = 0.0f;
        g_StickLY = 0.0f;
        g_StickRX = 0.0f;
        g_StickRY = 0.0f;
    }

    void Update()
    {
        g_PrevInputState = g_InputState;
        g_InputState = 0;

        // キーボード入力
        if (CheckHitKey(KEY_INPUT_UP))    g_InputState |= KEY_UP;
        if (CheckHitKey(KEY_INPUT_DOWN))  g_InputState |= KEY_DOWN;
        if (CheckHitKey(KEY_INPUT_LEFT))  g_InputState |= KEY_LEFT;
        if (CheckHitKey(KEY_INPUT_RIGHT)) g_InputState |= KEY_RIGHT;

        if (CheckHitKey(KEY_INPUT_W)) g_InputState |= KEY_W;
        if (CheckHitKey(KEY_INPUT_A)) g_InputState |= KEY_A;
        if (CheckHitKey(KEY_INPUT_S)) g_InputState |= KEY_S;
        if (CheckHitKey(KEY_INPUT_D)) g_InputState |= KEY_D;

        if (CheckHitKey(KEY_INPUT_Z))     g_InputState |= KEY_Z;
        if (CheckHitKey(KEY_INPUT_X))     g_InputState |= KEY_X;
        if (CheckHitKey(KEY_INPUT_Q))     g_InputState |= KEY_Q;
        if (CheckHitKey(KEY_INPUT_E))     g_InputState |= KEY_E;
        if (CheckHitKey(KEY_INPUT_R))     g_InputState |= KEY_R;

        if (CheckHitKey(KEY_INPUT_1))     g_InputState |= KEY_1;
        if (CheckHitKey(KEY_INPUT_SPACE)) g_InputState |= KEY_SPACE;
        if (CheckHitKey(KEY_INPUT_LSHIFT)) g_InputState |= KEY_L_SHIFT;
        if (CheckHitKey(KEY_INPUT_LCONTROL)) g_InputState |= KEY_L_CTRL;

        // ゲームパッド入力
        int pad = GetJoypadInputState(DX_INPUT_PAD1);

#ifdef _DEBUG
        DrawFormatString(
            0,
            600,
            GetColor(255, 255, 255),
            "PAD:%08X",
            pad
        );
#endif

        if (pad & PAD_INPUT_UP)    g_InputState |= KEY_PAD_UP;
        if (pad & PAD_INPUT_DOWN)  g_InputState |= KEY_PAD_DOWN;
        if (pad & PAD_INPUT_LEFT)  g_InputState |= KEY_PAD_LEFT;
        if (pad & PAD_INPUT_RIGHT) g_InputState |= KEY_PAD_RIGHT;

        if (pad & PAD_INPUT_1) g_InputState |= KEY_PAD_A;
        if (pad & PAD_INPUT_2) g_InputState |= KEY_PAD_B;
        if (pad & PAD_INPUT_3) g_InputState |= KEY_PAD_X;
        if (pad & PAD_INPUT_4) g_InputState |= KEY_PAD_Y;

        if (pad & PAD_INPUT_5) g_InputState |= KEY_PAD_L1;
        if (pad & PAD_INPUT_6) g_InputState |= KEY_PAD_R1;

        if (pad & PAD_INPUT_7) g_InputState |= KEY_PAD_BACK;
        if (pad & PAD_INPUT_8) g_InputState |= KEY_PAD_START;

        if (pad & PAD_INPUT_11) g_InputState |= KEY_PAD_L2;
        if (pad & PAD_INPUT_12) g_InputState |= KEY_PAD_R2;

        if (pad & PAD_INPUT_9)  g_InputState |= KEY_PAD_L_PUSH;
        if (pad & PAD_INPUT_10) g_InputState |= KEY_PAD_R_PUSH;

        // 左スティック
        int lx = 0;
        int ly = 0;

        GetJoypadAnalogInput(&lx, &ly, DX_INPUT_PAD1);

        g_StickLX = lx / 1000.0f;
        g_StickLY = ly / 1000.0f;

        float leftMag = sqrtf(
            g_StickLX * g_StickLX +
            g_StickLY * g_StickLY
        );

        if (leftMag < 0.2f)
        {
            g_StickLX = 0.0f;
            g_StickLY = 0.0f;
        }

        // 右スティック
        int rx = 0;
        int ry = 0;

        GetJoypadAnalogInputRight(&rx, &ry, DX_INPUT_PAD1);

        g_StickRX = rx / 1000.0f;
        g_StickRY = ry / 1000.0f;

        float rightMag = sqrtf(g_StickRX * g_StickRX + g_StickRY * g_StickRY);

        if (rightMag < 0.1f)
        {
            g_StickRX = 0.0f;
            g_StickRY = 0.0f;
        }
    }

    void Draw()
    {
    }

    void Fin()
    {
    }


    // 基本入力

    bool IsInputKey(InputKey key)
    {
        return (g_InputState & key) != 0;
    }

    bool IsTriggerKey(InputKey key)
    {
        return (g_InputState & key) && !(g_PrevInputState & key);
    }

    bool IsReleaseKey(InputKey key)
    {
        return !(g_InputState & key) && (g_PrevInputState & key);
    }


    // 移動

    bool IsMoveUp()
    {
        return IsInputKey(KEY_UP) ||IsInputKey(KEY_PAD_UP) ||IsInputKey(KEY_W) ||GetStickLY() > 0.1f;
    }

    bool IsMoveDown()
    {
        return IsInputKey(KEY_DOWN) ||IsInputKey(KEY_PAD_DOWN) ||IsInputKey(KEY_S) ||GetStickLY() < -0.1f;
    }

    bool IsMoveLeft()
    {
        return IsInputKey(KEY_LEFT) ||IsInputKey(KEY_PAD_LEFT) ||IsInputKey(KEY_A) ||GetStickLX() < -0.1f;
    }

    bool IsMoveRight()
    {
        return IsInputKey(KEY_RIGHT) ||IsInputKey(KEY_PAD_RIGHT) || IsInputKey(KEY_D) || GetStickLX() > 0.1f;
    }

    bool IsMove()
    {
        return IsMoveUp() ||IsMoveDown() ||IsMoveLeft() ||IsMoveRight();
    }

    VECTOR GetMove()
    {
        float x = GetStickLX();
        float y = -GetStickLY();

        if (IsInputKey(KEY_A) || IsInputKey(KEY_LEFT))
            x -= 1.0f;

        if (IsInputKey(KEY_D) || IsInputKey(KEY_RIGHT))
            x += 1.0f;

        if (IsInputKey(KEY_W) || IsInputKey(KEY_UP))
            y += 1.0f;

        if (IsInputKey(KEY_S) || IsInputKey(KEY_DOWN))
            y -= 1.0f;

        VECTOR move = VGet(x, 0.0f, y);

        float length = sqrtf(move.x * move.x + move.z * move.z);

        if (length > 1.0f)
        {
            move.x /= length;
            move.z /= length;
        }

        return move;
    }



    // ジャンプ

    bool IsJump()
    {
        return IsInputKey(KEY_Z) ||
            IsInputKey(KEY_PAD_A);
    }

    bool IsTriggerJump()
    {
        return IsTriggerKey(KEY_Z) ||
            IsTriggerKey(KEY_PAD_A);
    }

    bool IsReleaseJump()
    {
        return IsReleaseKey(KEY_Z) ||
            IsReleaseKey(KEY_PAD_A);
    }


    // アクション

    bool IsAttack()
    {
        return IsTriggerKey(KEY_X) || IsTriggerKey(KEY_PAD_X);
    }

    bool IsRolling()
    {
        return IsTriggerKey(KEY_SPACE) || IsTriggerKey(KEY_PAD_B);
    }

    bool IsShooting()
    {
        return false;
    }


    // メニュー

    bool IsMenu()
    {
        return IsTriggerKey(KEY_1) || IsTriggerKey(KEY_PAD_START);
    }


    // カメラ

    bool IsCameraUp()
    {
        return IsInputKey(KEY_W);
    }

    bool IsCameraDown()
    {
        return IsInputKey(KEY_S);
    }

    bool IsCameraLeft()
    {
        return IsInputKey(KEY_A);
    }

    bool IsCameraRight()
    {
        return IsInputKey(KEY_D);
    }

    bool IsCameraReset()
    {
        return IsTriggerKey(KEY_SPACE) || IsTriggerKey(KEY_PAD_R_PUSH);
    }


    // スキル

    bool IsSkill1()
    {
        return false;
    }

    bool IsSkill2()
    {
        return false;
    }

    bool IsSkill3()
    {
        return false;
    }

    bool IsSkill4()
    {
        return false;
    }


    // スティック

    float GetStickLX()
    {
        return g_StickLX;
    }

    float GetStickLY()
    {
        return g_StickLY;
    }

    float GetStickRX()
    {
        return g_StickRX;
    }

    float GetStickRY()
    {
        return g_StickRY;
    }
}
