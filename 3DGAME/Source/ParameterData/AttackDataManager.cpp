#include "AttackDataManager.h"
#include <fstream>
#include <sstream>
#include <unordered_map>

AttackDataManager* AttackDataManager::m_Instance = nullptr;

AttackDataManager::AttackDataManager()
{
}

AttackDataManager::~AttackDataManager()
{
}

void AttackDataManager::Load()
{
    std::ifstream file("Data/Parameter/PlayerAttack.csv");

    if (!file.is_open())
    {
        return;
    }

    // 文字列から列挙型への変換用静的マップ
    static const std::unordered_map<std::string, PlayerAttackType> typeMap = {
        {"Ground1", PlayerAttackType::GROUND_1},
        {"Ground2", PlayerAttackType::GROUND_2},
        {"Air1", PlayerAttackType::AIR_1},
        {"Air2", PlayerAttackType::AIR_2},
        {"Dash1", PlayerAttackType::DASH_1},
        {"Dash2", PlayerAttackType::DASH_2}
    };

    static const std::unordered_map<std::string, PlayerAnimationType> animMap = {
        {"GDCOMBO1", PLAYER_ANIMATION_GDCOMBO1},
        {"GDCOMBO2", PLAYER_ANIMATION_GDCOMBO2},
        {"AIRCOMBO1", PLAYER_ANIMATION_AIRCOMBO1},
        {"AIRCOMBO2", PLAYER_ANIMATION_AIRCOMBO2},
        {"DASHCOMBO1", PLAYER_ANIMATION_DASHCOMBO1},
        {"DASHCOMBO2", PLAYER_ANIMATION_DASHCOMBO2}
    };

    std::string line;
    std::getline(file, line);

    m_AttackData.clear();
    m_AttackData.reserve(16);

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream ss(line);
        std::string value;
        AttackData data;

        if (!std::getline(ss, data.name, ','))
        {
            continue;
        }

        auto typeIt = typeMap.find(data.name);

        if (typeIt == typeMap.end())
        {
            continue;
        }

        data.type = typeIt->second;

        if (!std::getline(ss, value, ','))
        {
            continue;
        }

        auto animIt = animMap.find(value);

        if (animIt != animMap.end())
        {
            data.animation = animIt->second;
        }

        try
        {
            if (std::getline(ss, value, ','))
                data.animationSpeed = std::stof(value);

            if (std::getline(ss, value, ','))
                data.start = std::stof(value);

            if (std::getline(ss, value, ','))
                data.duration = std::stof(value);

            if (std::getline(ss, value, ','))
                data.damage = std::stoi(value);

            if (std::getline(ss, value, ','))
                data.interval = std::stoi(value);

            if (std::getline(ss, value, ','))
                data.chainRate = std::stof(value);

            if (std::getline(ss, value, ','))
                data.moveSpeed = std::stof(value);

            if (std::getline(ss, value, ','))
                data.hitboxSizeX = std::stof(value);

            if (std::getline(ss, value, ','))
                data.hitboxSizeY = std::stof(value);

            if (std::getline(ss, value, ','))
                data.hitboxSizeZ = std::stof(value);

            if (std::getline(ss, value, ','))
                data.hitboxOffsetX = std::stof(value);

            if (std::getline(ss, value, ','))
                data.hitboxOffsetY = std::stof(value);

            if (std::getline(ss, value, ','))
                data.hitboxOffsetZ = std::stof(value);
        }
        catch (const std::exception&)
        {
            continue;
        }

        m_AttackData.push_back(data);
    }
}

const AttackData* AttackDataManager::GetAttackData(PlayerAttackType type) const
{
    for (const auto& data : m_AttackData)
    {
        if (data.type == type)
        {
            return &data;
        }
    }

    return nullptr;
}