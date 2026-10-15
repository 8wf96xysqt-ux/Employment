#include <fstream>
#include <sstream>
#include <string>
#include "PlayerDataManager.h"

PlayerDataManager* PlayerDataManager::m_Instance = nullptr;

PlayerDataManager::PlayerDataManager()
{
}

PlayerDataManager::~PlayerDataManager()
{
}

void PlayerDataManager::Load()
{
    std::ifstream file("Data/Parameter/PlayerParameter.csv");

    if (!file.is_open())
    {
        return;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream ss(line);
        std::string name;
        std::string value;

        if (!std::getline(ss, name, ','))
        {
            continue;
        }

        if (!std::getline(ss, value, ','))
        {
            continue;
        }

        try
        {
            float data = std::stof(value);

            if (name == "MoveSpeed")
                m_PlayerData.moveSpeed = data;
            else if (name == "AirMoveSpeed")
                m_PlayerData.airMoveSpeed = data;
            else if (name == "RotationSpeed")
                m_PlayerData.rotationSpeed = data;
            else if (name == "MoveInputThreshold")
                m_PlayerData.moveInputThreshold = data;
            else if (name == "RunInputThreshold")
                m_PlayerData.runInputThreshold = data;
            else if (name == "JumpPower")
                m_PlayerData.jumpPower = data;
            else if (name == "JumpHoldPower")
                m_PlayerData.jumpHoldPower = data;
            else if (name == "JumpCutRate")
                m_PlayerData.jumpCutRate = data;
            else if (name == "CoyoteTime")
                m_PlayerData.coyoteTime = data;
            else if (name == "JumpBufferTime")
                m_PlayerData.jumpBufferTime = data;
            else if (name == "FallAnimDelay")
                m_PlayerData.fallAnimDelay = data;
            else if (name == "RollSpeed")
                m_PlayerData.rollSpeed = data;
            else if (name == "RollTime")
                m_PlayerData.rollTime = data;
            else if (name == "HoverRiseSpeed")
                m_PlayerData.hoverRiseSpeed = data;
            else if (name == "HoverRiseHeight")
                m_PlayerData.hoverRiseHeight = data;
            else if (name == "HoverTime")
                m_PlayerData.hoverTime = data;
            else if (name == "HoverMoveSpeed")
                m_PlayerData.hoverMoveSpeed = data;
            else if (name == "Scale")
                m_PlayerData.scale = data;
            else if (name == "AABBSizeX")
                m_PlayerData.aabbSizeX = data;
            else if (name == "AABBSizeY")
                m_PlayerData.aabbSizeY = data;
            else if (name == "AABBSizeZ")
                m_PlayerData.aabbSizeZ = data;
            else if (name == "AABBOffsetX")
                m_PlayerData.aabbOffsetX = data;
            else if (name == "AABBOffsetY")
                m_PlayerData.aabbOffsetY = data;
            else if (name == "AABBOffsetZ")
                m_PlayerData.aabbOffsetZ = data;
            else if (name == "AnimSpeedHover")
                m_PlayerData.animSpeedHover = data;
            else if (name == "AnimSpeedDie")
                m_PlayerData.animSpeedDie = data;
            else if (name == "AnimSpeedFall")
                m_PlayerData.animSpeedFall = data;
            else if (name == "AnimSpeedDashJump")
                m_PlayerData.animSpeedDashJump = data;
            else if (name == "AnimSpeedIdle")
                m_PlayerData.animSpeedIdle = data;
            else if (name == "AnimSpeedJump")
                m_PlayerData.animSpeedJump = data;
            else if (name == "AnimSpeedRun")
                m_PlayerData.animSpeedRun = data;
            else if (name == "AnimSpeedWalk")
                m_PlayerData.animSpeedWalk = data;
            else if (name == "AnimSpeedRolling")
                m_PlayerData.animSpeedRolling = data;
        }
        catch (const std::exception&)
        {
            continue;
        }
    }
}

const PlayerData* PlayerDataManager::GetParameter() const
{
    return &m_PlayerData;
}