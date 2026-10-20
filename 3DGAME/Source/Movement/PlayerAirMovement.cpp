#include "PlayerAirMovement.h"
#include "../Player/Player.h"
#include "../MyMath/MyMath.h"

#include <cmath>

void PlayerAirMovement::Update(Player* player, float moveSpeed, float inputThreshold)
{
    if (player == nullptr)
    {
        return;
    }

    VECTOR move = player->GetMove();
    VECTOR airMove = player->GetCameraMoveInput();

    float length = MyMath::VecLong(airMove);

    if (length > inputThreshold)
    {
        airMove = MyMath::VecNormalize(airMove);

        move.x = airMove.x * moveSpeed;
        move.z = airMove.z * moveSpeed;

        player->SetTargetYaw(atan2f(airMove.x, airMove.z));
        player->DirectionRot();
    }
    else
    {
        move.x = 0.0f;
        move.z = 0.0f;
    }

    player->SetMove(move);
}