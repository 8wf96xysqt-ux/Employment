#pragma once

class Player;

class PlayerGroundMovement
{
public:
    static bool Update(Player* player, float speed, float inputThreshold);
};