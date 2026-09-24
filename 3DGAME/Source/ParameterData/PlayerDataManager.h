#pragma once
#include "PlayerParameter.h"

class PlayerDataManager
{
public:
    PlayerDataManager();
    ~PlayerDataManager();

    static void CreateInstance(){ if (!m_Instance)m_Instance = new PlayerDataManager;}

    static PlayerDataManager* GetInstance(){return m_Instance;}

    static void DeleteInstance(){if (m_Instance)delete m_Instance;m_Instance = nullptr;}

    void Load();

    const PlayerData* GetParameter() const;

private:
    static PlayerDataManager* m_Instance;

    PlayerData m_PlayerData;
};