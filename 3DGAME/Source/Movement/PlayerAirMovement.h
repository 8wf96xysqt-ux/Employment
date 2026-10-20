#pragma once

class Player;

class PlayerAirMovement
{
public:
    static void Update(Player* player, float moveSpeed, float inputThreshold);
};