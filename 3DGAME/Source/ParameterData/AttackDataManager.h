#pragma once
#include "AttackParameter.h"
#include <vector>

class AttackDataManager
{
public:
    AttackDataManager();
    ~AttackDataManager();
public:
    // AttackDataManager‚ğ¶¬‚·‚é
    static void CreateInstance() {if (!m_Instance) m_Instance = new AttackDataManager;}
    // AttackDataManager‚ğæ“¾‚·‚é
    static AttackDataManager* GetInstance(){ return m_Instance; }
    // AttackDataManager‚ğíœ‚·‚é
    static void DeleteInstance(){if (m_Instance) delete m_Instance; m_Instance = nullptr; }

public:
    void Load();
    const AttackData* GetAttackData(PlayerAttackType type) const;
private:
    static AttackDataManager* m_Instance;
    std::vector<AttackData> m_AttackData;
};