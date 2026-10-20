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

    int maxHP;

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

    PlayerData()
        : moveSpeed(0.0f)
        , airMoveSpeed(0.0f)
        , rotationSpeed(0.0f)
        , moveInputThreshold(0.0f)
        , runInputThreshold(0.0f)
        , jumpPower(0.0f)
        , jumpHoldPower(0.0f)
        , jumpCutRate(0.0f)
        , coyoteTime(0.0f)
        , jumpBufferTime(0.0f)
        , fallAnimDelay(0.0f)
        , rollSpeed(0.0f)
        , rollTime(0.0f)
        , hoverRiseSpeed(0.0f)
        , hoverRiseHeight(0.0f)
        , hoverTime(0.0f)
        , hoverMoveSpeed(0.0f)
        , maxHP(3)
        , scale(1.0f)
        , aabbSizeX(0.0f)
        , aabbSizeY(0.0f)
        , aabbSizeZ(0.0f)
        , aabbOffsetX(0.0f)
        , aabbOffsetY(0.0f)
        , aabbOffsetZ(0.0f)
        , animSpeedHover(0.0f)
        , animSpeedDie(0.0f)
        , animSpeedFall(0.0f)
        , animSpeedDashJump(0.0f)
        , animSpeedIdle(0.0f)
        , animSpeedJump(0.0f)
        , animSpeedRun(0.0f)
        , animSpeedWalk(0.0f)
        , animSpeedRolling(0.0f)
    {
    }
};