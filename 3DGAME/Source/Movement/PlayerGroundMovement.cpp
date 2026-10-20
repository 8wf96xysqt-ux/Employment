#include "PlayerGroundMovement.h"
#include "../Player/Player.h"
#include "../MyMath/MyMath.h"

#include <cmath>

bool PlayerGroundMovement::Update(
    Player* player,
    float speed,
    float inputThreshold)
{
    if (player == nullptr)
    {
        return false;
    }

    // カメラ基準の移動方向を取得
    VECTOR move = player->GetCameraMoveInput();

    float inputLength = MyMath::VecLong(move);

    // 入力がなければ水平方向の移動を止める
    if (inputLength <= inputThreshold)
    {
        move = player->GetMove();
        move.x = 0.0f;
        move.z = 0.0f;

        player->SetMove(move);

        return false;
    }

    // 移動方向を正規化
    move = MyMath::VecNormalize(move);

    // 入力の大きさに応じて移動速度を調整
    float moveSpeed = speed * inputLength;

    VECTOR velocity = player->GetMove();

    velocity.x = move.x * moveSpeed;
    velocity.z = move.z * moveSpeed;

    player->SetMove(velocity);

    // 移動方向へプレイヤーを向ける
    player->SetTargetYaw(atan2f(move.x, move.z));
    player->DirectionRot();

    return true;
}