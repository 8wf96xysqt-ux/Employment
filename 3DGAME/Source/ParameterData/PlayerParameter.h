#pragma once

struct PlayerData
{
    float moveSpeed;
    float airMoveSpeed;
    float rotationSpeed;

    float moveInputThreshold;
    float runInputThreshold;

    float jumpPower;
    float jumpHoldPower;
    float jumpCutRate;

    float coyoteTime;
    float jumpBufferTime;

    float fallAnimDelay;

    float rollSpeed;
    float rollTime;

    float hoverRiseSpeed;
    float hoverRiseHeight;
    float hoverTime;
    float hoverMoveSpeed;

    float scale;

    float aabbSizeX;
    float aabbSizeY;
    float aabbSizeZ;

    float aabbOffsetX;
    float aabbOffsetY;
    float aabbOffsetZ;

    float animSpeedHover;
    float animSpeedDie;
    float animSpeedFall;
    float animSpeedDashJump;
    float animSpeedIdle;
    float animSpeedJump;
    float animSpeedRun;
    float animSpeedWalk;
    float animSpeedRolling;
};